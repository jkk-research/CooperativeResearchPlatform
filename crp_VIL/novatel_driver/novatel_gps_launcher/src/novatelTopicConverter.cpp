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

    m_pub_currentPoseWithCovariance_->publish(poseWithCovariance);
}


int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::vil::NovatelTopicConverter>());
    rclcpp::shutdown();
    return 0;
}