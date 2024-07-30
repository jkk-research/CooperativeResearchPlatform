#include "planner_base/handlerBase.hpp"


crp::apl::HandlerBase::HandlerBase(const std::string & node_name, const rclcpp::NodeOptions & options)
    : rclcpp::Node(node_name, options)
{
    m_sub_strategy_ = this->create_subscription<tier4_planning_msgs::msg::Scenario>(
        "plan/scenario", 10, std::bind(&HandlerBase::strategyCallback, this, std::placeholders::_1));
    m_sub_target_space_ = this->create_subscription<crp_msgs::msg::TargetSpace>(
        "plan/target_space", 10, std::bind(&HandlerBase::targetSpaceCallback, this, std::placeholders::_1));
    m_sub_ego_ = this->create_subscription<crp_msgs::msg::Ego>(
        "plan/ego", 10, std::bind(&HandlerBase::egoCallback, this, std::placeholders::_1));

    // TODO: DECIDE MOTION_PLANNING AND PLANNERS COMMUNICATION
    std::string planner_name = node_name.substr(0, node_name.find("_handler"));
    m_pub_trajectory_ = this->create_publisher<autoware_planning_msgs::msg::Trajectory>("plan/"+planner_name+"/trajectory", 10);

    // TODO: timer to run the planners (run(), 30 hz)
}


void crp::apl::HandlerBase::strategyCallback(const tier4_planning_msgs::msg::Scenario::SharedPtr msg)
{
    // TODO: fill the input
    return;
}


void crp::apl::HandlerBase::targetSpaceCallback(const crp_msgs::msg::TargetSpace::SharedPtr msg)
{
    // TODO: fill the input
    return;
}


void crp::apl::HandlerBase::egoCallback(const crp_msgs::msg::Ego::SharedPtr msg)
{
    // TODO: fill the input
    return;
}


void crp::apl::HandlerBase::outputCPP2ROS(const PlannerOutput & output, autoware_planning_msgs::msg::Trajectory & msg)
{
    // TODO: convert output struct to autoware trajectory message
    return;
}


void crp::apl::HandlerBase::publishTrajectory(const PlannerOutput & trajectory)
{
    // autoware_planning_msgs::msg::Trajectory msg;
    // outputCPP2ROS(trajectory, msg);
    // m_pub_trajectory_->publish(msg);

    return;
}


void crp::apl::HandlerBase::run()
{
    // plan(m_input, m_output);
    // publishTrajectory(m_output);

    return;
}
