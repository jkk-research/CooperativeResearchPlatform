#include "novatel_gps_wrapper/novatelTopicConverter.hpp"


crp::vil::NovatelTopicConverter::NovatelTopicConverter() : Node("novatel_topic_converter")
{
    m_sub_currentPose_ = this->create_subscription<nav_msgs::msg::Odometry>("/novatel/oem7/odom", 5, std::bind(&NovatelTopicConverter::currentPoseCallback, this, std::placeholders::_1));
    m_sub_navSatFix_   = this->create_subscription<sensor_msgs::msg::NavSatFix>("/novatel/oem7/fix", 5, std::bind(&NovatelTopicConverter::navSatFixCallback, this, std::placeholders::_1));
    m_sub_imu_         = this->create_subscription<sensor_msgs::msg::Imu>("/novatel/oem7/imu/data", 5, std::bind(&NovatelTopicConverter::imuCallback, this, std::placeholders::_1));

    m_pub_pose       = this->create_publisher<geometry_msgs::msg::PoseWithCovarianceStamped>("/sensing/gnss/pose_with_covariance", 5);
    m_pub_navSatFix_ = this->create_publisher<sensor_msgs::msg::NavSatFix>("/sensing/gnss/nav_sat_fix", 5);
    m_pub_twist_     = this->create_publisher<geometry_msgs::msg::TwistWithCovarianceStamped>("/sensing/gnss/twist", 5);
    m_pub_accel_     = this->create_publisher<geometry_msgs::msg::AccelWithCovarianceStamped>("/sensing/gnss/accel", 5);
    m_pub_odometry_  = this->create_publisher<nav_msgs::msg::Odometry>("/odometry/kinematic_state/odometry", 5);
    // /novatel/oem7/imu/data
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

void crp::vil::NovatelTopicConverter::imuCallback(const sensor_msgs::msg::Imu::SharedPtr msg)
{
    geometry_msgs::msg::AccelWithCovarianceStamped accelMsg;
    accelMsg.header = msg->header;
    accelMsg.accel.accel.linear.x = msg->linear_acceleration.x;
    accelMsg.accel.accel.linear.y = msg->linear_acceleration.y;
    accelMsg.accel.accel.linear.z = msg->linear_acceleration.z;

    m_pub_accel_->publish(accelMsg);
}

void crp::vil::NovatelTopicConverter::currentPoseCallback(const nav_msgs::msg::Odometry::SharedPtr msg)
{
    geometry_msgs::msg::PoseWithCovarianceStamped poseWithCovariance;
    poseWithCovariance.header = msg->header;
    poseWithCovariance.header.frame_id = "map";
    poseWithCovariance.pose.pose = msg->pose.pose;

    publishGPSTransform(msg);

    geometry_msgs::msg::TwistWithCovarianceStamped twistStamped;
    twistStamped.header = msg->header;
    twistStamped.twist = msg->twist;

    m_pub_pose->publish(poseWithCovariance);
    m_pub_twist_->publish(twistStamped);
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