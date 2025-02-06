#include "ekf_wrapper/ekfTopicConverter.hpp"


crp::vil::EKFTopicConverter::EKFTopicConverter() : Node("ekf_topic_converter")
{
    this->declare_parameter<std::string>("ekf_frame", "map");
    
    this->get_parameter("ekf_frame", m_ekfFrame);

    sub_ekfPose_ = this->create_subscription<geometry_msgs::msg::PoseStamped>("/ekf/estimated_pose_baselink", 10, std::bind(&EKFTopicConverter::ekfPoseCallback, this, std::placeholders::_1));

    pub_ekfPose_ = this->create_publisher<geometry_msgs::msg::PoseWithCovarianceStamped>("/sensing/ekf/estimated_pose", 1);

    RCLCPP_INFO(this->get_logger(), "ekf_topic_converter has been started");
}


void crp::vil::EKFTopicConverter::ekfPoseCallback(const geometry_msgs::msg::PoseStamped::SharedPtr msg)
{
    geometry_msgs::msg::PoseWithCovarianceStamped converted_msg;
    converted_msg.header.stamp = msg->header.stamp;
    converted_msg.header.frame_id = m_ekfFrame;
    converted_msg.pose.pose = msg->pose;

    pub_ekfPose_->publish(converted_msg);
}


int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::vil::EKFTopicConverter>());
    rclcpp::shutdown();
    return 0;
}