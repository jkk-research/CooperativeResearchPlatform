#include "prcp_sensor_abstraction/scenarioAbstraction.hpp"


crp::cil::ScenarioAbstraction::ScenarioAbstraction() : Node("scenario_abstraction")
{
    this->declare_parameter<float>("local_path_length", 75.0);

    m_laneletMap = std::make_shared<lanelet::LaneletMap>();

    m_sub_staticMapFromFile_ = this->create_subscription<autoware_map_msgs::msg::LaneletMapBin>(
        "map/global_static_map_from_file/lanelet2_map", rclcpp::QoS{1}.transient_local(), std::bind(&ScenarioAbstraction::staticMapFromFileCallback, this, std::placeholders::_1));

    m_sub_pose_ = this->create_subscription<geometry_msgs::msg::PoseWithCovarianceStamped>(
        "/sensing/gnss/pose_with_covariance", 10, std::bind(&ScenarioAbstraction::poseCallback, this, std::placeholders::_1));

    m_pub_movingObjects_   = this->create_publisher<autoware_perception_msgs::msg::PredictedObjects>("cai/local_moving_objects", 10);
    m_pub_obstacles_       = this->create_publisher<autoware_perception_msgs::msg::PredictedObjects>("cai/local_obstacles", 10);
    m_pub_lanePath_        = this->create_publisher<tier4_planning_msgs::msg::PathWithLaneId>("cai/local_lane/path", 10);
    m_pub_drivableSurface_ = this->create_publisher<nav_msgs::msg::OccupancyGrid>("cai/local_drivable_surface", 10);

    m_publishTimer_ = this->create_wall_timer(
        std::chrono::milliseconds(50), std::bind(&ScenarioAbstraction::publishCallback, this));
}


float crp::cil::ScenarioAbstraction::distanceBetweenPoints(const lanelet::BasicPoint2d a, const lanelet::ConstPoint2d b)
{
    return sqrt(pow(b.x()-a.x(), 2)+pow(b.y()-a.y(), 2));
}


uint16_t crp::cil::ScenarioAbstraction::getGPSNNPointIdx(const lanelet::BasicPoint2d & currentPos, const lanelet::ConstLanelet & lane)
{
    lanelet::ConstLineString2d centerline = lane.centerline2d();
    double minDist = distanceBetweenPoints(currentPos, centerline.front());
    uint16_t minDistIdx = 0;
    for (uint16_t i = 1; i < centerline.size(); i++)
    {
        double currentDist = distanceBetweenPoints(currentPos, centerline[i]);
        if (currentDist < minDist)
        {
            minDist = currentDist;
            minDistIdx = i;
        }
    }
    return minDistIdx;
}

autoware_planning_msgs::msg::PathPoint crp::cil::ScenarioAbstraction::laneletPtToPathPoint(const lanelet::ConstPoint2d & pt)
{
    autoware_planning_msgs::msg::PathPoint pathPoint;
    pathPoint.pose.position.x = pt.x();
    pathPoint.pose.position.y = pt.y();
    return pathPoint;
}


void crp::cil::ScenarioAbstraction::publishCallback()
{
    if (!m_isMapLoaded)
        return;

    float localPathLength;
    this->get_parameter("local_path_length", localPathLength);

    lanelet::BasicPoint2d currentPoint(m_pose.pose.pose.position.x, m_pose.pose.pose.position.y);

    // get nearest lanelet to ego
    std::vector<std::pair<double, lanelet::Lanelet>> actuallyNearestLanelets = 
        lanelet::geometry::findNearest(m_laneletMap->laneletLayer, currentPoint, 1);
    lanelet::Lanelet egoLanelet = actuallyNearestLanelets.front().second;

    uint16_t nearestPointIdx = getGPSNNPointIdx(currentPoint, egoLanelet); // nearest point to ego on lane

    tier4_planning_msgs::msg::PathWithLaneId outPath;
    outPath.header.stamp = this->now();
    outPath.header.frame_id = "map";

    float currentPathLength = 0;
    lanelet::ConstPoint2d prevPoint = currentCenterline[nearestPointIdx];
    lanelet::ConstLineString2d currentCenterline = egoLanelet.centerline2d();
    outPath.points.push_back(laneletPtToPathPoint(egoLanelet.centerline2d()[nearestPointIdx])); // add ego point
    uint16_t currentPointIdx = nearestPointIdx + 1;
    while (currentPathLength < localPathLength && currentPointIdx < currentCenterline.size())
    {
        // add points to path from ego lanelet
        lanelet::ConstPoint2d currentPoint = currentCenterline[currentPointIdx];
        currentPathLength += distanceBetweenPoints(prevPoint, currentPoint);
        outPath.points.push_back(laneletPtToPathPoint(currentPoint));
        prevPoint = currentPoint;
        currentPointIdx++;
    }

    if (currentPathLength < localPathLength)
    {
        // add points to path from consecutive lanelets
        lanelet::Lanelet currentLanelet = egoLanelet;
        while (currentPathLength < localPathLength)
        {
            // get next lanelet (first from possible paths)
            lanelet::routing::LaneletPaths paths = m_routingGraph->possiblePaths(currentLanelet, 1, 0, false);
            lanelet::routing::LaneletPath selectedPath = paths.front();
            if (selectedPath.size() < 2)
                break;
            lanelet::ConstLanelet currentLane = selectedPath.at(1); // next lanelet
            

            // TODO:
            // get centerline
            // add points to path
            //      add pts
            //      update currentPathLength
            //      update prevPoint
            // update currentLanelet
            
        }

        // TODO: 
        // transform points to local frame !
        // if length: nice, publish
        /// else: publish but warning
    }

    

}


void crp::cil::ScenarioAbstraction::staticMapFromFileCallback(const autoware_map_msgs::msg::LaneletMapBin::SharedPtr msg)
{
    RCLCPP_INFO(this->get_logger(), "Loading map from file");
    lanelet::utils::conversion::fromBinMsg(*msg, m_laneletMap);
    m_trafficRules = lanelet::traffic_rules::TrafficRulesFactory::create(lanelet::Locations::Germany, lanelet::Participants::Vehicle);
    m_routingGraph = lanelet::routing::RoutingGraph::build(*m_laneletMap, *m_trafficRules);
    m_isMapLoaded = true;
    RCLCPP_INFO(this->get_logger(), "Map loaded");
}


void crp::cil::ScenarioAbstraction::poseCallback(const geometry_msgs::msg::PoseWithCovarianceStamped::SharedPtr msg)
{
    m_pose = *msg;
}


int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::cil::ScenarioAbstraction>());
    rclcpp::shutdown();
    return 0;
}
