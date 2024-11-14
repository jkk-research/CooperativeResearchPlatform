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

#include "autoware_control_msgs/msg/control.hpp"
#include "autoware_planning_msgs/msg/trajectory.hpp"
#include "autoware_vehicle_msgs/msg/steering_report.hpp"

#include "ctrl_vehicle_control/controller_inputs.hpp"
#include "ctrl_vehicle_control/controller_outputs.hpp"
#include "lib/utils.hpp"
#include "compensatory_model/compensatory_model.hpp"

using namespace std::chrono_literals;
using std::placeholders::_1;

class CtrlVehicleControl : public rclcpp::Node
{
public:
    CtrlVehicleControl() : Node("CtrlVehicleControl")
    {
        timer_ = this->create_wall_timer(std::chrono::milliseconds(33), std::bind(&CtrlVehicleControl::loop, this));  
        cmd_pub = this->create_publisher<autoware_control_msgs::msg::Lateral>("/control/command/control_cmdLat", 30);

        traj_sub = this->create_subscription<autoware_planning_msgs::msg::Trajectory>("/plan/trajectory", 10, std::bind(&CtrlVehicleControl::trajCallback, this, std::placeholders::_1));
        ego_vehicle_sub = this->create_subscription<crp_msgs::msg::Ego>("/ego", 10, std::bind(&CtrlVehicleControl::egoVehicleCallback, this, std::placeholders::_1));

        this->declare_parameter("/ctrl/ffGainOffsetGround", 1.0f);
        this->declare_parameter("/ctrl/ffGainSlope", 0.0f);
        this->declare_parameter("/ctrl/ffLookAheadTime", 1.5f);
        this->declare_parameter("/ctrl/ffMinLookAheadDistance", 0.1f);
        this->declare_parameter("/ctrl/steeringAngleLPFilter", 0.7f);
        this->declare_parameter("/ctrl/fbLookAheadTime", 0.0f);
        this->declare_parameter("/ctrl/fbPGain", 1.0);
        this->declare_parameter("/ctrl/fbDGain", 0.1f);
        this->declare_parameter("/ctrl/fbIGain", 0.0f);
        this->declare_parameter("/ctrl/fbThetaGain", 0.0f);
        this->declare_parameter("/ctrl/fbMinLookAheadDistance", 0.0f);
        this->declare_parameter("/ctrl/fbIntegralLimit", 3.0f);
        this->declare_parameter("/ctrl/trajectory_distance", 50.0f);
        this->declare_parameter("/ctrl/debugKPIs", true);

        RCLCPP_INFO(this->get_logger(), "ctrl_vehicle_control has been started");
    }

private:
    // VARIABLES
    crp::apl::ControlInput input;
    crp::apl::ControlOutput output;
    crp::apl::ControlParams params;
    crp::apl::GeometricOperators m_geometricOperator;
    crp::apl::CompensatoryModel m_compensatoryModel;

    void trajCallback(const autoware_planning_msgs::msg::Trajectory input_msg)
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
        }

        if (input_msg.points.size() > 0)
            input.target_speed = input_msg.points.at(0).longitudinal_velocity_mps;
        else
            input.target_speed = 0.0f;
   
    }

    void egoVehicleCallback(const crp_msgs::msg::Ego input_msg)
    {
        input.vxEgo = input_msg.twist.twist.linear.x;
        input.egoSteeringAngle = input_msg.tire_angle_front;

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
        params.trajectory_distance = this->get_parameter("/ctrl/trajectory_distance").as_double();
        params.debugKPIs = this->get_parameter("/ctrl/debugKPIs").as_bool();

        m_compensatoryModel.run(input, output, params);

        // print all the parameters
        RCLCPP_INFO(this->get_logger(), "ffGainOffsetGround: %f", params.ffGainOffsetGround);
        RCLCPP_INFO(this->get_logger(), "ffGainSlope: %f", params.ffGainSlope);
        RCLCPP_INFO(this->get_logger(), "ffLookAheadTime: %f", params.ffLookAheadTime);
        RCLCPP_INFO(this->get_logger(), "ffMinLookAheadDistance: %f", params.ffMinLookAheadDistance);
        RCLCPP_INFO(this->get_logger(), "steeringAngleLPFilter: %f", params.steeringAngleLPFilter);
        RCLCPP_INFO(this->get_logger(), "fbLookAheadTime: %f", params.fbLookAheadTime);
        RCLCPP_INFO(this->get_logger(), "fbPGain: %f", params.fbPGain);
        RCLCPP_INFO(this->get_logger(), "fbDGain: %f", params.fbDGain);
        RCLCPP_INFO(this->get_logger(), "fbIGain: %f", params.fbIGain);
        RCLCPP_INFO(this->get_logger(), "fbThetaGain: %f", params.fbThetaGain);

        // steering angle and steering angle gradiant
        ctrl_cmd.stamp = this->now();
        ctrl_cmd.steering_tire_angle = output.steeringAngleTarget;
        ctrl_cmd.steering_tire_rotation_rate = 0.0f;

        cmd_pub->publish(ctrl_cmd);
    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<autoware_control_msgs::msg::Lateral>::SharedPtr m_pub_cmd;

    rclcpp::Subscription<autoware_planning_msgs::msg::Trajectory>::SharedPtr traj_sub;
    rclcpp::Subscription<crp_msgs::msg::Ego>::SharedPtr ego_vehicle_sub;
    autoware_control_msgs::msg::Lateral ctrlCmd;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<CtrlVehicleControl>());
    rclcpp::shutdown();
    return 0;
}