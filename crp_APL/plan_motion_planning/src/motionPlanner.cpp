#include "plan_motion_planning/motionPlanner.hpp"


crp::apl::MotionPlanner::MotionPlanner() : Node("motion_planner")
{
    m_sub_strategy_   = this->create_subscription<tier4_planning_msgs::msg::Scenario>(
        "plan/strategy", 10, std::bind(&MotionPlanner::scenarioCallback, this, std::placeholders::_1));

    m_pub_trajectory_ = this->create_publisher<autoware_planning_msgs::msg::Trajectory>("plan/trajectory", 10);
}


void crp::apl::MotionPlanner::scenarioCallback(const tier4_planning_msgs::msg::Scenario::SharedPtr msg)
{
    // TODO
    return;
}


int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::apl::MotionPlanner>());
    rclcpp::shutdown();
    return 0;
}