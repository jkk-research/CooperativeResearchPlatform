#include "plan_behavior_planning/behaviorPlanner.hpp"


crp::apl::BehaviorPlanner::BehaviorPlanner() : Node("behavior_planner")
{
    m_sub_route    = this->create_subscription<autoware_planning_msgs::msg::LaneletRoute>(
        "plan/route", 10, std::bind(&BehaviorPlanner::routeCallback, this, std::placeholders::_1));
    m_sub_scenario = this->create_subscription<crp_msgs::msg::Scenario>(
        "scenario", 10, std::bind(&BehaviorPlanner::scenarioCallback, this, std::placeholders::_1));
    m_sub_ego      = this->create_subscription<crp_msgs::msg::Ego>(
        "ego", 10, std::bind(&BehaviorPlanner::egoCallback, this, std::placeholders::_1));

    m_sub_behavior_ = this->create_subscription<crp_msgs::msg::Behavior>(
        "/ui/behavior", 10,
        std::bind(&BehaviorPlanner::behaviorCallback, this, std::placeholders::_1));

    m_pub_strategy = this->create_publisher<tier4_planning_msgs::msg::Scenario>("plan/strategy", 10);
    m_pub_target_space = this->create_publisher<crp_msgs::msg::TargetSpace>("plan/target_space", 10);

    this->declare_parameter("/behavior_planner/current_scenario", "laneFollowWithSpeedAdjust");

    timer_ = this->create_wall_timer(std::chrono::milliseconds(33), std::bind(&BehaviorPlanner::loop, this));  
}


void crp::apl::BehaviorPlanner::routeCallback(const autoware_planning_msgs::msg::LaneletRoute::SharedPtr msg)
{
    // TODO
    return;
}


void crp::apl::BehaviorPlanner::scenarioCallback(const crp_msgs::msg::Scenario::SharedPtr msg)
{
    crp_msgs::msg::TargetSpace targetSpaceMsg;

    if (msg->paths.size() > 0){
        targetSpaceMsg.path = msg->paths[0];
    }


    for (tier4_planning_msgs::msg::PathPointWithLaneId & pathPoint : targetSpaceMsg.path.path.points)
    {
        pathPoint.point.longitudinal_velocity_mps = std::min(m_maximum_speed, pathPoint.point.longitudinal_velocity_mps);
    }

    targetSpaceMsg.free_space = msg->free_space;
    
    m_pub_target_space->publish(targetSpaceMsg);

}


void crp::apl::BehaviorPlanner::egoCallback(const crp_msgs::msg::Ego::SharedPtr msg)
{
    // TODO
    return;
}


void crp::apl::BehaviorPlanner::behaviorCallback(const crp_msgs::msg::Behavior::SharedPtr msg)
{
    m_maximum_speed = msg->target_speed.data;
}


void crp::apl::BehaviorPlanner::loop()
{
    // possible Scenrios: "off", "laneFollowWithSpeedAdjust", "laneFollow", "speedAdjust"

    std::string current_scenario_msg = this->get_parameter("/behavior_planner/current_scenario").as_string();

    tier4_planning_msgs::msg::Scenario scenario_msg;
    scenario_msg.current_scenario = current_scenario_msg;

    m_pub_strategy->publish(scenario_msg);
    
    return;
}


int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::apl::BehaviorPlanner>());
    rclcpp::shutdown();
    return 0;
}