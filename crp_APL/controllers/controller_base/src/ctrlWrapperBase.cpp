#include "controller_base/ctrlWrapperBase.hpp"


crp::apl::CtrlWrapperBase::CtrlWrapperBase(const std::string & node_name, const rclcpp::NodeOptions & options)
    : rclcpp::Node(node_name, options)
{
    
}