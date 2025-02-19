#ifndef CRP_APL_CTRL_LAT_WRAPPER_BASE_CONTROLLERBASE_HPP
#define CRP_APL_CTRL_LAT_WRAPPER_BASE_CONTROLLERBASE_HPP


#include <rclcpp/rclcpp.hpp>
#include <autoware_control_msgs/msg/lateral.hpp>

#include "controller_base/ctrlWrapperBase.hpp"

namespace crp
{
namespace apl
{

class CtrlLatWrapperBase : public CtrlWrapperBase
{
public:
    CtrlLatWrapperBase(const std::string & node_name, const rclcpp::NodeOptions & options = rclcpp::NodeOptions());

protected:
    virtual void controlLoop(const ControlInput & input, LatControlOutput & output) = 0;

    LatControlOutput m_output;

private:
    void run() override;
    void publishControlCmd(const LatControlOutput & output);
    rclcpp::Publisher<autoware_control_msgs::msg::Lateral>::SharedPtr m_pub_cmd;

    autoware_control_msgs::msg::Lateral m_ctrlCmdMsg;
};

} // namespace apl
} // namespace crp
#endif // CRP_APL_CTRL_LAT_WRAPPER_BASE_CONTROLLERBASE_HPP