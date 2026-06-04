#ifndef CRP_CIL_MPC_25_WRAPPER_HPP
#define CRP_CIL_MPC_25_WRAPPER_HPP

#include <rclcpp/rclcpp.hpp>
#include <tier4_planning_msgs/msg/path_with_lane_id.hpp>
#include <tier4_planning_msgs/msg/path_point_with_lane_id.hpp>
#include "crp_if/msg/mpc_camera_input.hpp"


namespace crp
{
namespace cil
{

class Mpc25Wrapper : public rclcpp::Node
{
public:
    Mpc25Wrapper();

private:
    void mpcCameraInputCallback(const crp_if::msg::MpcCameraInput::SharedPtr msg);

    rclcpp::Subscription<crp_if::msg::MpcCameraInput>::SharedPtr m_sub_mpcCameraInput_;

    rclcpp::Publisher<tier4_planning_msgs::msg::PathWithLaneId>::SharedPtr m_pub_localLanesFromCamera_;

    float p_pathStepSize = 0.5f; // meters
};

} // namespace cil
} // namespace crp

#endif // CRP_CIL_MPC_25_WRAPPER_HPP