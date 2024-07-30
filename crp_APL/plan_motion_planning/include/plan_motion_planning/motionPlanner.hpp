#ifndef CRP_APL_MOTION_PLANNER_PLANMOTIONPLANNING_HPP
#define CRP_APL_MOTION_PLANNER_PLANMOTIONPLANNING_HPP


#include <rclcpp/rclcpp.hpp>
#include <tier4_planning_msgs/msg/scenario.hpp>
#include <autoware_planning_msgs/msg/trajectory.hpp>


namespace crp
{
namespace apl
{

class MotionPlanner : public rclcpp::Node
{
public:
    MotionPlanner();

private:
    void scenarioCallback(const tier4_planning_msgs::msg::Scenario::SharedPtr msg);

    rclcpp::Subscription<tier4_planning_msgs::msg::Scenario>::SharedPtr m_sub_strategy_;
    
    rclcpp::Publisher<autoware_planning_msgs::msg::Trajectory>::SharedPtr m_pub_trajectory_;
};

} // namespace apl
} // namespace crp
#endif // CRP_APL_MOTION_PLANNER_PLANMOTIONPLANNING_HPP
