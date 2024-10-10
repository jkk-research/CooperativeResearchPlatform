#include "novatel_gps_launcher/novatelTopicConverter.hpp"


crp::vil::NovatelTopicConverter::NovatelTopicConverter() : Node("novatel_topic_converter")
{
    m_sub_currentPose_ = this->create_subscription<geometry_msgs::msg::PoseStamped>("current_pose", 10, std::bind(&NovatelTopicConverter::currentPoseCallback, this, std::placeholders::_1));
    
    m_pub_currentPoseWithCovariance_ = this->create_publisher<geometry_msgs::msg::PoseWithCovarianceStamped>("/sensing/gnss/pose_with_covariance", 10);
}


void crp::vil::NovatelTopicConverter::currentPoseCallback(const geometry_msgs::msg::PoseStamped::SharedPtr msg)
{
    geometry_msgs::msg::PoseWithCovarianceStamped poseWithCovariance;
    poseWithCovariance.header = msg->header;
    poseWithCovariance.pose.pose = msg->pose;

    tf2::Quaternion q_orig;
    tf2::convert(msg->pose.orientation, q_orig);

    tf2::Matrix3x3 m(q_orig);
    double roll, pitch, yaw;
    m.getRPY(roll, pitch, yaw);

    yaw -= M_PI / 2.0;

    tf2::Quaternion q_rotated;
    q_rotated.setRPY(roll, pitch, yaw);

    poseWithCovariance.pose.pose.orientation = tf2::toMsg(q_rotated);

    m_pub_currentPoseWithCovariance_->publish(poseWithCovariance);
}


int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::vil::NovatelTopicConverter>());
    rclcpp::shutdown();
    return 0;
}