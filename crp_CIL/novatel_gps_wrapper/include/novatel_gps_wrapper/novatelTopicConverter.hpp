#ifndef CRP_VIL_NOVATEL_TOPIC_CONVERTER_NOVATELGPSWRAPPER_HPP
#define CRP_VIL_NOVATEL_TOPIC_CONVERTER_NOVATELGPSWRAPPER_HPP

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <geometry_msgs/msg/pose_with_covariance_stamped.hpp>
#include <geometry_msgs/msg/twist_with_covariance_stamped.hpp>
#include <geometry_msgs/msg/accel_with_covariance_stamped.hpp>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>
#include "tf2_ros/transform_broadcaster.h"
#include <sensor_msgs/msg/nav_sat_fix.hpp>
#include <sensor_msgs/msg/imu.hpp>
#include <nav_msgs/msg/odometry.hpp>

namespace crp
{
namespace vil
{

class NovatelTopicConverter : public rclcpp::Node
{
public:
    NovatelTopicConverter();

private:
    void  currentPoseCallback(const nav_msgs::msg::Odometry::SharedPtr msg);
    void  navSatFixCallback(const sensor_msgs::msg::NavSatFix::SharedPtr msg);
    float getYawFromQuaternion(const geometry_msgs::msg::Quaternion & quaternion);
    void  publishGPSTransform(const nav_msgs::msg::Odometry::SharedPtr msg);
    void  imuCallback(const sensor_msgs::msg::Imu::SharedPtr msg);

    rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr     m_sub_currentPose_;
    rclcpp::Subscription<sensor_msgs::msg::NavSatFix>::SharedPtr m_sub_navSatFix_;
    rclcpp::Subscription<sensor_msgs::msg::Imu>::SharedPtr       m_sub_imu_;

    rclcpp::Publisher<geometry_msgs::msg::PoseWithCovarianceStamped>::SharedPtr  m_pub_pose;
    rclcpp::Publisher<sensor_msgs::msg::NavSatFix>::SharedPtr                    m_pub_navSatFix_;
    rclcpp::Publisher<geometry_msgs::msg::TwistWithCovarianceStamped>::SharedPtr m_pub_twist_;
    rclcpp::Publisher<geometry_msgs::msg::AccelWithCovarianceStamped>::SharedPtr m_pub_accel_;
    rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr                        m_pub_odometry_;

    std::unique_ptr<tf2_ros::TransformBroadcaster> m_tf_broadcaster;
};

} // namespace vil
} // namespace crp

#endif // CRP_VIL_NOVATEL_TOPIC_CONVERTER_NOVATELGPSWRAPPER_HPP