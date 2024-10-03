#ifndef CRP_APL_MOTION_HANDLER_PLANMOTIONPLANNING_HPP
#define CRP_APL_MOTION_HANDLER_PLANMOTIONPLANNING_HPP

#include <rclcpp/rclcpp.hpp>
#include <autoware_planning_msgs/msg/trajectory.hpp>
#include <tier4_planning_msgs/msg/scenario.hpp>


namespace crp
{
namespace apl
{

class MotionHandler : public rclcpp::Node
{
public:
    MotionHandler();

private:
    void scenarioCallback(const tier4_planning_msgs::msg::Scenario::SharedPtr msg);
    void planLatLaneFollowCallback(const autoware_planning_msgs::msg::Trajectory::SharedPtr msg);
    void planLonEmergencyCallback(const autoware_planning_msgs::msg::Trajectory::SharedPtr msg);
    void planLonIntelligentSpeedAdjustCallback(const autoware_planning_msgs::msg::Trajectory::SharedPtr msg);

    rclcpp::Subscription<tier4_planning_msgs::msg::Scenario>::SharedPtr m_sub_strategy_;
    rclcpp::Subscription<autoware_planning_msgs::msg::Trajectory>::SharedPtr m_sub_plan_latLaneFollow_;
    rclcpp::Subscription<autoware_planning_msgs::msg::Trajectory>::SharedPtr m_sub_plan_lonEmergency_;
    rclcpp::Subscription<autoware_planning_msgs::msg::Trajectory>::SharedPtr m_sub_plan_lonIntelligentSpeedAdjust_;
    
    rclcpp::Publisher<autoware_planning_msgs::msg::Trajectory>::SharedPtr m_pub_trajectory_;

    std::vector<std::string> m_planners;
};

} // namespace apl
} // namespace crp
#endif // CRP_APL_MOTION_HANDLER_PLANMOTIONPLANNING_HPP
