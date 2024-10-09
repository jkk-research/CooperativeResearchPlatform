#ifndef CRP_CIL_SCENARIO_ABSTRACION_SCENARIOABSTRACTION_HPP
#define CRP_CIL_SCENARIO_ABSTRACION_SCENARIOABSTRACTION_HPP

#include <rclcpp/rclcpp.hpp>
#include <autoware_map_msgs/msg/lanelet_map_bin.hpp>
#include <geometry_msgs/msg/pose_with_covariance_stamped.hpp>
#include <autoware_perception_msgs/msg/predicted_objects.hpp>
#include <nav_msgs/msg/occupancy_grid.hpp>
#include <tier4_planning_msgs/msg/path_with_lane_id.hpp>
#include <tier4_planning_msgs/msg/path_point_with_lane_id.hpp>
#include <nav_msgs/msg/occupancy_grid.hpp>

#include <lanelet2_core/LaneletMap.h>
#include <autoware_lanelet2_extension/utility/message_conversion.hpp>
#include <lanelet2_routing/RoutingGraph.h>
#include <lanelet2_traffic_rules/TrafficRulesFactory.h>
#include <lanelet2_core/geometry/Lanelet.h>
#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_listener.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>
#include <tf2/LinearMath/Quaternion.h>

#include "prcp_sensor_abstraction/abstractionUtils.hpp"


namespace crp
{
namespace cil
{

class ScenarioAbstraction : public rclcpp::Node
{
public:
    ScenarioAbstraction();

private:
    void staticMapFromFileCallback(const autoware_map_msgs::msg::LaneletMapBin::SharedPtr msg);
    void poseCallback(const geometry_msgs::msg::PoseWithCovarianceStamped::SharedPtr msg);
    void publishCallback();

    rclcpp::Subscription<autoware_map_msgs::msg::LaneletMapBin>::SharedPtr         m_sub_staticMapFromFile_;
    rclcpp::Subscription<geometry_msgs::msg::PoseWithCovarianceStamped>::SharedPtr m_sub_pose_;

    rclcpp::Publisher<autoware_perception_msgs::msg::PredictedObjects>::SharedPtr m_pub_movingObjects_;
    rclcpp::Publisher<autoware_perception_msgs::msg::PredictedObjects>::SharedPtr m_pub_obstacles_;
    rclcpp::Publisher<tier4_planning_msgs::msg::PathWithLaneId>::SharedPtr m_pub_lane_path_;
    rclcpp::Publisher<nav_msgs::msg::OccupancyGrid>::SharedPtr m_pub_drivable_surface_;
    rclcpp::Publisher<tier4_planning_msgs::msg::PathWithLaneId>::SharedPtr        m_pub_lanePath_;
    rclcpp::Publisher<nav_msgs::msg::OccupancyGrid>::SharedPtr                    m_pub_drivableSurface_;

    bool m_isMapLoaded = false;
    bool m_isGpsTransformSet = false;

    lanelet::LaneletMapPtr                        m_laneletMap;
    lanelet::traffic_rules::TrafficRulesPtr       m_trafficRules;
    lanelet::routing::RoutingGraphUPtr            m_routingGraph;
    geometry_msgs::msg::PoseWithCovarianceStamped m_egoPoseMapFrame;
    std::string                                   m_mapFrameId;
    geometry_msgs::msg::TransformStamped          m_gps2mapTransform;
    rclcpp::TimerBase::SharedPtr                  m_publishTimer_;

    AbstractionUtils m_abstractionUtils;
};

} // namespace cil
} // namespace crp
#endif // CRP_CIL_SCENARIO_ABSTRACION_SCENARIOABSTRACTION_HPP