#include "controller_base/ctrlLatWrapperBase.hpp"


crp::apl::CtrlLatWrapperBase::CtrlLatWrapperBase(const std::string & node_name, const rclcpp::NodeOptions & options)
    : CtrlWrapperBase(node_name, options)
{
}

void crp::apl::CtrlLatWrapperBase::run(const ControlInput & input)
{
    controlLoop(input, m_output);
    publishControlCmd(m_output);
}

void crp::apl::CtrlLatWrapperBase::publishControlCmd(const LatControlOutput & output)
{
    m_ctrlCmdMsg.stamp = this->now();
    m_ctrlCmdMsg.steering_tire_angle = output.steeringAngleTarget;
    m_ctrlCmdMsg.steering_tire_rotation_rate = output.accelerationTarget;

    m_pub_cmd->publish(m_ctrlCmdMsg);
}
