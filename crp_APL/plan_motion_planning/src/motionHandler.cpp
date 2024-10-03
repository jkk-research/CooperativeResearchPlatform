#include "plan_motion_planning/motionHandler.hpp"


crp::apl::MotionHandler::MotionHandler() : Node("motion_handler")
{
    m_sub_strategy_   = this->create_subscription<tier4_planning_msgs::msg::Scenario>(
        "plan/strategy", 10, std::bind(&MotionHandler::scenarioCallback, this, std::placeholders::_1));
    m_sub_plan_latLaneFollow_ = this->create_subscription<autoware_planning_msgs::msg::Trajectory>(
        "plan/lat_lane_follow/trajectory", 10, std::bind(&MotionHandler::planLatLaneFollowCallback, this, std::placeholders::_1));
    m_sub_plan_lonEmergency_ = this->create_subscription<autoware_planning_msgs::msg::Trajectory>(
        "plan/lon_emergency/trajectory", 10, std::bind(&MotionHandler::planLonEmergencyCallback, this, std::placeholders::_1));
    m_sub_plan_lonIntelligentSpeedAdjust_ = this->create_subscription<autoware_planning_msgs::msg::Trajectory>(
        "plan/lon_intelligent_speed_adjust/trajectory", 10, std::bind(&MotionHandler::planLonIntelligentSpeedAdjustCallback, this, std::placeholders::_1));

    m_pub_trajectory_ = this->create_publisher<autoware_planning_msgs::msg::Trajectory>("plan/trajectory", 10);

    m_timer_ = this->create_wall_timer(std::chrono::milliseconds(20), std::bind(&MotionHandler::run, this));
}

void crp::apl::MotionHandler::scenarioCallback(const tier4_planning_msgs::msg::Scenario::SharedPtr msg)
{
    if(msg->current_scenario == "laneFollowWithSpeedAdjust"){
        m_currentStrategy = "laneFollowWithSpeedAdjust";
    }
    else{
        m_currentStrategy = "off";
    }
    return;
    // TODO: in some cases there are planners who provide both longitudinal and lateral planning, as well. It means that incoming topic will contain both plans. A concept of how to handle that with strategy interface shall be found out.
}

void crp::apl::MotionHandler::planLatLaneFollowCallback(const autoware_planning_msgs::msg::Trajectory::SharedPtr msg)
{
    // waypoints are coming from the lane follow node - in case of strategy incl. lane follow lateral information should be handed over
    // TODO: if the velocity would come from the longitudinal planner, first an interpolation of the values should happen - it means, that each waypoint should contain both the lateral and the longitudinal targets at the same time
    if (m_currentStrategy == "laneFollowWithSpeedAdjust"){
        m_outputTrajectory = msg;
    }
    // else do nothing
    return;
}


void crp::apl::MotionHandler::planLonEmergencyCallback(const autoware_planning_msgs::msg::Trajectory::SharedPtr msg)
{
    // TODO
    return;
}


void crp::apl::MotionHandler::planLonIntelligentSpeedAdjustCallback(const autoware_planning_msgs::msg::Trajectory::SharedPtr msg)
{
    // TODO
    return;
}

void crp::apl::MotionHandler::run()
{
    m_pub_trajectory_->publish(m_outputTrajectory);
}


int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::apl::MotionHandler>());
    rclcpp::shutdown();
    return 0;
}