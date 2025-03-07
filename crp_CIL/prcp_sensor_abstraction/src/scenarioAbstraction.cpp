#include "prcp_sensor_abstraction/scenarioAbstraction.hpp"


crp::cil::ScenarioAbstraction::ScenarioAbstraction() : Node("scenario_abstraction")
{
    this->declare_parameter<float>("local_path_length", 75.0);
    this->declare_parameter<std::string>("localization_source", "ekf");

    std::string localizationSource;
    this->get_parameter("localization_source", localizationSource);

    std::string localizationTopic = "";
    if (localizationSource == "ekf")
        localizationTopic = "/sensing/ekf/estimated_pose";
    else if (localizationSource == "gnss")
        localizationTopic = "/sensing/gnss/pose_with_covariance";
    else
        throw std::runtime_error("Unknown localization source: " + localizationSource);

    m_laneletMap = std::make_shared<lanelet::LaneletMap>();

    m_sub_staticMapFromFile_ = this->create_subscription<autoware_map_msgs::msg::LaneletMapBin>(
        "map/global_static_map_from_file/lanelet2_map", rclcpp::QoS{1}.transient_local(), std::bind(&ScenarioAbstraction::staticMapFromFileCallback, this, std::placeholders::_1));

    m_sub_pose_ = this->create_subscription<geometry_msgs::msg::PoseWithCovarianceStamped>(
        localizationTopic, 10, std::bind(&ScenarioAbstraction::poseCallback, this, std::placeholders::_1));

    m_pub_movingObjects_   = this->create_publisher<autoware_perception_msgs::msg::PredictedObjects>("cai/local_moving_objects", 10);
    m_pub_obstacles_       = this->create_publisher<autoware_perception_msgs::msg::PredictedObjects>("cai/local_obstacles", 10);
    m_pub_possiblePaths_   = this->create_publisher<crp_msgs::msg::PathWithTrafficRulesArray>("cai/local_lane/possible_paths", 10);
    m_pub_drivableSurface_ = this->create_publisher<nav_msgs::msg::OccupancyGrid>("cai/local_drivable_surface", 10);

    m_publishTimer_ = this->create_wall_timer(
        std::chrono::milliseconds(50), std::bind(&ScenarioAbstraction::publishCallback, this));
    
    RCLCPP_INFO(this->get_logger(), "scenario_abstraction has been started");
}

crp_msgs::msg::PathWithTrafficRules crp::cil::ScenarioAbstraction::extractLaneletToCompleteLength(
    const lanelet::ConstLanelet&                         lanelet,
    const float&                                         currentCompletePathLength,
    const tier4_planning_msgs::msg::PathPointWithLaneId& prevPoint,
    uint16_t                                             currentPointIdx=0
)
{
    crp_msgs::msg::PathWithTrafficRules lanePath;

    float laneletSpeedLimit = m_trafficRules->speedLimit(lanelet).speedLimit.value();
    lanePath.path_length = 0.0f;
    lanelet::ConstLineString2d currentCenterline = lanelet.centerline2d();
    tier4_planning_msgs::msg::PathPointWithLaneId currentPrevPoint = prevPoint;
    while (lanePath.path_length+currentCompletePathLength < m_localPathLength && currentPointIdx < currentCenterline.size())
    {
        // add points to path from current lanelet
        tier4_planning_msgs::msg::PathPointWithLaneId currentPoint = m_abstractionUtils.laneletPtToPathPoint(currentCenterline[currentPointIdx],laneletSpeedLimit);
        lanePath.path_length += m_abstractionUtils.distanceBetweenPoints(currentPrevPoint, currentPoint);
        lanePath.path.points.push_back(
            m_abstractionUtils.transformToLocal(currentPoint, m_egoPoseMapFrame)
        );
        currentPrevPoint = currentPoint;
        currentPointIdx++;
    }

    // calculate path orientations
    m_abstractionUtils.calcPathOrientation(lanePath);

    return lanePath;
}

