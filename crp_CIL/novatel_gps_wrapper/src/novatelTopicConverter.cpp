#include "novatel_gps_wrapper/novatelTopicConverter.hpp"


crp::vil::NovatelTopicConverter::NovatelTopicConverter() : Node("novatel_topic_converter")
{
    m_sub_currentPose_ = this->create_subscription<nav_msgs::msg::Odometry>("/novatel/oem7/odom", 10, std::bind(&NovatelTopicConverter::currentPoseCallback, this, std::placeholders::_1));
    m_sub_navSatFix_   = this->create_subscription<sensor_msgs::msg::NavSatFix>("/novatel/oem7/fix", 10, std::bind(&NovatelTopicConverter::navSatFixCallback, this, std::placeholders::_1));

    m_pub_currentPoseWithCovariance_ = this->create_publisher<geometry_msgs::msg::PoseWithCovarianceStamped>("/sensing/gnss/pose_with_covariance", 10);
    m_pub_navSatFix_ = this->create_publisher<sensor_msgs::msg::NavSatFix>("/sensing/gnss/nav_sat_fix", 10);
    m_pub_odometry_ = this->create_publisher<nav_msgs::msg::Odometry>("/odometry/kinematic_state/odometry", 10);

    RCLCPP_INFO(this->get_logger(), "novatel_topic_converter has been started");

    m_tf_broadcaster = std::make_unique<tf2_ros::TransformBroadcaster>(*this);

}

// write a GPS tf publisher
void crp::vil::NovatelTopicConverter::publishGPSTransform(const nav_msgs::msg::Odometry::SharedPtr msg)
{
    geometry_msgs::msg::TransformStamped transform;
    transform.header.frame_id = "map";
    transform.child_frame_id = "gps";

    transform.transform.translation.x = msg->pose.pose.position.x;
    transform.transform.translation.y = msg->pose.pose.position.y;
    transform.transform.translation.z = msg->pose.pose.position.z;
    transform.transform.rotation = msg->pose.pose.orientation;

    m_tf_broadcaster->sendTransform(transform);
}

void crp::vil::NovatelTopicConverter::currentPoseCallback(const nav_msgs::msg::Odometry::SharedPtr msg)
{
    geometry_msgs::msg::PoseWithCovarianceStamped poseWithCovariance;
    poseWithCovariance.header = msg->header;
    poseWithCovariance.header.frame_id = "map";
    poseWithCovariance.pose.pose = msg->pose.pose;

    publishGPSTransform(msg);

    m_pub_currentPoseWithCovariance_->publish(poseWithCovariance);

    m_pub_odometry_->publish(*msg);
}

void crp::vil::NovatelTopicConverter::navSatFixCallback(const sensor_msgs::msg::NavSatFix::SharedPtr msg)
{
    m_pub_navSatFix_->publish(*msg);
}

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::vil::NovatelTopicConverter>());
    rclcpp::shutdown();
    return 0;
}