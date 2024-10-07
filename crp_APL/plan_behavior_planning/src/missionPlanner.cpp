#include "plan_behavior_planning/missionPlanner.hpp"


crp::apl::MissionPlanner::MissionPlanner() : Node("mission_planner")
{
    m_sub_world = this->create_subscription<crp_msgs::msg::World>(
        "world", 10, std::bind(&MissionPlanner::worldCallback, this, std::placeholders::_1));

    m_pub_route = this->create_publisher<autoware_planning_msgs::msg::LaneletRoute>("plan/route", 10);
}


void crp::apl::MissionPlanner::worldCallback(const crp_msgs::msg::World::SharedPtr msg)
{
    // TODO
    return;
}


int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::apl::MissionPlanner>());
    rclcpp::shutdown();
    return 0;
}