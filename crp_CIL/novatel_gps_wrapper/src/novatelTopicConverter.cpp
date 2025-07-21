#include "novatel_gps_wrapper/novatelTopicConverter.hpp"


crp::vil::NovatelTopicConverter::NovatelTopicConverter() : Node("novatel_topic_converter")
{
    m_sub_currentPose_ = this->create_subscription<geometry_msgs::msg::PoseStamped>("current_pose", 10, std::bind(&NovatelTopicConverter::currentPoseCallback, this, std::placeholders::_1));
    m_sub_navSatFix_   = this->create_subscription<sensor_msgs::msg::NavSatFix>("fix", 10, std::bind(&NovatelTopicConverter::navSatFixCallback, this, std::placeholders::_1));

    m_pub_currentPoseWithCovariance_ = this->create_publisher<geometry_msgs::msg::PoseWithCovarianceStamped>("/sensing/gnss/pose_with_covariance", 10);
    m_pub_navSatFix_ = this->create_publisher<sensor_msgs::msg::NavSatFix>("/sensing/gnss/nav_sat_fix", 10);

    RCLCPP_INFO(this->get_logger(), "novatel_topic_converter has been started");
}

void crp::vil::NovatelTopicConverter::currentPoseCallback(const geometry_msgs::msg::PoseStamped::SharedPtr msg)
{
    geometry_msgs::msg::PoseWithCovarianceStamped poseWithCovariance;
    poseWithCovariance.header = msg->header;
    poseWithCovariance.pose.pose = msg->pose;

    m_pub_currentPoseWithCovariance_->publish(poseWithCovariance);
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