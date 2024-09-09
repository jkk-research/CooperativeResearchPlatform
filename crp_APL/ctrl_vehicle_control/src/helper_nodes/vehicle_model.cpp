#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <math.h>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/pose.hpp"
#include "std_msgs/msg/float32_multi_array.hpp"
#include "std_msgs/msg/float32.hpp"

#include "autoware_auto_control_msgs/msg/ackermann_lateral_command.hpp"
#include "autoware_auto_control_msgs/msg/ackermann_control_command.hpp"
#include "autoware_auto_planning_msgs/msg/trajectory.hpp"
#include "autoware_auto_vehicle_msgs/msg/steering_report.hpp"
#include "autoware_auto_vehicle_msgs/msg/vehicle_odometry.hpp"
#include "pacmod3_msgs/msg/vehicle_speed_rpt.hpp"
#include "pacmod3_msgs/msg/yaw_rate_rpt.hpp"
#include "nav_msgs/msg/odometry.hpp"

#include "lib/utils.hpp"

#include <iostream>
#include <iomanip>
#include <array>
#include <iterator>

using namespace std;

using namespace std::chrono_literals;
using std::placeholders::_1;

class model_vehicle_model : public rclcpp::Node
{
public:
    model_vehicle_model() : Node("model_vehicle_model")
    {
        timer_ = this->create_wall_timer(std::chrono::milliseconds(10), std::bind(&model_vehicle_model::loop, this));  

        odom_pub = this->create_publisher<nav_msgs::msg::Odometry>("/localization/kinematic_state", 100);
        vehspeed_pub = this->create_publisher<pacmod3_msgs::msg::VehicleSpeedRpt>("/pacmod/vehicle_speed_rpt", 100);
        yawrate_pub = this->create_publisher<pacmod3_msgs::msg::YawRateRpt>("/pacmod/yaw_rate_rpt", 100);
        
        cmd_sub = this->create_subscription<autoware_auto_control_msgs::msg::AckermannControlCommand>("/control/command/ctrl_cmd", 10, std::bind(&model_vehicle_model::cmd_callback, this, std::placeholders::_1));

        RCLCPP_INFO(this->get_logger(), "model_vehicle_model has been started");
    }

private:

    crp::apl::geometricOperators m_geometricOperator;

    double steeringCmd = 0.0f; 
    double curvature = 0.0f;
    double L = 2.7; // m
    double vx = 10.0f; // m/s
    double yawRate = 0.0f;
    double egoPose[3]{0.0f, 0.0f, 0.0f};
    double dT = 0.01;
    double* q;

    void cmd_callback(const autoware_auto_control_msgs::msg::AckermannControlCommand input_msg)
    {
        steeringCmd = input_msg.lateral.steering_tire_angle;
    }

    void loop()
    {
        // Publish state
        nav_msgs::msg::Odometry odom_msg;
        pacmod3_msgs::msg::VehicleSpeedRpt vehSpeed_msg;
        pacmod3_msgs::msg::YawRateRpt yawRate_msg;
        

        // kinematic bicycle model
        curvature = std::tan(steeringCmd)/L;
        yawRate = curvature*vx;
        egoPose[2] = egoPose[2] + dT*yawRate;
        double dx = vx*dT*std::cos(egoPose[2]);
        double dy = vx*dT*std::sin(egoPose[2]);
        egoPose[0] = egoPose[0]+dx;
        egoPose[1] = egoPose[1]+dy;

        odom_msg.pose.pose.position.x = egoPose[0];
        odom_msg.pose.pose.position.y = egoPose[1];
        q = m_geometricOperator.ToQuaternion(0.0, 0.0, egoPose[2]);
        odom_msg.pose.pose.orientation.x = q[0];
        odom_msg.pose.pose.orientation.y = q[1];
        odom_msg.pose.pose.orientation.z = q[2];
        odom_msg.pose.pose.orientation.w = q[3];

        vehSpeed_msg.vehicle_speed = vx;
        yawRate_msg.yaw_rate = yawRate;

        printf("steering command is %f\n", steeringCmd);
        printf("vehicle pose is: [%f %f %f]\n", egoPose[0], egoPose[1], egoPose[2]);

        odom_pub->publish(odom_msg);
        vehspeed_pub->publish(vehSpeed_msg);
        yawrate_pub->publish(yawRate_msg);
    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr odom_pub;
    rclcpp::Publisher<pacmod3_msgs::msg::VehicleSpeedRpt>::SharedPtr vehspeed_pub;
    rclcpp::Publisher<pacmod3_msgs::msg::YawRateRpt>::SharedPtr yawrate_pub;

    rclcpp::Subscription<autoware_auto_control_msgs::msg::AckermannControlCommand>::SharedPtr cmd_sub;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<model_vehicle_model>());
    rclcpp::shutdown();
    return 0;
}