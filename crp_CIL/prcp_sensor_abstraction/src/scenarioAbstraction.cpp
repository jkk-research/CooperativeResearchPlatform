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


void crp::cil::ScenarioAbstraction::publishCallback()
{
    if (!m_isMapLoaded)
        return;

    float localPathLength;
    this->get_parameter("local_path_length", localPathLength);
    RCLCPP_INFO(this->get_logger(), "Local path length: %f", localPathLength);

    lanelet::traffic_rules::TrafficRulesPtr trafficRules = 
        lanelet::traffic_rules::TrafficRulesFactory::create(lanelet::Locations::Germany, lanelet::Participants::Vehicle);
    lanelet::routing::RoutingGraphUPtr routingGraph = lanelet::routing::RoutingGraph::build(*m_laneletMap, *trafficRules);

    lanelet::BasicPoint2d currentPoint(435.02, -1475.87); //! TEMP

    std::vector<std::pair<double, lanelet::Lanelet>> actuallyNearestLanelets = 
        lanelet::geometry::findNearest(m_laneletMap->laneletLayer, currentPoint, 1);
    lanelet::Lanelet currentLanelet = actuallyNearestLanelets.front().second;

    lanelet::routing::LaneletPaths paths = routingGraph->possiblePaths(currentLanelet, localPathLength, 0, false);

    // !!!!!!!!!!
    // !! TEMP !!
    // !!!!!!!!!!
    RCLCPP_INFO(this->get_logger(), "Number of paths: %d", paths.size());
    double longestPathLength = 0;
    uint32_t longestPathId = 0;
    for (lanelet::routing::LaneletPath path : paths)
    { 
        double distance = 0;
        for (lanelet::Id laneletId : path.getRemainingLane(currentLanelet).ids())
        {
            lanelet::Lanelet lanelet = m_laneletMap->laneletLayer.get(laneletId);
            lanelet::ConstPoint2d point = lanelet.centerline().back();
            distance += std::sqrt(std::pow(point.x() - currentPoint.x(), 2) + std::pow(point.y() - currentPoint.y(), 2));
            RCLCPP_INFO(this->get_logger(), "Lanelet id: %d, distance: %f", laneletId, distance);
            
        }
        RCLCPP_INFO(this->get_logger(), "Path distance: %f", distance);
        if (distance > longestPathLength)
        {
            longestPathLength = distance;
            longestPathId = path.getRemainingLane(currentLanelet).ids().back();
        }
    }
    RCLCPP_INFO(this->get_logger(), "Distance: %f (id: %d)", longestPathLength, longestPathId);
}


void crp::cil::ScenarioAbstraction::staticMapFromFileCallback(const autoware_map_msgs::msg::LaneletMapBin::SharedPtr msg)
{
    RCLCPP_INFO(this->get_logger(), "Loading map from file");
    lanelet::utils::conversion::fromBinMsg(*msg, m_laneletMap);
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
