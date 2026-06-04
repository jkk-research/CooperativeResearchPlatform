#include "duro_gps_wrapper/duroTopicConverter.hpp"


crp::vil::DuroTopicConverter::DuroTopicConverter() : Node("duro_topic_converter")
{
    m_sub_currentPose_ = this->create_subscription<geometry_msgs::msg::PoseStamped>("current_pose", 10, std::bind(&DuroTopicConverter::currentPoseCallback, this, std::placeholders::_1));
    
    m_pub_currentPoseWithCovariance_ = this->create_publisher<geometry_msgs::msg::PoseWithCovarianceStamped>("/sensing/gnss/pose_with_covariance", 10);

    RCLCPP_INFO(this->get_logger(), "duro_topic_converter has been started");
}


void crp::vil::DuroTopicConverter::currentPoseCallback(const geometry_msgs::msg::PoseStamped::SharedPtr msg)
{
    geometry_msgs::msg::PoseWithCovarianceStamped poseWithCovariance;
    poseWithCovariance.header = msg->header;
    poseWithCovariance.pose.pose = msg->pose;
    m_pub_currentPoseWithCovariance_->publish(poseWithCovariance);
}


int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::vil::DuroTopicConverter>());
    rclcpp::shutdown();
    return 0;
}