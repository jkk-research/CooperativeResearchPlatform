#ifndef CRP_CIL_SCENARIO_ABSTRACION_SCENARIOABSTRACTION_HPP
#define CRP_CIL_SCENARIO_ABSTRACION_SCENARIOABSTRACTION_HPP


#include <rclcpp/rclcpp.hpp>
#include <autoware_perception_msgs/msg/predicted_objects.hpp>
#include <tier4_planning_msgs/msg/path_with_lane_id.hpp>
#include <nav_msgs/msg/occupancy_grid.hpp>

namespace crp
{
namespace cil
{

class ScenarioAbstraction : public rclcpp::Node
{
public:
    ScenarioAbstraction();

private:
    // TODO: subscribers

    rclcpp::Publisher<autoware_perception_msgs::msg::PredictedObjects>::SharedPtr m_pub_moving_objects_;
    rclcpp::Publisher<autoware_perception_msgs::msg::PredictedObjects>::SharedPtr m_pub_obstacles_;
    rclcpp::Publisher<tier4_planning_msgs::msg::PathWithLaneId>::SharedPtr        m_pub_lane_path_;
    rclcpp::Publisher<nav_msgs::msg::OccupancyGrid>::SharedPtr                    m_pub_drivable_surface_;
};

} // namespace cil
} // namespace crp
#endif // CRP_CIL_SCENARIO_ABSTRACION_SCENARIOABSTRACTION_HPP