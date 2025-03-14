#ifndef CRP_APL_CTRL_WRAPPER_BASE_CONTROLLERBASE_HPP
#define CRP_APL_CTRL_WRAPPER_BASE_CONTROLLERBASE_HPP


#include <rclcpp/rclcpp.hpp>
#include <crp_msgs/msg/ego.hpp>
#include <autoware_planning_msgs/msg/trajectory.hpp>

#include "controllerInterfaces/controllerInterfaces.hpp"
#include "generalUtils/inc/dataConversion.hpp"


namespace crp
{
namespace apl
{

class CtrlWrapperBase : public rclcpp::Node
{
public:
    CtrlWrapperBase(const std::string & node_name, const rclcpp::NodeOptions & options = rclcpp::NodeOptions());

protected:
    virtual void run(const ControlInput & input) = 0;
    crp::apl::DataConversion m_dataConversion;

private:
    void timerCallback();

    void trajectoryCallback(const autoware_planning_msgs::msg::Trajectory input_msg);
    void egoCallback(const crp_msgs::msg::Ego input_msg);

    rclcpp::Subscription<autoware_planning_msgs::msg::Trajectory>::SharedPtr m_sub_trajectory_;
    rclcpp::Subscription<crp_msgs::msg::Ego>::SharedPtr                      m_sub_ego_;

    rclcpp::TimerBase::SharedPtr m_timer_;

    ControlInput m_input;
};

} // namespace apl
} // namespace crp
#endif // CRP_APL_CTRL_WRAPPER_BASE_CONTROLLERBASE_HPP