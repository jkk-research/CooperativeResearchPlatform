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
    m_pub_targetSpace = this->create_publisher<crp_msgs::msg::TargetSpace>("plan/target_space", 10);
    m_pub_behaviorLimits = this->create_publisher<crp_msgs::msg::BehaviorLimits>("plan/behavior_limits", 10);

    m_timer_ = this->create_wall_timer(std::chrono::milliseconds(33), std::bind(&BehaviorPlanner::loop, this));

    RCLCPP_INFO(this->get_logger(), "behavior_planner has been started");
}


void crp::apl::BehaviorPlanner::routeCallback(const autoware_planning_msgs::msg::LaneletRoute::SharedPtr msg)
{
    // TODO
    (void)msg;
    return;
}


void crp::apl::BehaviorPlanner::scenarioCallback(const crp_msgs::msg::Scenario::SharedPtr msg)
{
    crp_msgs::msg::TargetSpace targetSpaceMsg;

    if (msg->paths.size() > 0){
        targetSpaceMsg.path = msg->paths[0];
    }

    targetSpaceMsg.path.traffic_rules.maximum_speed = m_maximum_speed;

    targetSpaceMsg.free_space = msg->free_space;
    
    m_pub_targetSpace->publish(targetSpaceMsg);

}


void crp::apl::BehaviorPlanner::egoCallback(const crp_msgs::msg::Ego::SharedPtr msg)
{
    // TODO
    m_ego_speed = msg->twist.twist.linear.x;
    (void)msg;
    return;
}


void crp::apl::BehaviorPlanner::behaviorCallback(const crp_msgs::msg::Behavior::SharedPtr msg)
{
    m_maximum_speed = msg->target_speed.data;
    crp_msgs::msg::BehaviorLimits behaviorLimits_msg;

    // mapping the limits according to the incoming behavior styles
    if (m_ego_speed <= 2.0f)
    {
        // start up situation
        if (msg->start_up_acceleration_mode.data==0U){behaviorLimits_msg.ax_max.data = 1.0f;}
        else if(msg->start_up_acceleration_mode.data==1U){behaviorLimits_msg.ax_max.data = 2.5f;}
        else{behaviorLimits_msg.ax_max.data = 5.0f;}
    }
    else
    {
        // acceleration limit assignment according to the speed up mode
        if (msg->speed_up_acceleration_mode.data==0U){behaviorLimits_msg.ax_max.data = 0.8f;}
        else if(msg->speed_up_acceleration_mode.data==1U){behaviorLimits_msg.ax_max.data = 2.0f;}
        else{behaviorLimits_msg.ax_max.data = 4.0f;}
    }

    if (msg->lateral_dynamic_mode.data==0U){behaviorLimits_msg.ay_lim.data = 1.5f;}
        else if(msg->lateral_dynamic_mode.data==1U){behaviorLimits_msg.ay_lim.data = 2.5f;}
        else{behaviorLimits_msg.ay_lim.data = 3.5f;}
    
    behaviorLimits_msg.ax_min.data = -3.0f;

    if (msg->brake_jerk_mode.data==0U){behaviorLimits_msg.jx_max.data = 1.5f;}
        else if(msg->brake_jerk_mode.data==1U){behaviorLimits_msg.jx_max.data = 2.5f;}
        else{behaviorLimits_msg.jx_max.data = 3.5f;}
    
    if (msg->acceleration_jerk_mode.data==0U){behaviorLimits_msg.jx_min.data = -1.5f;}
        else if(msg->acceleration_jerk_mode.data==1U){behaviorLimits_msg.jx_min.data = -2.5f;}
        else{behaviorLimits_msg.jx_min.data = -3.5f;}


    m_pub_behaviorLimits->publish(behaviorLimits_msg);
}


void crp::apl::BehaviorPlanner::loop()
{
    // possible Scenrios: "off", "laneFollowWithDefaultSpeed", "", "laneFollow", "speedAdjust"
    std::string currentScenarioMsg = "laneFollowWithSpeedAdjust";

    tier4_planning_msgs::msg::Scenario scenario_msg;
    scenario_msg.current_scenario = currentScenarioMsg;

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