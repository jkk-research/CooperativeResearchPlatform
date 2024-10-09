#ifndef CRP_APL_TEST_NODE_HPP
#define CRP_APL_TEST_NODE_HPP

#include <rclcpp/rclcpp.hpp>
#include <autoware_planning_msgs/msg/trajectory.hpp>
#include <tier4_planning_msgs/msg/scenario.hpp>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <string>
#include "autoware_control_msgs/msg/control.hpp"
#include <crp_msgs/msg/world.hpp>
#include <crp_msgs/msg/scenario.hpp>
#include <crp_msgs/msg/ego.hpp>
#include <crp_msgs/msg/path_with_traffic_rules.hpp>
#include <crp_msgs/msg/traffic_rule.hpp>

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
    void mapScenario();
    void mapEgo();

    rclcpp::Subscription<autoware_control_msgs::msg::Control>::SharedPtr m_sub_ctrlCmd_;
    
    rclcpp::Publisher<crp_msgs::msg::World>::SharedPtr    m_pub_world_;
    rclcpp::Publisher<crp_msgs::msg::Scenario>::SharedPtr m_pub_scenario_;
    rclcpp::Publisher<crp_msgs::msg::Ego>::SharedPtr      m_pub_ego_;

    crp_msgs::msg::World m_worldMsg;
    crp_msgs::msg::Scenario m_scenarioMsg;
    crp_msgs::msg::Ego m_egoMsg;

    rclcpp::TimerBase::SharedPtr m_timer_;

    // target values coming from the vehicle
    float m_roadWheelAngleTarget{0.0f};
    float m_vehicleSpeedTarget{0.0f};
    float p_wheelBase{2.7f}; // in meters

    // vehicle model, state variables: [X Y Theta], input variable: road wheel angle, parameter: longitudinal speed
    float x[3]{0.0f};

    // local path
    float m_localPath[1000][3];
    float p_maximumSpeed{20.0}; // in mps
};

} // namespace apl
} // namespace crp
#endif // CRP_APL_TEST_NODE_HPP
