#ifndef CRP_VIL_ACTUATOR_CONTROL_HPP
#define CRP_VIL_ACTUATOR_CONTROL_HPP

#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include <rclcpp/rclcpp.hpp>
#include <crp_msgs/msg/ego.hpp>
#include "autoware_control_msgs/msg/control.hpp"
#include <crp_msgs/msg/behavior.hpp>

#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/bool.hpp"
#include <pacmod3_msgs/msg/vehicle_speed_rpt.hpp>
#include <pacmod3_msgs/msg/system_cmd_float.hpp>
#include <pacmod3_msgs/msg/system_rpt_float.hpp>
#include <pacmod3_msgs/msg/steering_cmd.hpp>
#include <pdp_if/msg/pdp_personalized_params_active.hpp>

namespace crp
{
namespace vil
{

class ActuatorControl : public rclcpp::Node
{
public:
    ActuatorControl();
    
private:
    void egoCallback(const crp_msgs::msg::Ego msg);
    void controlCmdCallback(const autoware_control_msgs::msg::Control msg);
    void autonomReinitCallback(const std_msgs::msg::Bool msg);
    void behaviorCallback(const crp_msgs::msg::Behavior msg);
    void pdpCallback(const pdp_if::msg::PdpPersonalizedParamsActive::SharedPtr msg);
    void setLongitudinalDynamics();

    void run();

    rclcpp::Subscription<crp_msgs::msg::Ego>::SharedPtr m_sub_ego_;
    rclcpp::Subscription<autoware_control_msgs::msg::Control>::SharedPtr m_sub_controlCommand_;
    rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr m_sub_autonom_reinit_;
    rclcpp::Subscription<crp_msgs::msg::Behavior>::SharedPtr m_sub_behavior_;
    rclcpp::Subscription<pdp_if::msg::PdpPersonalizedParamsActive>::SharedPtr m_sub_pdp_;

    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr m_status_string_pub_;
    rclcpp::Publisher<pacmod3_msgs::msg::SystemCmdFloat>::SharedPtr m_accel_pub_;
    rclcpp::Publisher<pacmod3_msgs::msg::SystemCmdFloat>::SharedPtr m_brake_pub_;
    rclcpp::Publisher<pacmod3_msgs::msg::SteeringCmd>::SharedPtr m_steer_pub_;
    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr m_enable_pub_;

    rclcpp::TimerBase::SharedPtr m_timer_; 

    pacmod3_msgs::msg::SystemCmdFloat m_accelCommandMsg;
    pacmod3_msgs::msg::SystemCmdFloat m_brakeCommandMsg;
    pacmod3_msgs::msg::SteeringCmd m_steerCommandMsg;
    std_msgs::msg::String m_statusStringMsg;
    std_msgs::msg::Bool m_enableMsg;

    float m_p_gain_accel, m_i_gain_accel, m_d_gain_accel;
    float m_p_gain_brake, m_i_gain_brake, m_d_gain_brake;
    bool m_autonom_status_changed = true;
    std::string lx_namespace;
    double m_vehicle_speed_actual = 0.0;
    double m_vehicle_speed_reference = 0.0; 
    double m_vehicle_steering_reference = 0.0;
    double m_currentSteeringTireAngle = 0.0;

    bool m_control_state = true; // control_state: acceleration(true) / deceleration(false) state (necessary for hysteresis to avoid fluctuating speed)

    double m_speed_diff_prev = 0.0;

    double m_p_out_accel = 0.0;
    double m_i_out_accel = 0.0;
    double m_d_out_accel = 0.0;

    double m_p_out_brake = 0.0;
    double m_i_out_brake = 0.0; 
    double m_d_out_brake = 0.0;

    double dt = 0.0;

    double const m_max_i_accel = 1.0, m_max_i_brake = -1.5; // anti windup constants TODO: check valid params
    double m_t_integ_accel = 0.0, m_t_integ_brake = 0.0;     // anti windup
    double m_t_derivative_brake = 0.0;

    double m_accel_command_prev = 0.0;
    double m_brake_command_prev = 0.0;
    double m_prev_time = 0.0;

    double m_maximum_deceleration = -1.0;
    double m_maximum_acceleration = 1.0;
    double m_maximum_jerk = 1.5;
    double m_minimum_jerk = -1.5;

    uint8_t m_uiAccelMode  = 0;
    uint8_t m_uiDecelMode  = 0;
    uint8_t m_uiJerkMode   = 0;
    uint8_t m_pdpAccelMode = 0;
    uint8_t m_pdpDecelMode = 0;

    bool m_enable_lateral_control = false;
    bool m_enable_longitudinal_control = false;
    
    bool m_first_run = true;


};

} // namespace vil
} // namespace crp
#endif //CRP_VIL_ACTUATOR_CONTROL_HPP