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
        std::chrono::milliseconds(33), std::bind(&ScenarioAbstraction::publishCallback, this));
    
    RCLCPP_INFO(this->get_logger(), "scenario_abstraction has been started");
}

crp_msgs::msg::PathWithTrafficRules crp::cil::ScenarioAbstraction::extractLaneletToCompleteLength(
    const lanelet::ConstLanelet&                   lanelet,
    const float&                                   currentCompletePathLength,
    tier4_planning_msgs::msg::PathPointWithLaneId& prevPointGlobal,
    uint16_t                                       currentPointIdx=0
)
{
    crp_msgs::msg::PathWithTrafficRules lanePath;

    // extract path
    float laneletSpeedLimit = m_trafficRules->speedLimit(lanelet).speedLimit.value();
    lanePath.path_length = 0.0f;
    lanelet::ConstLineString2d currentCenterline = lanelet.centerline2d();
    tier4_planning_msgs::msg::PathPointWithLaneId currentPrevPoint = prevPointGlobal;
    while (lanePath.path_length+currentCompletePathLength < m_localPathLength && currentPointIdx < currentCenterline.size())
    {
        // get current point
        tier4_planning_msgs::msg::PathPointWithLaneId currentPoint = m_abstractionUtils.laneletPtToPathPoint(currentCenterline[currentPointIdx],laneletSpeedLimit);
        
        // add point to lanelet
        lanePath.path_length += m_abstractionUtils.distanceBetweenPoints(currentPrevPoint, currentPoint);
        lanePath.path.points.push_back(
            m_abstractionUtils.transformToLocal(currentPoint, m_egoPoseMapFrame)
        );
        currentPrevPoint = currentPoint;
        currentPointIdx++;
    }

    // extract stop signs
    std::vector<std::shared_ptr<const lanelet::TrafficSign>> trafficSigns = lanelet.regulatoryElementsAs<lanelet::TrafficSign>();
    for (std::shared_ptr<const lanelet::TrafficSign> sign : trafficSigns)
    {
        if (sign->type() == "stop_sign")
        {
            if (sign->refLines().empty())
                continue;

            // add stop lines
            for (lanelet::ConstLineString3d& stopLine : sign->refLines())
            {
                crp_msgs::msg::StopPose stopPose;

                // average out the stop line points
                geometry_msgs::msg::Pose stopPoint;
                for (lanelet::BasicPoint3d stopLinePt : stopLine)
                {
                    stopPose.pose.position.x += stopLinePt.x();
                    stopPose.pose.position.y += stopLinePt.y();
                }
                stopPose.pose.position.x /= stopLine.size();
                stopPose.pose.position.y /= stopLine.size();
                
                stopPose.type = crp_msgs::msg::StopPose::STOP_SIGN;
                stopPose.confidence = 1.0f;

                lanePath.traffic_rules.stop_poses.push_back(stopPose);
            }
        }
    }

    // extract crosswalks
    lanelet::ConstLaneletOrAreas crosswalks = m_routingGraph->conflicting(lanelet);
    for (lanelet::ConstLaneletOrArea crosswalk : crosswalks)
    {
        if (crosswalk.isLanelet() && crosswalk.attributes().at(lanelet::AttributeName::Subtype) == "crosswalk")
        {
            // if crosswalk is a lanelet get the middle of the centerline
            crp_msgs::msg::StopPose stopPose;
            stopPose.type = crp_msgs::msg::StopPose::CROSSWALK;
            stopPose.confidence = 1.0f;

            if (crosswalk.isLanelet())
            {
                lanelet::ConstLineString2d crosswalkLine = crosswalk.lanelet().value().centerline2d();
                for (lanelet::BasicPoint2d crosswalkPt : crosswalkLine)
                {
                    stopPose.pose.position.x += crosswalkPt.x();
                    stopPose.pose.position.y += crosswalkPt.y();
                }
                stopPose.pose.position.x /= crosswalkLine.size();
                stopPose.pose.position.y /= crosswalkLine.size();
            }
            else if (crosswalk.isArea())
            {
                lanelet::CompoundPolygon3d crosswalkPoly = crosswalk.boundingPolygon();
                float minX = crosswalkPoly.front().x();
                float minY = crosswalkPoly.front().y();
                float maxX = crosswalkPoly.front().x();
                float maxY = crosswalkPoly.front().y();
                for (lanelet::BasicPoint3d crosswalkPt : crosswalkPoly)
                {
                    if (crosswalkPt.x() < minX)
                        minX = crosswalkPt.x();
                    if (crosswalkPt.y() < minY)
                        minY = crosswalkPt.y();
                    if (crosswalkPt.x() > maxX)
                        maxX = crosswalkPt.x();
                    if (crosswalkPt.y() > maxY)
                        maxY = crosswalkPt.y();
                }
                stopPose.pose.position.x = (minX + maxX) / 2.0;
                stopPose.pose.position.y = (minY + maxY) / 2.0;
            }

            lanePath.traffic_rules.stop_poses.push_back(stopPose);
        }
    }

    // set prevPoint for next lanelet
    prevPointGlobal = currentPrevPoint;

    return lanePath;
}

