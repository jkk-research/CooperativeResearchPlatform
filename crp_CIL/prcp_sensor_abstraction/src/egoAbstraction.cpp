#include "prcp_sensor_abstraction/egoAbstraction.hpp"


crp::cil::EgoAbstraction::EgoAbstraction() : Node("ego_abstraction")
{
    m_sub_pose_ = this->create_subscription<geometry_msgs::msg::PoseWithCovarianceStamped>(
        "/sensing/gnss/pose_with_covariance", 10, std::bind(&EgoAbstraction::poseCallback, this, std::placeholders::_1));
    
    m_sub_twist_ = this->create_subscription<geometry_msgs::msg::TwistWithCovarianceStamped>(
        "/sensing/vehicle/twist", 10, std::bind(&EgoAbstraction::twistCallback, this, std::placeholders::_1));

    m_sub_accel_ = this->create_subscription<geometry_msgs::msg::AccelWithCovarianceStamped>(
        "/sensing/vehicle/accel", 10, std::bind(&EgoAbstraction::accelCallback, this, std::placeholders::_1));

    m_pub_kinematicState_ = this->create_publisher<autoware_localization_msgs::msg::KinematicState>("cai/kinematic_state", 10);
    m_pub_egoStatus_      = this->create_publisher<crp_msgs::msg::EgoStatus>("cai/ego_status", 10);

    m_publishTimer_ = this->create_wall_timer(
        std::chrono::milliseconds(50), std::bind(&EgoAbstraction::publishCallback, this));
}


void crp::cil::EgoAbstraction::publishCallback()
{
    m_pub_kinematicState_->publish(m_kinematicState);
}


void crp::cil::EgoAbstraction::poseCallback(const geometry_msgs::msg::PoseWithCovarianceStamped::SharedPtr msg)
{
    m_kinematicState.header = msg->header;
    m_kinematicState.pose_with_covariance.pose = msg->pose.pose;
    m_kinematicState.pose_with_covariance.covariance = msg->pose.covariance;
}


void crp::cil::EgoAbstraction::twistCallback(const geometry_msgs::msg::TwistWithCovarianceStamped::SharedPtr msg)
{
    m_kinematicState.header = msg->header;
    m_kinematicState.twist_with_covariance.twist = msg->twist.twist;
    m_kinematicState.twist_with_covariance.covariance = msg->twist.covariance;
}


void crp::cil::EgoAbstraction::accelCallback(const geometry_msgs::msg::AccelWithCovarianceStamped::SharedPtr msg)
{
    m_kinematicState.header = msg->header;
    m_kinematicState.accel_with_covariance.accel = msg->accel.accel;
    m_kinematicState.accel_with_covariance.covariance = msg->accel.covariance;

}


int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::cil::EgoAbstraction>());
    rclcpp::shutdown();
    return 0;
}
