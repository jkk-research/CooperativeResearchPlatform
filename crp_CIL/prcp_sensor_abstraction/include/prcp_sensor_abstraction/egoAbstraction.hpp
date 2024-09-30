#ifndef CRP_CIL_EGO_ABSTRACTION_PRCPSENSORABSTRACTION_HPP
#define CRP_CIL_EGO_ABSTRACTION_PRCPSENSORABSTRACTION_HPP

#include <rclcpp/rclcpp.hpp>
#include <autoware_localization_msgs/msg/kinematic_state.hpp>

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

    rclcpp::Publisher<autoware_localization_msgs::msg::KinematicState>::SharedPtr m_pub_kinematic_state_;
};

} // namespace cil
} // namespace crp
#endif // CRP_CIL_EGO_ABSTRACTION_PRCPSENSORABSTRACTION_HPP