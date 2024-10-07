#ifndef VEHICLE_INFO_HPP
#define VEHICLE_INFO_HPP

#include <rclcpp/rclcpp.hpp>
#include <can_msgs/msg/frame.hpp>
#include <pacmod3_msgs/msg/linear_accel_rpt.hpp>

#include <cstdint>
#include <vector>


class PacmodDefinitions
{
public:
    pacmod3_msgs::msg::LinearAccelRpt decodeLinAccel(const can_msgs::msg::Frame::SharedPtr msg);

    const uint16_t LIN_ACCEL_ID = 1049;
};

#endif // VEHICLE_INFO_HPP