crp_msgs::msg::PathWithTrafficRules crp::cil::ScenarioAbstraction::extractPossiblePaths(
    const lanelet::ConstLanelet&                         startLanelet,
    const tier4_planning_msgs::msg::PathPointWithLaneId& prevPoint,
    std::unordered_set<int64_t>&                         visitedLaneletsById,
    crp_msgs::msg::PathWithTrafficRulesArray&            outPaths,
    const float                                          currentCompletePathLength=0,
    const uint16_t                                       currentPointIdx=0
)
{
    if (visitedLaneletsById.find(startLanelet.id()) != visitedLaneletsById.end())
    {
        // return if lanelet has already been visited
        return crp_msgs::msg::PathWithTrafficRules();
    }

    crp_msgs::msg::PathWithTrafficRules currentPath = extractLaneletToCompleteLength(startLanelet, currentCompletePathLength, prevPoint, currentPointIdx);
    visitedLaneletsById.insert(startLanelet.id());
    
    if (currentPath.path_length >= m_localPathLength)
    {
        // return if current path is long enough
        // this could only happen with the first ever lanelet, because path extracton takes the whole path length into account not just the current lanelet
        m_abstractionUtils.calcPathOrientation(currentPath);
        outPaths.paths.push_back(currentPath);
        return crp_msgs::msg::PathWithTrafficRules();
    }

    if (currentPath.path_length + currentCompletePathLength >= m_localPathLength)
    {
        // return if the complete path is long enough
        return currentPath;
    }

    lanelet::routing::LaneletPaths paths = m_routingGraph->possiblePaths(startLanelet, 1, 0, false);

    if (paths.empty())
    {
        // return if no possible paths
        if (currentCompletePathLength == 0)
        {
            // length not reached and no possible paths after FIRST lanelet
            outPaths.paths.push_back(currentPath);
            return crp_msgs::msg::PathWithTrafficRules();
        }
        // length not reached but no possible paths after lanelet
        return currentPath;
    }

    for (lanelet::routing::LaneletPath selectedPath : paths)
    {
        // add points to path from consecutive possible lanelets

        crp_msgs::msg::PathWithTrafficRules selectedPathExtracted = extractPossiblePaths(selectedPath.front(), currentPath.path.points.back(), visitedLaneletsById, outPaths, currentCompletePathLength+currentPath.path_length, 0);
        if (selectedPathExtracted.path_length > 0)
        {
            // return if the complete path is long enough
            currentPath.path.points.insert(currentPath.path.points.end(), selectedPathExtracted.path.points.begin(), selectedPathExtracted.path.points.end());
            currentPath.path_length += selectedPathExtracted.path_length;
        }
        
        outPaths.paths.push_back(currentPath);
    }
    
    return crp_msgs::msg::PathWithTrafficRules();
}

void crp::cil::ScenarioAbstraction::publishCallback()
{
    if (!m_isMapLoaded)
        return;
    
    this->get_parameter("local_path_length", m_localPathLength);

    lanelet::BasicPoint2d currentPoint(m_egoPoseMapFrame.pose.pose.position.x, m_egoPoseMapFrame.pose.pose.position.y);

    // get nearest lanelet to ego
    std::vector<std::pair<double, lanelet::Lanelet>> actuallyNearestLanelets = 
        lanelet::geometry::findNearest(m_laneletMap->laneletLayer, currentPoint, 1);
    lanelet::Lanelet egoLanelet = actuallyNearestLanelets.front().second;

    lanelet::traffic_rules::SpeedLimitInformation limit = m_trafficRules->speedLimit(egoLanelet);
    uint16_t nearestPointIdx = m_abstractionUtils.getGPSNNPointIdx(currentPoint, egoLanelet); // nearest point to ego on lane

    lanelet::ConstLineString2d egoCenterline = egoLanelet.centerline2d();
    tier4_planning_msgs::msg::PathPointWithLaneId prevPoint = m_abstractionUtils.laneletPtToPathPoint(egoCenterline[nearestPointIdx],0);
    
    std::unordered_set<int64_t> visitedLaneletsById;
    
    crp_msgs::msg::PathWithTrafficRulesArray outPaths;
    outPaths.header.stamp = this->now();

    extractPossiblePaths(egoLanelet, prevPoint, visitedLaneletsById, outPaths, 0, nearestPointIdx);

    // std::vector<lanelet::TrafficSign::Ptr> trafficSigns = egoLanelet.regulatoryElementsAs<lanelet::TrafficSign>();
    // lanelet::LineString3d stopLine = trafficSigns[0]->refLines()[0];

    m_pub_possiblePaths_->publish(outPaths);
}


void crp::cil::ScenarioAbstraction::staticMapFromFileCallback(const autoware_map_msgs::msg::LaneletMapBin::SharedPtr msg)
{
    RCLCPP_INFO(this->get_logger(), "Loading map from topic");
    lanelet::utils::conversion::fromBinMsg(*msg, m_laneletMap);
    m_trafficRules = lanelet::traffic_rules::TrafficRulesFactory::create(lanelet::Locations::Germany, lanelet::Participants::Vehicle);
    m_routingGraph = lanelet::routing::RoutingGraph::build(*m_laneletMap, *m_trafficRules);
    m_mapFrameId = msg->header.frame_id;

    m_isMapLoaded = true;
    RCLCPP_INFO(this->get_logger(), "Map loaded");
}


void crp::cil::ScenarioAbstraction::poseCallback(const geometry_msgs::msg::PoseWithCovarianceStamped::SharedPtr msg)
{
    if (!m_isMapLoaded)
        return;
    if (!m_isGpsTransformSet)
    {
        tf2_ros::Buffer tfBuffer(this->get_clock());
        tf2_ros::TransformListener tfListener(tfBuffer);
        m_gps2mapTransform = tfBuffer.lookupTransform(m_mapFrameId, msg->header.frame_id, rclcpp::Time(0), rclcpp::Duration(5, 0));
        m_isGpsTransformSet = true;
    }
    // transform ego pose to map frame
    tf2::doTransform(*msg, m_egoPoseMapFrame, m_gps2mapTransform);
}


int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::cil::ScenarioAbstraction>());
    rclcpp::shutdown();
    return 0;
}
