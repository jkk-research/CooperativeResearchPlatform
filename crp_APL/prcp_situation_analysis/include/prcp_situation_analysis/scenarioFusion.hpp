#ifndef CRP_APL_SCENARIO_FUSION_PRCPSITUATIONANALYSIS_HPP
#define CRP_APL_SCENARIO_FUSION_PRCPSITUATIONANALYSIS_HPP

#include <rclcpp/rclcpp.hpp>
#include <crp_msgs/msg/scenario.hpp>
#include <crp_msgs/msg/path_with_traffic_rules.hpp>
#include <crp_msgs/msg/traffic_rule.hpp>
#include <crp_msgs/msg/behavior.hpp>
#include <tier4_planning_msgs/msg/path_with_lane_id.hpp>
#include <autoware_perception_msgs/msg/predicted_objects.hpp>


namespace crp
{
namespace apl
{

class ScenarioFusion : public rclcpp::Node
{
public:
    ScenarioFusion();

private:
    void localMovingObjectsCallback(const autoware_perception_msgs::msg::PredictedObjects::SharedPtr msg);
    void localObstaclesCallback(const autoware_perception_msgs::msg::PredictedObjects::SharedPtr msg);
    void localPathCallback(const tier4_planning_msgs::msg::PathWithLaneId::SharedPtr msg);
    void localDrivableSurfaceCallback(const nav_msgs::msg::OccupancyGrid::SharedPtr msg);
    void publishCallback();

    rclcpp::Subscription<autoware_perception_msgs::msg::PredictedObjects>::SharedPtr m_sub_localMovingObjects_;
    rclcpp::Subscription<autoware_perception_msgs::msg::PredictedObjects>::SharedPtr m_sub_localObstacles_;
    rclcpp::Subscription<tier4_planning_msgs::msg::PathWithLaneId>::SharedPtr        m_sub_localPath_;
    rclcpp::Subscription<nav_msgs::msg::OccupancyGrid>::SharedPtr                    m_sub_localDrivableSurface_;

    rclcpp::Publisher<crp_msgs::msg::Scenario>::SharedPtr m_pub_scenario_;

    rclcpp::TimerBase::SharedPtr m_publishTimer_;

    crp_msgs::msg::Scenario m_scenario;
};

} // namespace apl
} // namespace crp
#endif // CRP_APL_SCENARIO_FUSION_PRCPSITUATIONANALYSIS_HPP