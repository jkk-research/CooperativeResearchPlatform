#include "planner_base/wrapperBase.hpp"


crp::apl::WrapperBase::WrapperBase(const std::string & node_name, const rclcpp::NodeOptions & options)
    : rclcpp::Node(node_name, options)
{
    m_sub_strategy_ = this->create_subscription<tier4_planning_msgs::msg::Scenario>(
        "plan/strategy", 10, std::bind(&WrapperBase::strategyCallback, this, std::placeholders::_1));
    m_sub_target_space_ = this->create_subscription<crp_msgs::msg::TargetSpace>(
        "plan/target_space", 10, std::bind(&WrapperBase::targetSpaceCallback, this, std::placeholders::_1));
    m_sub_ego_ = this->create_subscription<crp_msgs::msg::Ego>(
        "ego", 10, std::bind(&WrapperBase::egoCallback, this, std::placeholders::_1));

    // TODO: DECIDE MOTION_PLANNING AND PLANNERS COMMUNICATION
    std::string planner_name = node_name.substr(5, node_name.size()-5);
    m_pub_trajectory_ = this->create_publisher<autoware_planning_msgs::msg::Trajectory>("plan/"+planner_name+"/trajectory", 10);

    // TODO: timer to run the planners (run(), 30 hz)
}


void crp::apl::WrapperBase::strategyCallback(const tier4_planning_msgs::msg::Scenario::SharedPtr msg)
{
    // TODO: fill the input
    return;
}


void crp::apl::WrapperBase::targetSpaceCallback(const crp_msgs::msg::TargetSpace::SharedPtr msg)
{
    // TODO: fill the input
    return;
}


void crp::apl::WrapperBase::egoCallback(const crp_msgs::msg::Ego::SharedPtr msg)
{
    // TODO: fill the input
    return;
}


void crp::apl::WrapperBase::outputCPP2ROS(const PlannerOutput & output, autoware_planning_msgs::msg::Trajectory & msg)
{
    // TODO: convert output struct to autoware trajectory message
    return;
}


void crp::apl::WrapperBase::publishTrajectory(const PlannerOutput & trajectory)
{
    // autoware_planning_msgs::msg::Trajectory msg;
    // outputCPP2ROS(trajectory, msg);
    // m_pub_trajectory_->publish(msg);

    return;
}


void crp::apl::WrapperBase::run()
{
    // plan(m_input, m_output);
    // publishTrajectory(m_output);

    return;
}
