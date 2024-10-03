#ifndef PACMOD_EXTENDER_HPP
#define PACMOD_EXTENDER_HPP

#include <rclcpp/rclcpp.hpp>
#include <can_msgs/msg/frame.hpp>
#include <geometry_msgs/msg/twist_stamped.hpp>
#include <pacmod3_msgs/msg/linear_accel_rpt.hpp>
#include <pacmod3_msgs/msg/yaw_rate_rpt.hpp>

#include "pacmod_extender/pacmodDefinitions.hpp"


class PacmodExtender : public rclcpp::Node
{
public:
    PacmodExtender();

private:
    void publishMessages();
    void canCallback(const can_msgs::msg::Frame::SharedPtr msg);
    void twistCallback(const geometry_msgs::msg::TwistStamped::SharedPtr msg);

    rclcpp::Subscription<can_msgs::msg::Frame>::SharedPtr sub_can_;
    rclcpp::Subscription<geometry_msgs::msg::TwistStamped>::SharedPtr sub_twist_;

    rclcpp::Publisher<pacmod3_msgs::msg::LinearAccelRpt>::SharedPtr pub_linAccel_;
    rclcpp::Publisher<pacmod3_msgs::msg::YawRateRpt>::SharedPtr pub_yawRate_;

    PacmodDefinitions pacmodDefinitions;

    pacmod3_msgs::msg::LinearAccelRpt linAccel;
    float yawRate{0};

    const float WHEELBASE = 2.79;
};


#endif // PACMOD_EXTENDER_HPP