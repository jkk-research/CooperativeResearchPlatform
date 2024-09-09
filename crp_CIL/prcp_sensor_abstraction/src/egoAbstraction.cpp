#include "prcp_sensor_abstraction/egoAbstraction.hpp"


crp::cil::EgoAbstraction::EgoAbstraction() : Node("ego_abstraction")
{
    m_sub_currentPose_ = this->create_subscription<geometry_msgs::msg::PoseWithCovarianceStamped>(
        "/sensing/gnss/pose_with_covariance", 10, std::bind(&EgoAbstraction::currentPoseCallback, this, std::placeholders::_1));
    m_sub_kinematicState_ = this->create_subscription<nav_msgs::msg::Odometry>(
        "/odometry/kinematic_state", 10, std::bind(&EgoAbstraction::kinematicStateCallback, this, std::placeholders::_1));

    m_pub_kinematicState_ = this->create_publisher<autoware_localization_msgs::msg::KinematicState>("cai/ego/kinematic_state", 10);
    m_pub_egoStatus_      = this->create_publisher<crp_msgs::msg::EgoStatus>("cai/ego/ego_status", 10);
}


void crp::cil::EgoAbstraction::currentPoseCallback(const geometry_msgs::msg::PoseWithCovarianceStamped::SharedPtr msg)
{
    // TODO
    return;
}


void crp::cil::EgoAbstraction::kinematicStateCallback(const nav_msgs::msg::Odometry::SharedPtr msg)
{
    // TODO
    return;
}


int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::cil::EgoAbstraction>());
    rclcpp::shutdown();
    return 0;
}
