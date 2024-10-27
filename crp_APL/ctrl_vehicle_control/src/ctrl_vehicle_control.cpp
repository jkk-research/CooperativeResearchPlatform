#include "ctrl_vehicle_control/ctrl_vehicle_control.hpp"

using namespace std::chrono_literals;
using std::placeholders::_1;


crp::apl::ControlHandler::ControlHandler() : Node("ControlHandler")
{
    m_timer_ = this->create_wall_timer(std::chrono::milliseconds(33), std::bind(&ControlHandler::run, this));  
    m_pub_control_ = this->create_publisher<autoware_control_msgs::msg::Control>("/control/command/control_cmd", 30);
    m_pub_twist_ = this->create_publisher<geometry_msgs::msg::Twist>("lexus3/cmd_vel", 30);

    m_sub_controlLat_ = this->create_subscription<autoware_control_msgs::msg::Control>("/control/command/control_cmdLat", 10, std::bind(&ControlHandler::controlLatCallback, this, std::placeholders::_1));
    m_sub_controlLong_ = this->create_subscription<autoware_control_msgs::msg::Control>("/control/command/control_cmdLong", 10, std::bind(&ControlHandler::controlLongCallback, this, std::placeholders::_1));

    RCLCPP_INFO(this->get_logger(), "ctrl_vehicle_control has been started");
}

void crp::apl::ControlHandler::controlLatCallback(const autoware_control_msgs::msg::Control::SharedPtr msg)
{
    twist_msg.angular.z = msg->lateral.steering_tire_angle;
    ctrl_msg.lateral.steering_tire_angle = msg->lateral.steering_tire_angle;
    ctrl_msg.lateral.steering_tire_rotation_rate = 0.0f;
}

void crp::apl::ControlHandler::controlLongCallback(const autoware_control_msgs::msg::Control::SharedPtr msg)
{
    twist_msg.linear.x = msg->longitudinal.velocity;
    ctrl_msg.longitudinal.velocity = msg->longitudinal.velocity;
}

void crp::apl::ControlHandler::run()
{
    m_pub_twist_->publish(twist_msg);
    m_pub_control_->publish(ctrl_msg);
}    

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::apl::ControlHandler>());
    rclcpp::shutdown();
    return 0;
}