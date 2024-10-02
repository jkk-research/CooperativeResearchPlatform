#ifndef CRP_CIL_EGO_ABSTRACTION_PRCPSENSORABSTRACTION_HPP
#define CRP_CIL_EGO_ABSTRACTION_PRCPSENSORABSTRACTION_HPP

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/pose_with_covariance_stamped.hpp>
#include <geometry_msgs/msg/twist_with_covariance_stamped.hpp>
#include <geometry_msgs/msg/accel_with_covariance_stamped.hpp>
#include <autoware_localization_msgs/msg/kinematic_state.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <crp_msgs/msg/ego_status.hpp>


namespace crp
{
namespace cil
{

class EgoAbstraction : public rclcpp::Node
{
public:
    EgoAbstraction();

private:
    void poseCallback(const geometry_msgs::msg::PoseWithCovarianceStamped::SharedPtr msg);
    void twistCallback(const geometry_msgs::msg::TwistWithCovarianceStamped::SharedPtr msg);
    void accelCallback(const geometry_msgs::msg::AccelWithCovarianceStamped::SharedPtr msg);
    void kinematicStateCallback(const nav_msgs::msg::Odometry::SharedPtr msg);
    void publishCallback();

    rclcpp::Subscription<geometry_msgs::msg::PoseWithCovarianceStamped>::SharedPtr  m_sub_pose_;
    rclcpp::Subscription<geometry_msgs::msg::TwistWithCovarianceStamped>::SharedPtr m_sub_twist_;
    rclcpp::Subscription<geometry_msgs::msg::AccelWithCovarianceStamped>::SharedPtr m_sub_accel_;

    rclcpp::Publisher<autoware_localization_msgs::msg::KinematicState>::SharedPtr m_pub_kinematicState_;
    rclcpp::Publisher<crp_msgs::msg::EgoStatus>::SharedPtr m_pub_egoStatus_;

    autoware_localization_msgs::msg::KinematicState m_kinematicState;

    rclcpp::TimerBase::SharedPtr m_publishTimer_;
};

} // namespace cil
} // namespace crp
#endif // CRP_CIL_EGO_ABSTRACTION_PRCPSENSORABSTRACTION_HPP