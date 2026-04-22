#ifndef CRP_APL_SCENARIO_GENERATOR_PRCP_SCENARIO_GENERATOR_HPP
#define CRP_APL_SCENARIO_GENERATOR_PRCP_SCENARIO_GENERATOR_HPP

#include <rclcpp/rclcpp.hpp>
#include <crp_msgs/msg/scenario.hpp>
#include <crp_msgs/msg/path_with_traffic_rules.hpp>
#include <crp_msgs/msg/traffic_rule.hpp>
#include <crp_msgs/msg/behavior.hpp>
#include <tier4_planning_msgs/msg/path_with_lane_id.hpp>
#include <autoware_perception_msgs/msg/predicted_objects.hpp>

#include <crp_msgs/msg/ego_status.hpp>
#include <crp_msgs/msg/ego.hpp>
#include <autoware_localization_msgs/msg/kinematic_state.hpp>
#include <sensor_msgs/msg/nav_sat_fix.hpp>

#include <math.h>

namespace crp
{
namespace apl
{

struct Quaternion
{
    double x;
    double y;
    double z;
    double w;
};

class ScenarioGenerator : public rclcpp::Node
{
public:
    ScenarioGenerator();

private:
    void egoKinematicStateCallback(const autoware_localization_msgs::msg::KinematicState::SharedPtr msg);
    void gnssFixCallback(const sensor_msgs::msg::NavSatFix::SharedPtr msg);
    void egoStatusCallback(const crp_msgs::msg::EgoStatus::SharedPtr msg);

    void publishCallback();

    Quaternion yawToQuaternion(double);

    rclcpp::Subscription<autoware_localization_msgs::msg::KinematicState>::SharedPtr m_sub_egoKinematicState_;
    rclcpp::Subscription<sensor_msgs::msg::NavSatFix>::SharedPtr                     m_sub_gnssFix_;
    rclcpp::Subscription<crp_msgs::msg::EgoStatus>::SharedPtr                        m_sub_egoStatus_;

    rclcpp::Publisher<crp_msgs::msg::Scenario>::SharedPtr m_pub_scenario_;

    rclcpp::TimerBase::SharedPtr m_publishTimer_;

    crp_msgs::msg::Ego m_ego;
    autoware_perception_msgs::msg::PredictedObject m_followedObject;

    double m_ego_x{0.0f};
    double m_ego_y{0.0f};
    double m_ego_orientation{0.0f};

    double m_object_x0{100.0f};
    double m_object_y0{0.0f};
    double m_objectOrientation{0.0f};

    bool m_initialized{false};

    double p_axleDistance{2.7f};
};

} // namespace apl
} // namespace crp
#endif // CRP_APL_SCENARIO_GENERATOR_PRCP_SCENARIO_GENERATOR_HPP