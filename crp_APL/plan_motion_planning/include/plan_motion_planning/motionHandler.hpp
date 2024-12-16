#ifndef CRP_APL_MOTION_HANDLER_PLANMOTIONPLANNING_HPP
#define CRP_APL_MOTION_HANDLER_PLANMOTIONPLANNING_HPP

#include <rclcpp/rclcpp.hpp>
#include <autoware_planning_msgs/msg/trajectory.hpp>
#include <tier4_planning_msgs/msg/scenario.hpp>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <visualization_msgs/msg/marker.hpp>
#include <string>

#include "plannerInterfaces/plannerInterfaces.hpp"

namespace crp
{
namespace apl
{

class MotionHandler : public rclcpp::Node
{
public:
    MotionHandler();

private:
    // callbacks
    void  scenarioCallback(const tier4_planning_msgs::msg::Scenario::SharedPtr msg);
    void  planLatLaneFollowCallback(const autoware_planning_msgs::msg::Trajectory::SharedPtr msg);
    void  planLonEmergencyCallback(const autoware_planning_msgs::msg::Trajectory::SharedPtr msg);
    void  planLonIntelligentSpeedAdjustCallback(const autoware_planning_msgs::msg::Trajectory::SharedPtr msg);
    // core methods
    void  run();
    void  mapIncomingInputs();
    void  interpolateSpeed(autoware_planning_msgs::msg::Trajectory & outputTrajectory, const PlannerOutput & longitudinalTrajectory);
    // helper methods
    void   visualizeTrajectory();
    float  getYawFromQuaternion(const geometry_msgs::msg::Quaternion & quaternion);
    double pointDistance(const crp::apl::Point3D & p1, const crp::apl::Point3D & p2);
    void   transformPoint(const crp::apl::Point3D & inPoint, const crp::apl::Pose3D & origo, crp::apl::Point3D & outPoint);
    void   findNeighbouringPointsLocal(const PlannerOutputTrajectory & trajectory, const Pose3D & targetPose, const uint32_t & closestPtIdx, int32_t & outIpPointIdx1, int32_t & outIpPointIdx2);

    rclcpp::Subscription<tier4_planning_msgs::msg::Scenario>::SharedPtr m_sub_strategy_;
    rclcpp::Subscription<autoware_planning_msgs::msg::Trajectory>::SharedPtr m_sub_plan_latLaneFollow_;
    rclcpp::Subscription<autoware_planning_msgs::msg::Trajectory>::SharedPtr m_sub_plan_lonEmergency_;
    rclcpp::Subscription<autoware_planning_msgs::msg::Trajectory>::SharedPtr m_sub_plan_lonIntelligentSpeedAdjust_;
    rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr m_pub_trajectory_viz_;
    
    rclcpp::Publisher<autoware_planning_msgs::msg::Trajectory>::SharedPtr m_pub_trajectory_;

    rclcpp::TimerBase::SharedPtr m_timer_;

    autoware_planning_msgs::msg::Trajectory m_outputTrajectory;

    std::string m_currentStrategy{"off"};
    PlannerOutput m_longitudinalTrajectory;
    PlannerOutput m_lateralTrajectory;
};

} // namespace apl
} // namespace crp
#endif // CRP_APL_MOTION_HANDLER_PLANMOTIONPLANNING_HPP
