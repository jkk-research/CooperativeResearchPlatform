#include "pacmod_extender/pacmodExtender.hpp"


PacmodExtender::PacmodExtender() : Node("pacmod_extender_node")
{
    sub_can_   = this->create_subscription<can_msgs::msg::Frame>("pacmod/can_tx", 10, std::bind(&PacmodExtender::canCallback, this, std::placeholders::_1));
    sub_twist_ = this->create_subscription<geometry_msgs::msg::TwistStamped>("vehicle_status", 10, std::bind(&PacmodExtender::twistCallback, this, std::placeholders::_1));

    pub_linAccel_ = this->create_publisher<pacmod3_msgs::msg::LinearAccelRpt>("pacmod/linear_accel_rpt", 10);
    pub_yawRate_  = this->create_publisher<pacmod3_msgs::msg::YawRateRpt>("pacmod/yaw_rate_calc_rpt", 10);
}

void PacmodExtender::canCallback(const can_msgs::msg::Frame::SharedPtr msg)
{
    if (msg->id == pacmodDefinitions.LIN_ACCEL_ID)
    {
        linAccel = pacmodDefinitions.decodeLinAccel(msg);
        pub_linAccel_->publish(linAccel);
    }
}

void PacmodExtender::twistCallback(const geometry_msgs::msg::TwistStamped::SharedPtr msg)
{
    pacmod3_msgs::msg::YawRateRpt yawRateMsg;
    yawRateMsg.header.stamp = this->now();
    yawRateMsg.header.frame_id = "pacmod";

    yawRate = msg->twist.linear.x * tan(msg->twist.angular.z) / WHEELBASE;
    yawRateMsg.yaw_rate = yawRate;
    
    pub_yawRate_->publish(yawRateMsg);
}

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<PacmodExtender>());
    rclcpp::shutdown();
    return 0;
}