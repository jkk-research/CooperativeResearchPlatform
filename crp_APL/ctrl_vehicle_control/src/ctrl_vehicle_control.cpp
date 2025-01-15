#include "ctrl_vehicle_control/ctrl_vehicle_control.hpp"

using namespace std::chrono_literals;
using std::placeholders::_1;


crp::apl::ControlHandler::ControlHandler() : Node("ControlHandler")
{
    this->declare_parameter<float>("lat_accel_lim", 3.0f);
    this->declare_parameter<float>("jerk_lim", 0.5f);
    this->declare_parameter<float>("vehicle_param_L", 2.7f);

    this->get_parameter<float>("lat_accel_lim", m_latAccelLim);
    this->get_parameter<float>("jerk_lim", m_jerkLim);
    this->get_parameter<float>("vehicle_param_L", m_wheelBase);

    m_timer_ = this->create_wall_timer(std::chrono::milliseconds(33), std::bind(&ControlHandler::run, this));  
    m_pub_control_ = this->create_publisher<autoware_control_msgs::msg::Control>("/control/command/control_cmd", 30);
    m_pub_twist_ = this->create_publisher<geometry_msgs::msg::Twist>("lexus3/cmd_vel", 30);

    m_sub_controlLat_ = this->create_subscription<autoware_control_msgs::msg::Lateral>("/control/command/control_cmdLat", 10, std::bind(&ControlHandler::controlLatCallback, this, std::placeholders::_1));
    m_sub_controlLong_ = this->create_subscription<autoware_control_msgs::msg::Longitudinal>("/control/command/control_cmdLong", 10, std::bind(&ControlHandler::controlLongCallback, this, std::placeholders::_1));

    RCLCPP_INFO(this->get_logger(), "ctrl_vehicle_control has been started");

    // initialize control message

    m_ctrlCmdMsg.lateral.steering_tire_angle = 0.0f;
    m_ctrlCmdMsg.lateral.steering_tire_rotation_rate = 0.0f;
    m_ctrlCmdMsg.longitudinal.velocity = 0.0f;

    // initialize twist message
    m_twistMsg.linear.x = 0.0f;
    m_twistMsg.linear.y = 0.0f;
    m_twistMsg.linear.z = 0.0f;
    m_twistMsg.angular.x = 0.0f;
    m_twistMsg.angular.y = 0.0f;
    m_twistMsg.angular.z = 0.0f;
}

void crp::apl::ControlHandler::controlLatCallback(const autoware_control_msgs::msg::Lateral::SharedPtr msg)
{
    // check for nan values
    if (std::isnan(msg->steering_tire_angle))
    {
        RCLCPP_WARN(this->get_logger(), "Received nan value for steering angle");
        return;
    }

    float tireAngleLim = 20; // default: no limit

    if (m_twistMsg.linear.x != 0)
    {
        // calculate tire angle limit based on lateral acceleration limit
        tireAngleLim = atan((m_latAccelLim * m_wheelBase) / pow(m_twistMsg.linear.x, 2));
    }

    m_twistMsg.angular.z = std::min(msg->steering_tire_angle, tireAngleLim);
    m_ctrlCmdMsg.lateral.steering_tire_angle = std::min(msg->steering_tire_angle, tireAngleLim);

    float tireRotationRateLim = 10; // default: no limit
    
    if (m_twistMsg.linear.x != 0)
    {
        // calculate tire rotation rate limit based on jerk limit
        // (jerk * cos^2(steering angle) * L) / v^2 - (2 * ax * tan(steering angle) * cos^2(steering angle)) / v
        float ax = 2.0f; // calculate with constant acceleration
        tireRotationRateLim = ((m_jerkLim*pow(cos(m_ctrlCmdMsg.lateral.steering_tire_angle), 2)*m_wheelBase) / pow(m_ctrlCmdMsg.longitudinal.velocity, 2)) - (2*ax*tan(m_ctrlCmdMsg.lateral.steering_tire_angle)*pow(cos(m_ctrlCmdMsg.lateral.steering_tire_angle), 2)) / m_ctrlCmdMsg.longitudinal.velocity;
    }

    m_ctrlCmdMsg.lateral.steering_tire_rotation_rate = std::min(msg->steering_tire_rotation_rate, tireRotationRateLim);
}

void crp::apl::ControlHandler::controlLongCallback(const autoware_control_msgs::msg::Longitudinal::SharedPtr msg)
{
    // check for nan values
    if (std::isnan(msg->velocity))
    {
        RCLCPP_WARN(this->get_logger(), "Received nan value for velocity");
        return;
    }


    m_twistMsg.linear.x = msg->velocity;
    m_ctrlCmdMsg.longitudinal.velocity = msg->velocity;
}

void crp::apl::ControlHandler::run()
{
    m_ctrlCmdMsg.stamp = this->now();

    m_pub_twist_->publish(m_twistMsg);
    m_pub_control_->publish(m_ctrlCmdMsg);
}    

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::apl::ControlHandler>());
    rclcpp::shutdown();
    return 0;
}