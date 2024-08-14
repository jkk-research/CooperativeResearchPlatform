#ifndef CRP_APL_WRAPPER_BASE_PLANBASE_HPP
#define CRP_APL_WRAPPER_BASE_PLANBASE_HPP


#include <rclcpp/rclcpp.hpp>
#include <crp_msgs/msg/target_space.hpp>
#include <crp_msgs/msg/ego.hpp>
#include <tier4_planning_msgs/msg/scenario.hpp>
#include <autoware_planning_msgs/msg/trajectory.hpp>


namespace crp
{
namespace apl
{

class WrapperBase : public rclcpp::Node
{
public:
    WrapperBase(const std::string & node_name, const rclcpp::NodeOptions & options = rclcpp::NodeOptions());

protected:
    struct PlannerInput
    {
        // TODO: standar c++ definitions of the input messages
    };

    struct PlannerOutput
    {
        // TODO: standar c++ definition of the output trajectory message
    };

    virtual void plan(const PlannerInput & input, PlannerOutput & output) = 0;

private:
    void strategyCallback(const tier4_planning_msgs::msg::Scenario::SharedPtr msg);
    void targetSpaceCallback(const crp_msgs::msg::TargetSpace::SharedPtr msg);
    void egoCallback(const crp_msgs::msg::Ego::SharedPtr msg);
    
    void outputCPP2ROS(const PlannerOutput & output, autoware_planning_msgs::msg::Trajectory & msg);
    void publishTrajectory(const PlannerOutput & trajectory);
    void run();

    rclcpp::Subscription<tier4_planning_msgs::msg::Scenario>::SharedPtr m_sub_strategy_;
    rclcpp::Subscription<crp_msgs::msg::TargetSpace>::SharedPtr         m_sub_target_space_;
    rclcpp::Subscription<crp_msgs::msg::Ego>::SharedPtr                 m_sub_ego_;

    rclcpp::Publisher<autoware_planning_msgs::msg::Trajectory>::SharedPtr m_pub_trajectory_;

    PlannerInput  m_input;
    PlannerOutput m_output;
};

} // namespace apl
} // namespace crp
#endif // CRP_APL_WRAPPER_BASE_PLANBASE_HPP
