#ifndef CRP_APL_CTRL_LONG_WRAPPER_BASE_CONTROLLERBASE_HPP
#define CRP_APL_CTRL_LONG_WRAPPER_BASE_CONTROLLERBASE_HPP


#include <rclcpp/rclcpp.hpp>
#include <autoware_control_msgs/msg/longitudinal.hpp>

#include "controller_base/ctrlWrapperBase.hpp"

namespace crp
{
namespace apl
{

class CtrlLongWrapperBase : public CtrlWrapperBase
{
public:
    CtrlLongWrapperBase(const std::string & node_name, const rclcpp::NodeOptions & options = rclcpp::NodeOptions());

protected:
    virtual void controlLoop(const ControlInput & input, LongControlOutput & output) = 0;

private:
    void run(const ControlInput & input) override;
    void publishControlCmd(const LongControlOutput & output);

    rclcpp::Publisher<autoware_control_msgs::msg::Longitudinal>::SharedPtr m_pub_cmd;
    
    LongControlOutput m_output;
    autoware_control_msgs::msg::Longitudinal m_ctrlCmdMsg;

};

} // namespace apl
} // namespace crp
#endif // CRP_APL_CTRL_LONG_WRAPPER_BASE_CONTROLLERBASE_HPP