void crp::cil::ScenarioAbstraction::extractPossiblePaths(
    const lanelet::ConstLanelet&                   startLanelet,
    tier4_planning_msgs::msg::PathPointWithLaneId& prevPointGlobal,
    crp_msgs::msg::PathWithTrafficRulesArray&      outPaths,
    const float                                    currentCompletePathLength=0,
    const uint16_t                                 currentPointIdx=0
)
{
    crp_msgs::msg::PathWithTrafficRules currentPath = extractLaneletToCompleteLength(startLanelet, currentCompletePathLength, prevPointGlobal, currentPointIdx);

    if (currentPath.path_length >= m_localPathLength)
    {
        // return if current path is long enough
        // this could only happen with the first ever lanelet, because path extracton takes the whole path length into account not just the current lanelet
        outPaths.paths.push_back(currentPath);
        return;
    }

    if (currentPath.path_length + currentCompletePathLength >= m_localPathLength)
    {
        // return if the complete path is long enough
        outPaths.paths.push_back(currentPath);
        return;
    }

    // query next possible lanelets
    lanelet::routing::LaneletPaths paths = m_routingGraph->possiblePaths(startLanelet, 1, 0, true);

    if (paths.empty())
    {
        // length not reached but no possible paths after lanelet
        outPaths.paths.push_back(currentPath);
        return;
    }

    for (lanelet::routing::LaneletPath selectedPath : paths)
    {
        // add points to path from consecutive possible lanelets
        crp_msgs::msg::PathWithTrafficRulesArray childPaths;
        extractPossiblePaths(selectedPath[1], prevPointGlobal, childPaths, currentCompletePathLength+currentPath.path_length, 0);

        for (crp_msgs::msg::PathWithTrafficRules childPath : childPaths.paths)
        {
            // add child paths to current path
            crp_msgs::msg::PathWithTrafficRules outPath;
            outPath.path.points = currentPath.path.points;
            outPath.path.points.insert(outPath.path.points.end(), childPath.path.points.begin(), childPath.path.points.end());
            outPath.path_length = currentPath.path_length + childPath.path_length;
            outPath.traffic_rules.stop_poses = currentPath.traffic_rules.stop_poses;
            outPath.traffic_rules.stop_poses.insert(outPath.traffic_rules.stop_poses.end(), childPath.traffic_rules.stop_poses.begin(), childPath.traffic_rules.stop_poses.end());

            outPaths.paths.push_back(outPath);
        }
    }
    
    return;
}

void crp::cil::ScenarioAbstraction::publishCallback()
{
    if (!m_isMapLoaded || !m_isGpsTransformSet)
        return;
    
    this->get_parameter("local_path_length", m_localPathLength);

    lanelet::BasicPoint2d currentPoint(m_egoPoseMapFrame.pose.pose.position.x, m_egoPoseMapFrame.pose.pose.position.y);

    // get nearest lanelet to ego
    std::vector<std::pair<double, lanelet::Lanelet>> actuallyNearestLanelets = 
        lanelet::geometry::findNearest(m_laneletMap->laneletLayer, currentPoint, 1);
    lanelet::Lanelet egoLanelet = actuallyNearestLanelets.front().second;

    uint16_t nearestPointIdx = m_abstractionUtils.getGPSNNPointIdx(currentPoint, egoLanelet); // nearest point to ego on lane

    lanelet::ConstLineString2d egoCenterline = egoLanelet.centerline2d();
    tier4_planning_msgs::msg::PathPointWithLaneId prevPoint = m_abstractionUtils.laneletPtToPathPoint(egoCenterline[nearestPointIdx],0);
    
    crp_msgs::msg::PathWithTrafficRulesArray outPaths;
    outPaths.header.stamp = this->now();

    extractPossiblePaths(egoLanelet, prevPoint, outPaths, 0, nearestPointIdx);

    // TODO: adjust stopsign and crosswalk stop poses

    for (crp_msgs::msg::PathWithTrafficRules& path : outPaths.paths)
    {
        m_abstractionUtils.calcPathOrientation(path);
    }

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
        if (m_mapFrameId != msg->header.frame_id)
        {
            tf2_ros::Buffer tfBuffer(this->get_clock());
            tf2_ros::TransformListener tfListener(tfBuffer);
            m_gps2mapTransform = tfBuffer.lookupTransform(m_mapFrameId, msg->header.frame_id, rclcpp::Time(0), rclcpp::Duration(5, 0));
        }
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
