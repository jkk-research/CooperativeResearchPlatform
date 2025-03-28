#ifndef CRP_APL_BEHAVIOR_PLANNER_PLANBEHAVIORPLANNING_HPP
#define CRP_APL_BEHAVIOR_PLANNER_PLANBEHAVIORPLANNING_HPP


#include <rclcpp/rclcpp.hpp>
#include <crp_msgs/msg/scenario.hpp>
#include <crp_msgs/msg/ego.hpp>
#include <crp_msgs/msg/target_space.hpp>
#include <crp_msgs/msg/behavior.hpp>
#include <autoware_planning_msgs/msg/lanelet_route.hpp>
#include <tier4_planning_msgs/msg/scenario.hpp>

namespace crp
{
namespace apl
{

class BehaviorPlanner : public rclcpp::Node
{
public:
    BehaviorPlanner();

private:
    void routeCallback(const autoware_planning_msgs::msg::LaneletRoute::SharedPtr msg);
    void scenarioCallback(const crp_msgs::msg::Scenario::SharedPtr msg);
    void egoCallback(const crp_msgs::msg::Ego::SharedPtr msg);
    void behaviorCallback(const crp_msgs::msg::Behavior::SharedPtr msg);
    void loop();

    rclcpp::Subscription<autoware_planning_msgs::msg::LaneletRoute>::SharedPtr m_sub_route;
    rclcpp::Subscription<crp_msgs::msg::Scenario>::SharedPtr                   m_sub_scenario;
    rclcpp::Subscription<crp_msgs::msg::Ego>::SharedPtr                        m_sub_ego;
    rclcpp::Subscription<crp_msgs::msg::Behavior>::SharedPtr                   m_sub_behavior_;

    rclcpp::Publisher<tier4_planning_msgs::msg::Scenario>::SharedPtr m_pub_strategy;
    rclcpp::Publisher<crp_msgs::msg::TargetSpace>::SharedPtr         m_pub_targetSpace;

    rclcpp::TimerBase::SharedPtr m_timer_;
    
    float m_maximum_speed;
};

} // namespace apl
} // namespace crp
#endif // CRP_APL_BEHAVIOR_PLANNER_PLANBEHAVIORPLANNING_HPP
