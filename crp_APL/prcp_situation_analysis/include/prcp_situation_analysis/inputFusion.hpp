#ifndef CRP_APL_INPUT_FUSION_PRCPSITUATIONANALYSIS_HPP
#define CRP_APL_INPUT_FUSION_PRCPSITUATIONANALYSIS_HPP

#include <rclcpp/rclcpp.hpp>
#include <nav_msgs/msg/occupancy_grid.hpp>
#include <autoware_localization_msgs/msg/kinematic_state.hpp>
#include <autoware_map_msgs/msg/lanelet_map_bin.hpp>
#include <autoware_perception_msgs/msg/predicted_objects.hpp>
#include <tier4_planning_msgs/msg/path_with_lane_id.hpp>
#include <crp_msgs/msg/world.hpp>
#include <crp_msgs/msg/scenario.hpp>
#include <crp_msgs/msg/ego.hpp>

namespace crp
{
namespace apl
{

class InputFusion : public rclcpp::Node
{
public:
    InputFusion();

private:
    void globalStaticMapCallback(const autoware_map_msgs::msg::LaneletMapBin::SharedPtr msg);
    void globalMovingObjectsCallback(const autoware_perception_msgs::msg::PredictedObjects::SharedPtr msg);
    void localMovingObjectsCallback(const autoware_perception_msgs::msg::PredictedObjects::SharedPtr msg);
    void localObstaclesCallback(const autoware_perception_msgs::msg::PredictedObjects::SharedPtr msg);
    void localLanePathCallback(const tier4_planning_msgs::msg::PathWithLaneId::SharedPtr msg);
    void localDrivableSurfaceCallback(const nav_msgs::msg::OccupancyGrid::SharedPtr msg);
    void egoKinematicStateCallback(const autoware_localization_msgs::msg::KinematicState::SharedPtr msg);

    // world
    rclcpp::Subscription<autoware_map_msgs::msg::LaneletMapBin>::SharedPtr           m_sub_global_static_map_;
    rclcpp::Subscription<autoware_perception_msgs::msg::PredictedObjects>::SharedPtr m_sub_global_moving_objects_;
    // scenario
    rclcpp::Subscription<autoware_perception_msgs::msg::PredictedObjects>::SharedPtr m_sub_local_moving_objects_;
    rclcpp::Subscription<autoware_perception_msgs::msg::PredictedObjects>::SharedPtr m_sub_local_obstacles_;
    rclcpp::Subscription<tier4_planning_msgs::msg::PathWithLaneId>::SharedPtr        m_sub_local_lane_path_;
    rclcpp::Subscription<nav_msgs::msg::OccupancyGrid>::SharedPtr                    m_sub_local_drivable_surface_;
    // ego
    rclcpp::Subscription<autoware_localization_msgs::msg::KinematicState>::SharedPtr m_sub_ego_kinematic_state_;


    rclcpp::Publisher<crp_msgs::msg::World>::SharedPtr    m_pub_world_;
    rclcpp::Publisher<crp_msgs::msg::Scenario>::SharedPtr m_pub_scenario_;
    rclcpp::Publisher<crp_msgs::msg::Ego>::SharedPtr      m_pub_ego_;
};

} // namespace apl
} // namespace crp
#endif // CRP_APL_INPUT_FUSION_PRCPSITUATIONANALYSIS_HPP
