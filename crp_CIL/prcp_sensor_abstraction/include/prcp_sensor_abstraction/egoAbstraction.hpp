#ifndef CRP_CIL_EGO_ABSTRACTION_PRCPSENSORABSTRACTION_HPP
#define CRP_CIL_EGO_ABSTRACTION_PRCPSENSORABSTRACTION_HPP

#include <rclcpp/rclcpp.hpp>
#include <nav_msgs/msg/odometry.hpp>

namespace crp
{
namespace cil
{

class EgoAbstraction : public rclcpp::Node
{
public:
    EgoAbstraction();

private:
    // TODO: subscribers

    rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr m_pub_kinematic_state_;
};

} // namespace cil
} // namespace crp
#endif // CRP_CIL_EGO_ABSTRACTION_PRCPSENSORABSTRACTION_HPP