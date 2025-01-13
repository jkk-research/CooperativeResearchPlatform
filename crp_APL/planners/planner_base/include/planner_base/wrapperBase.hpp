#ifndef CRP_APL_WRAPPER_BASE_PLANBASE_HPP
#define CRP_APL_WRAPPER_BASE_PLANBASE_HPP

#include <rclcpp/rclcpp.hpp>
#include <crp_msgs/msg/target_space.hpp>
#include <crp_msgs/msg/ego.hpp>
#include <autoware_planning_msgs/msg/trajectory.hpp>
#include <autoware_planning_msgs/msg/trajectory_point.hpp>
#include <autoware_perception_msgs/msg/object_classification.hpp>
#include <autoware_perception_msgs/msg/predicted_path.hpp>
#include <nav_msgs/msg/occupancy_grid.hpp>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <tier4_planning_msgs/msg/scenario.hpp>

#include "../../../interfaces/plannerInterfaces/plannerInterfaces.hpp"

#include "../../../crp_utils/geometryUtils/inc/geometricPathCalculation.hpp"


namespace crp
{
namespace apl
{

class WrapperBase : public rclcpp::Node
{
public:
    WrapperBase(const std::string & node_name, const rclcpp::NodeOptions & options = rclcpp::NodeOptions());

protected:
    virtual void plan(const PlannerInput & input, PlannerOutput & output) = 0;
    bool inputPlausibilityCheck(const PlannerInput & input);

private:
    void strategyCallback(const tier4_planning_msgs::msg::Scenario::SharedPtr msg);
    void targetSpaceCallback(const crp_msgs::msg::TargetSpace::SharedPtr msg);
    void egoCallback(const crp_msgs::msg::Ego::SharedPtr msg);
    
    float getYawFromQuaternion(const geometry_msgs::msg::Quaternion & quaternion);
    OccupancyGrid convertMsgToOccupancyGrid(const nav_msgs::msg::OccupancyGrid & msg);
    PlannerInputObject convertMsgToObjects(const autoware_perception_msgs::msg::PredictedObject & msg);
    void convertOutputToMsg(const PlannerOutput & output, autoware_planning_msgs::msg::Trajectory & msg);
    void publishTrajectory(const PlannerOutput & trajectory);
    void run();

    rclcpp::Subscription<tier4_planning_msgs::msg::Scenario>::SharedPtr m_sub_strategy_;
    rclcpp::Subscription<crp_msgs::msg::TargetSpace>::SharedPtr         m_sub_target_space_;
    rclcpp::Subscription<crp_msgs::msg::Ego>::SharedPtr                 m_sub_ego_;

    rclcpp::Publisher<autoware_planning_msgs::msg::Trajectory>::SharedPtr m_pub_trajectory_;

    rclcpp::TimerBase::SharedPtr m_timer_;

    PlannerInput  m_input;
    PlannerOutput m_output;

    GeometricPathCalculation m_geometricPathCalculator;
};

} // namespace apl
} // namespace crp
#endif // CRP_APL_WRAPPER_BASE_PLANBASE_HPP
