#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <math.h>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/pose.hpp"
#include "std_msgs/msg/float32_multi_array.hpp"
#include "std_msgs/msg/float32.hpp"
#include "crp_msgs/msg/ego.hpp"

#include "autoware_auto_control_msgs/msg/ackermann_control_command.hpp"
#include "autoware_auto_planning_msgs/msg/trajectory.hpp"
#include "autoware_auto_vehicle_msgs/msg/steering_report.hpp"
#include "autoware_auto_vehicle_msgs/msg/vehicle_odometry.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "pacmod3_msgs/msg/vehicle_speed_rpt.hpp"
#include "pacmod3_msgs/msg/system_rpt_float.hpp"

#include "ctrl_vehicle_control/controller_inputs.hpp"
#include "ctrl_vehicle_control/controller_outputs.hpp"
#include "lib/utils.hpp"
#include "compensatory_model/compensatory_model.hpp"

using namespace std::chrono_literals;
using std::placeholders::_1;

class ctrl_vehicle_control : public rclcpp::Node
{
public:
    ctrl_vehicle_control() : Node("ctrl_vehicle_control")
    {
        timer_ = this->create_wall_timer(std::chrono::milliseconds(33), std::bind(&ctrl_vehicle_control::loop, this));  
        cmd_pub = this->create_publisher<autoware_auto_control_msgs::msg::AckermannControlCommand>("/control/command/ctrl_cmd", 30);
        
        traj_sub = this->create_subscription<autoware_auto_planning_msgs::msg::Trajectory>("/planning/scenario_planning/trajectory", 10, std::bind(&ctrl_vehicle_control::traj_callback, this, std::placeholders::_1));
        odom_sub = this->create_subscription<nav_msgs::msg::Odometry>("/localization/kinematic_state", 10, std::bind(&ctrl_vehicle_control::odom_callback, this, std::placeholders::_1));
        ego_vehicle_sub = this->create_subscription<crp_msgs::msg::Ego>("/cai/ego", 10, std::bind(&ctrl_vehicle_control::ego_vehicle_callback, this, std::placeholders::_1));
        cmd_dummy_sub = this->create_subscription<autoware_auto_control_msgs::msg::AckermannControlCommand>("/control/command/control_cmd_dummy", 10, std::bind(&ctrl_vehicle_control::cmd_dummy_callback, this, std::placeholders::_1));

        this->declare_parameter("/ctrl/ffGainOffsetGround", 1.0f);
        this->declare_parameter("/ctrl/ffGainSlope", 0.0f);
        this->declare_parameter("/ctrl/ffLookAheadTime", 1.5f);
        this->declare_parameter("/ctrl/ffMinLookAheadDistance", 0.1f);
        this->declare_parameter("/ctrl/steeringAngleLPFilter", 0.7f);
        this->declare_parameter("/ctrl/fbLookAheadTime", 0.0f);
        this->declare_parameter("/ctrl/fbPGain", 1.0f);
        this->declare_parameter("/ctrl/fbDGain", 0.1f);
        this->declare_parameter("/ctrl/fbIGain", 0.0f);
        this->declare_parameter("/ctrl/fbThetaGain", 0.0f);
        this->declare_parameter("/ctrl/fbMinLookAheadDistance", 0.0f);
        this->declare_parameter("/ctrl/fbIntegralLimit", 3.0f);
        this->declare_parameter("/ctrl/debugKPIs", true);

        RCLCPP_INFO(this->get_logger(), "ctrl_vehicle_control has been started");
    }

private:
    // VARIABLES
    crp::apl::controlInput input;
    crp::apl::controlOutput output;
    crp::apl::controlParams params;
    crp::apl::geometricOperators m_geometricOperator;
    crp::apl::compensatoryModel m_compensatoryModel;

    void cmd_dummy_callback(const autoware_auto_control_msgs::msg::AckermannControlCommand msg_)
    {
        ctrl_cmd = msg_;
    }

