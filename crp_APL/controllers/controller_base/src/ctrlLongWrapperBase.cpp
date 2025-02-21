#include "controller_base/ctrlLongWrapperBase.hpp"


crp::apl::CtrlLongWrapperBase::CtrlLongWrapperBase(const std::string & node_name, const rclcpp::NodeOptions & options)
    : CtrlWrapperBase(node_name, options)
{
}

void crp::apl::CtrlLongWrapperBase::run(const ControlInput & input)
{
    controlLoop(input, m_output);
    publishControlCmd(m_output);
}

void crp::apl::CtrlLongWrapperBase::publishControlCmd(const LongControlOutput & output)
{
    m_ctrlCmdMsg.stamp = this->now();
    m_ctrlCmdMsg.velocity = output.velocityTarget;

    m_pub_cmd->publish(m_ctrlCmdMsg);
}
