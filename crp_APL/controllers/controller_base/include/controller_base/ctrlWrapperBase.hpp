#ifndef CRP_APL_WRAPPER_BASE_CONTROLLERBASE_HPP
#define CRP_APL_WRAPPER_BASE_CONTROLLERBASE_HPP

#include <rclcpp/rclcpp.hpp>
#include <crp_msgs/msg/ego.hpp>
#include <autoware_planning_msgs/msg/trajectory.hpp>
#include <autoware_control_msgs/msg/lateral.hpp>

#include "controllerInterfaces/controllerInterfaces.hpp"

namespace crp
{
namespace apl
{

class CtrlWrapperBase : public rclcpp::Node
{
public:
    CtrlWrapperBase(const std::string & node_name, const rclcpp::NodeOptions & options = rclcpp::NodeOptions());

protected:
    virtual void controlLoop(const ControlInput & input, LatControlOutput & output) = 0;

private:
    void trajectoryCallback(const autoware_planning_msgs::msg::Trajectory input_msg);
    void egoCallback(const crp_msgs::msg::Ego input_msg);

    rclcpp::Publisher<autoware_control_msgs::msg::Lateral>::SharedPtr m_pub_cmd;

    rclcpp::Subscription<autoware_planning_msgs::msg::Trajectory>::SharedPtr m_sub_trajectory_;
    rclcpp::Subscription<crp_msgs::msg::Ego>::SharedPtr                      m_sub_ego_;

    rclcpp::TimerBase::SharedPtr m_timer_;

    ControlInput     m_input;
    LatControlOutput m_output;

    autoware_control_msgs::msg::Lateral m_ctrlCmdMsg;
};

} // namespace apl
} // namespace crp
#endif // CRP_APL_WRAPPER_BASE_CONTROLLERBASE_HPP