#ifndef PACMOD_EXTENDER_HPP
#define PACMOD_EXTENDER_HPP

#include <rclcpp/rclcpp.hpp>
#include <can_msgs/msg/frame.hpp>
#include <geometry_msgs/msg/twist_stamped.hpp>
#include <geometry_msgs/msg/twist_with_covariance_stamped.hpp>
#include <geometry_msgs/msg/accel_with_covariance_stamped.hpp>
#include <pacmod3_msgs/msg/yaw_rate_rpt.hpp>
#include <pacmod3_msgs/msg/linear_accel_rpt.hpp>
#include <std_msgs/msg/float32.hpp>


#include "pacmod_extender/pacmodDefinitions.hpp"


class PacmodExtender : public rclcpp::Node
{
public:
    PacmodExtender();

private:
    void canCallback(const can_msgs::msg::Frame::SharedPtr msg);
    void twistCallback(const geometry_msgs::msg::TwistStamped::SharedPtr msg);
    void publishMessages();

    rclcpp::Subscription<can_msgs::msg::Frame>::SharedPtr m_sub_can_;
    rclcpp::Subscription<geometry_msgs::msg::TwistStamped>::SharedPtr m_sub_twist_;

    rclcpp::Publisher<geometry_msgs::msg::TwistWithCovarianceStamped>::SharedPtr m_pub_vehicleTwist_;
    rclcpp::Publisher<geometry_msgs::msg::AccelWithCovarianceStamped>::SharedPtr m_pub_vehicleAccel_;
    rclcpp::Publisher<pacmod3_msgs::msg::LinearAccelRpt>::SharedPtr m_pub_linAccel_;
    rclcpp::Publisher<pacmod3_msgs::msg::YawRateRpt>::SharedPtr m_pub_yawRate_;
    rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr m_pub_tireAngle_;

    PacmodDefinitions pacmodDefinitions;

    rclcpp::TimerBase::SharedPtr m_timer_;
    geometry_msgs::msg::TwistWithCovarianceStamped m_twistWithCovariance;
    pacmod3_msgs::msg::LinearAccelRpt m_linAccel;

    const float WHEELBASE = 2.79;
};


#endif // PACMOD_EXTENDER_HPP