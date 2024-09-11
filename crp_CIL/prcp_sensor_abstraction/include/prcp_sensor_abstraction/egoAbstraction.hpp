#ifndef CRP_CIL_EGO_ABSTRACTION_PRCPSENSORABSTRACTION_HPP
#define CRP_CIL_EGO_ABSTRACTION_PRCPSENSORABSTRACTION_HPP

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/pose_with_covariance_stamped.hpp>
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
    void currentPoseCallback(const geometry_msgs::msg::PoseWithCovarianceStamped::SharedPtr msg);
    void kinematicStateCallback(const nav_msgs::msg::Odometry::SharedPtr msg);

    rclcpp::Subscription<geometry_msgs::msg::PoseWithCovarianceStamped>::SharedPtr m_sub_currentPose_;
    rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr                       m_sub_kinematicState_;

    rclcpp::Publisher<autoware_localization_msgs::msg::KinematicState>::SharedPtr m_pub_kinematicState_;
    rclcpp::Publisher<crp_msgs::msg::EgoStatus>::SharedPtr m_pub_egoStatus_;

    nav_msgs::msg::Odometry m_odometry;
};

} // namespace cil
} // namespace crp
#endif // CRP_CIL_EGO_ABSTRACTION_PRCPSENSORABSTRACTION_HPP