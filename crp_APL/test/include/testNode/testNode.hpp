#ifndef CRP_APL_TEST_NODE_HPP
#define CRP_APL_TEST_NODE_HPP

#include <rclcpp/rclcpp.hpp>
#include <tf2/LinearMath/Quaternion.h>
#include <string>
#include "autoware_control_msgs/msg/control.hpp"
#include <autoware_perception_msgs/msg/predicted_objects.hpp>
#include <nav_msgs/msg/occupancy_grid.hpp>
#include <autoware_localization_msgs/msg/kinematic_state.hpp>

#include <tier4_planning_msgs/msg/path_point_with_lane_id.hpp>
#include <tier4_planning_msgs/msg/path_with_lane_id.hpp>
#include <geometry_msgs/msg/pose_with_covariance_stamped.hpp>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>

#include <crp_msgs/msg/ego_status.hpp>
#include <crp_msgs/msg/behavior.hpp>
#include <iostream>

namespace crp
{
namespace apl
{

class TestNode : public rclcpp::Node
{
public:
    TestNode();

private:
    void controlCommandCallback(const autoware_control_msgs::msg::Control::SharedPtr msg);
    
    void run();
    float getYawFromQuaternion(const geometry_msgs::msg::Quaternion & quaternion);

    // member methods
    void vehicleModel();
    float dT = 0.02; // model runs in 20ms

    float* globalToEgoTransform(float globalPose[3]);

    void calculateRoute();
    void calculateSpeed();
    void mapPath();
    void mapObstacles();
    void mapMovingObjects();
    void mapDrivableSurface();

    void mapEgo();

    rclcpp::Subscription<autoware_control_msgs::msg::Control>::SharedPtr m_sub_ctrlCmd_;
    
    // new setup 
    rclcpp::Publisher<autoware_perception_msgs::msg::PredictedObjects>::SharedPtr m_pub_movingObjects_;
    rclcpp::Publisher<autoware_perception_msgs::msg::PredictedObjects>::SharedPtr m_pub_obstacles_;
    rclcpp::Publisher<tier4_planning_msgs::msg::PathWithLaneId>::SharedPtr        m_pub_lanePath_;
    rclcpp::Publisher<nav_msgs::msg::OccupancyGrid>::SharedPtr                    m_pub_drivableSurface_;

    rclcpp::Publisher<autoware_localization_msgs::msg::KinematicState>::SharedPtr m_pub_kinematicState_;
    rclcpp::Publisher<crp_msgs::msg::EgoStatus>::SharedPtr m_pub_egoStatus_;

    rclcpp::Publisher<crp_msgs::msg::Behavior>::SharedPtr m_pub_behavior_;

    // new messages
    tier4_planning_msgs::msg::PathWithLaneId m_outPathMsg;
    autoware_perception_msgs::msg::PredictedObjects m_movingObjectsMsg;
    autoware_perception_msgs::msg::PredictedObjects m_obstaclesMsg;
    nav_msgs::msg::OccupancyGrid m_drivableSurfaceMsg;

    autoware_localization_msgs::msg::KinematicState m_kinematicState;
    crp_msgs::msg::EgoStatus m_egoStatus;

    crp_msgs::msg::Behavior m_behavior;

    rclcpp::TimerBase::SharedPtr m_timer_;

    // target values coming from the vehicle
    double m_roadWheelAngleTarget{0.0f};
    double m_vehicleSpeedTarget{0.0f};
    double p_wheelBase{2.7f}; // in meters
    double m_axEgo{0.0f}; // in m/s^2
    double m_ayEgo{0.0f}; // in m/s^2
    double m_vxEgo{0.0f}; // in m/s^2

    double m_maximumSpeedInit{20.0f}; // in mps
    
    double p_previewDistance = 100; // in meters
    double m_maximumSpeed{0.0f};
    bool m_noEnoughPointsLeft{false};
    bool m_vehicleTooFarFromPath{false};

    // vehicle model, state variables: [X Y Theta], input variable: road wheel angle, parameter: longitudinal speed
    // local path
    std::vector<double> m_localPath_x;
    std::vector<double> m_localPath_y;
    std::vector<double> m_localPath_theta;
    
    double x[3]{0.0f, 0.0f, 0.0f};
};

} // namespace apl
} // namespace crp
#endif // CRP_APL_TEST_NODE_HPP