    void traj_callback(const autoware_auto_planning_msgs::msg::Trajectory input_msg)
    {
        input.path_x.clear();
        input.path_y.clear();
        input.path_theta.clear();
        double quaternion[4];
        
        // this callback maps the input trajectory to the internal interface
        for (long unsigned int i=0; i<input_msg.points.size(); i++)
        {
            input.path_x.push_back(input_msg.points.at(i).pose.position.x);
            input.path_y.push_back(input_msg.points.at(i).pose.position.y);
            quaternion[0] = input_msg.points.at(i).pose.orientation.x; 
            quaternion[1] = input_msg.points.at(i).pose.orientation.y; 
            quaternion[2] = input_msg.points.at(i).pose.orientation.z; 
            quaternion[3] = input_msg.points.at(i).pose.orientation.w; 
            input.path_theta.push_back(m_geometricOperator.transformQuatToEuler(quaternion));
            if (i<10){
                //printf("input trajectory %d: %f %f %f\n", i, input_msg.points.at(i).pose.position.x, input_msg.points.at(i).pose.position.y, m_geometricOperator.transformQuatToEuler(quaternion));
            }
        }
   
    }

    void odom_callback(const nav_msgs::msg::Odometry input_msg)
    {
        // this callback maps the vehicle localization to the internal interface
        input.egoPoseGlobal[0] = input_msg.pose.pose.position.x;
        input.egoPoseGlobal[1] = input_msg.pose.pose.position.y;
        double quaternion[4];
        quaternion[0] = input_msg.pose.pose.orientation.x; 
        quaternion[1] = input_msg.pose.pose.orientation.y; 
        quaternion[2] = input_msg.pose.pose.orientation.z; 
        quaternion[3] = input_msg.pose.pose.orientation.w; 
        double theta = m_geometricOperator.transformQuatToEuler(quaternion);
        input.egoPoseGlobal[2] = theta;
    }

    void ego_vehicle_callback(const crp_msgs::msg::Ego input_msg)
    {
        input.vxEgo = input_msg.twist.twist.linear.x;
        input.egoSteeringAngle = input_msg.road_wheel_angle_front;
    }

    void loop()
    {
        // parameter assignments
        params.ffGainOffsetGround = this->get_parameter("/ctrl/ffGainOffsetGround").as_double();
        params.ffGainSlope = this->get_parameter("/ctrl/ffGainSlope").as_double();
        params.ffLookAheadTime = this->get_parameter("/ctrl/ffLookAheadTime").as_double();
        params.ffMinLookAheadDistance = this->get_parameter("/ctrl/ffMinLookAheadDistance").as_double();
        params.steeringAngleLPFilter = this->get_parameter("/ctrl/steeringAngleLPFilter").as_double();
        params.fbLookAheadTime = this->get_parameter("/ctrl/fbLookAheadTime").as_double();
        params.fbPGain = this->get_parameter("/ctrl/fbPGain").as_double();
        params.fbDGain = this->get_parameter("/ctrl/fbDGain").as_double();
        params.fbIGain = this->get_parameter("/ctrl/fbIGain").as_double();
        params.fbThetaGain = this->get_parameter("/ctrl/fbThetaGain").as_double();
        params.fbMinLookAheadDistance = this->get_parameter("/ctrl/fbMinLookAheadDistance").as_double();
        params.fbIntegralLimit = this->get_parameter("/ctrl/fbIntegralLimit").as_double();
        params.debugKPIs = this->get_parameter("/ctrl/debugKPIs").as_bool();

        m_compensatoryModel.run(input, output, params);

        // steering angle and steering angle gradiant
        ctrl_cmd.lateral.steering_tire_angle = output.steeringAngleTarget;
        ctrl_cmd.lateral.steering_tire_rotation_rate = 0.0f;

        cmd_pub->publish(ctrl_cmd);
    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<autoware_auto_control_msgs::msg::AckermannControlCommand>::SharedPtr cmd_pub;
    
    rclcpp::Subscription<autoware_auto_planning_msgs::msg::Trajectory>::SharedPtr traj_sub;
    rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odom_sub;
    rclcpp::Subscription<crp_msgs::msg::Ego>::SharedPtr ego_vehicle_sub;
    rclcpp::Subscription<autoware_auto_control_msgs::msg::AckermannControlCommand>::SharedPtr cmd_dummy_sub;

    autoware_auto_control_msgs::msg::AckermannControlCommand ctrl_cmd;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ctrl_vehicle_control>());
    rclcpp::shutdown();
    return 0;
}