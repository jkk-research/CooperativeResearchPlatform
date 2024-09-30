#include "plan_motion_planning/motionHandler.hpp"


crp::apl::MotionHandler::MotionHandler() : Node("motion_handler")
{
    this->declare_parameter<std::vector<std::string>>("planners", std::vector<std::string>{});

    this->get_parameter("planners", m_planners);

    m_sub_plan_latLaneFollow_ = this->create_subscription<autoware_planning_msgs::msg::Trajectory>(
        "plan/lat_lane_follow/trajectory", 10, std::bind(&MotionHandler::planLatLaneFollowCallback, this, std::placeholders::_1));
    m_sub_plan_lonEmergency_ = this->create_subscription<autoware_planning_msgs::msg::Trajectory>(
        "plan/lon_emergency/trajectory", 10, std::bind(&MotionHandler::planLonEmergencyCallback, this, std::placeholders::_1));
    m_sub_plan_lonIntelligentSpeedAdjust_ = this->create_subscription<autoware_planning_msgs::msg::Trajectory>(
        "plan/lon_intelligent_speed_adjust/trajectory", 10, std::bind(&MotionHandler::planLonIntelligentSpeedAdjustCallback, this, std::placeholders::_1));

    m_pub_trajectory_ = this->create_publisher<autoware_planning_msgs::msg::Trajectory>("plan/trajectory", 10);

}





void crp::apl::MotionHandler::planLatLaneFollowCallback(const autoware_planning_msgs::msg::Trajectory::SharedPtr msg)
{
    // TODO
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


int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::apl::MotionHandler>());
    rclcpp::shutdown();
    return 0;
}