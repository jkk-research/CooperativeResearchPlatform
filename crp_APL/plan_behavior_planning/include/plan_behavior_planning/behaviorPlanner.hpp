#ifndef CRP_APL_BEHAVIOR_PLANNER_PLANBEHAVIORPLANNING_HPP
#define CRP_APL_BEHAVIOR_PLANNER_PLANBEHAVIORPLANNING_HPP


#include <rclcpp/rclcpp.hpp>
#include <crp_msgs/msg/scenario.hpp>
#include <crp_msgs/msg/ego.hpp>
#include <crp_msgs/msg/target_space.hpp>
#include <crp_msgs/msg/behavior.hpp>
#include <autoware_planning_msgs/msg/lanelet_route.hpp>
#include <tier4_planning_msgs/msg/path_point_with_lane_id.hpp>
#include <tier4_planning_msgs/msg/scenario.hpp>
#include <visualization_msgs/msg/marker.hpp>
#include <pdp_if/msg/pdp_personalized_params_active.hpp>

#include "plannerInterfaces/plannerInterfaces.hpp"

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
    void PdpPersonalizedParamsActiveCallback(const pdp_if::msg::PdpPersonalizedParamsActive::SharedPtr msg);
    void loop();

    rclcpp::Subscription<autoware_planning_msgs::msg::LaneletRoute>::SharedPtr m_sub_route_;
    rclcpp::Subscription<crp_msgs::msg::Scenario>::SharedPtr                   m_sub_scenario_;
    rclcpp::Subscription<crp_msgs::msg::Ego>::SharedPtr                        m_sub_ego_;
    rclcpp::Subscription<crp_msgs::msg::Behavior>::SharedPtr                   m_sub_behavior_;
    rclcpp::Subscription<pdp_if::msg::PdpPersonalizedParamsActive>::SharedPtr m_pdp_subscriber_;

    rclcpp::Publisher<tier4_planning_msgs::msg::Scenario>::SharedPtr m_pub_strategy_;
    rclcpp::Publisher<crp_msgs::msg::TargetSpace>::SharedPtr         m_pub_targetSpace_;
    rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr    m_pub_laneVisualization_;
    rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr    m_pub_targetSpeedVisualization_;

    rclcpp::TimerBase::SharedPtr m_timer_;

    BehaviorPlannerInput m_behaviorPlannerInput;

    float m_corneringSpeed{0.0f};
    double m_orientationError{0.0f};
};

} // namespace apl
} // namespace crp
#endif // CRP_APL_BEHAVIOR_PLANNER_PLANBEHAVIORPLANNING_HPP
