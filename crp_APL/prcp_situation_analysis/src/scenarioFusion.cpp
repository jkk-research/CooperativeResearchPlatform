#include "prcp_situation_analysis/scenarioFusion.hpp"


crp::apl::ScenarioFusion::ScenarioFusion() : Node("scenario_fusion")
{
    m_sub_localMovingObjects_ = this->create_subscription<autoware_perception_msgs::msg::PredictedObjects>(
        "/cai/local_moving_objects", 10,
        std::bind(&ScenarioFusion::localMovingObjectsCallback, this, std::placeholders::_1));
    m_sub_localObstacles_ = this->create_subscription<autoware_perception_msgs::msg::PredictedObjects>(
        "/cai/local_obstacles", 10,
        std::bind(&ScenarioFusion::localObstaclesCallback, this, std::placeholders::_1));
    m_sub_possiblePaths_ = this->create_subscription<crp_msgs::msg::PathWithTrafficRulesArray>(
        "/cai/local_lane/possible_paths", 10,
        std::bind(&ScenarioFusion::localPathCallback, this, std::placeholders::_1));
    m_sub_localDrivableSurface_ = this->create_subscription<nav_msgs::msg::OccupancyGrid>(
        "/cai/local_drivable_surface", 10,
        std::bind(&ScenarioFusion::localDrivableSurfaceCallback, this, std::placeholders::_1));

    m_pub_scenario_ = this->create_publisher<crp_msgs::msg::Scenario>("/scenario", 10);

    m_publishTimer_ = this->create_wall_timer(std::chrono::milliseconds(50), std::bind(&ScenarioFusion::publishCallback, this));

    RCLCPP_INFO(this->get_logger(), "scenario_fusion has been started");
}


void crp::apl::ScenarioFusion::publishCallback()
{
    m_pub_scenario_->publish(m_scenario);
}


void crp::apl::ScenarioFusion::localMovingObjectsCallback(const autoware_perception_msgs::msg::PredictedObjects::SharedPtr msg)
{
    m_scenario.header = msg->header;
    m_scenario.local_moving_objects = *msg;
}


void crp::apl::ScenarioFusion::localObstaclesCallback(const autoware_perception_msgs::msg::PredictedObjects::SharedPtr msg)
{
    m_scenario.header = msg->header;
    m_scenario.local_obstacles = *msg;
}


void crp::apl::ScenarioFusion::localPathCallback(const crp_msgs::msg::PathWithTrafficRulesArray::SharedPtr msg)
{
    m_scenario.header = msg->header;
    m_scenario.paths  = msg->paths;
}


void crp::apl::ScenarioFusion::localDrivableSurfaceCallback(const nav_msgs::msg::OccupancyGrid::SharedPtr msg)
{
    m_scenario.header = msg->header;
    m_scenario.free_space = *msg;
}


int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::apl::ScenarioFusion>());
    rclcpp::shutdown();
    return 0;
}
