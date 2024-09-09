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
#include "autoware_auto_planning_msgs/msg/trajectory.hpp"
#include "autoware_auto_vehicle_msgs/msg/steering_report.hpp"
#include "autoware_auto_vehicle_msgs/msg/vehicle_odometry.hpp"
#include "nav_msgs/msg/odometry.hpp"

#include "../../../inc/lib/utils.hpp"

#include <iostream>
#include <iomanip>
#include <array>
#include <iterator>

using namespace std;

using namespace std::chrono_literals;
using std::placeholders::_1;

class plan_waypoint_publisher : public rclcpp::Node
{
public:
    plan_waypoint_publisher() : Node("plan_waypoint_publisher")
    {
        timer_ = this->create_wall_timer(std::chrono::milliseconds(200), std::bind(&plan_waypoint_publisher::loop, this));  
        traj_pub = this->create_publisher<autoware_auto_planning_msgs::msg::Trajectory>("/planning/scenario_planning/trajectory", 10);

        RCLCPP_INFO(this->get_logger(), "plan_waypoint_publisher has been started");
    }

private:

    crp::apl::transforms m_transforms;
    double c0 = 0; double c1 = 0.1; double c2 = -0.24; double c3 = 0.001;
    void loop()
    {
        // Publish state
        autoware_auto_planning_msgs::msg::Trajectory path_msg;
        autoware_auto_planning_msgs::msg::TrajectoryPoint path_point;
        geometry_msgs::msg::Pose path_pose;
        double point[3];

        // filling up the path with some dummy data
        for (int i=0; i<=100; i++)
        {
            point[0] = double(i);
            point[1] = c0+c1*point[0]+c2*std::pow(point[0], 2)+c3*std::pow(point[0], 3);
            point[2] = 0.0f;
            path_pose.position.x = point[0];
            path_pose.position.y = point[1];
            path_point.pose = path_pose;
            path_msg.points.push_back(path_point);
        }
        traj_pub->publish(path_msg);
    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<autoware_auto_planning_msgs::msg::Trajectory>::SharedPtr traj_pub;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<plan_waypoint_publisher>());
    rclcpp::shutdown();
    return 0;
}