#include "behavior_planning/behaviorPlanner.hpp"


BehaviorPlanner::BehaviorPlanner() : Node("behavior_planner")
{
    m_sub_route    = this->create_subscription<autoware_planning_msgs::msg::LaneletRoute>("plan/route", 10, std::bind(&BehaviorPlanner::routeCallback, this, std::placeholders::_1));
    m_sub_scenario = this->create_subscription<crp_msgs::msg::Scenario>("scenario", 10, std::bind(&BehaviorPlanner::scenarioCallback, this, std::placeholders::_1));
    m_sub_ego      = this->create_subscription<crp_msgs::msg::Ego>("ego", 10, std::bind(&BehaviorPlanner::egoCallback, this, std::placeholders::_1));

    m_pub_strategy     = this->create_publisher<tier4_planning_msgs::msg::Scenario>("plan/strategy", 10);
    m_pub_target_space = this->create_publisher<crp_msgs::msg::TargetSpace>("plan/target_space", 10);
}


void BehaviorPlanner::routeCallback(const autoware_planning_msgs::msg::LaneletRoute::SharedPtr msg)
{
    return;
}


void BehaviorPlanner::scenarioCallback(const crp_msgs::msg::Scenario::SharedPtr msg)
{
    return;
}


void BehaviorPlanner::egoCallback(const crp_msgs::msg::Ego::SharedPtr msg)
{
    return;
}


int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<BehaviorPlanner>());
    rclcpp::shutdown();
    return 0;
}