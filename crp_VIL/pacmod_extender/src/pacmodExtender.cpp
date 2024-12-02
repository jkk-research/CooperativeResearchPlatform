#include "pacmod_extender/pacmodExtender.hpp"


PacmodExtender::PacmodExtender() : Node("pacmod_extender_node")
{
    m_sub_can_   = this->create_subscription<can_msgs::msg::Frame>("pacmod/can_tx", 10, std::bind(&PacmodExtender::canCallback, this, std::placeholders::_1));
    m_sub_twist_ = this->create_subscription<geometry_msgs::msg::TwistStamped>("vehicle_status", 10, std::bind(&PacmodExtender::twistCallback, this, std::placeholders::_1));

    m_pub_vehicleTwist_ = this->create_publisher<geometry_msgs::msg::TwistWithCovarianceStamped>("/sensing/vehicle/twist", 10);
    m_pub_vehicleAccel_ = this->create_publisher<geometry_msgs::msg::AccelWithCovarianceStamped>("/sensing/vehicle/accel", 10);
    m_pub_linAccel_     = this->create_publisher<pacmod3_msgs::msg::LinearAccelRpt>("pacmod/linear_accel_rpt", 10);
    m_pub_yawRate_        = this->create_publisher<pacmod3_msgs::msg::YawRateRpt>("pacmod/yaw_rate_calc_rpt", 10);

    m_timer_ = this->create_wall_timer(std::chrono::milliseconds(33), std::bind(&PacmodExtender::publishMessages, this));
}


void PacmodExtender::publishMessages()
{
    geometry_msgs::msg::AccelWithCovarianceStamped accelWithCovariance;
    accelWithCovariance.header = m_linAccel.header;
    accelWithCovariance.accel.accel.linear.x = m_linAccel.longitudinal_accel;
    accelWithCovariance.accel.accel.linear.y = m_linAccel.lateral_accel;
    accelWithCovariance.accel.accel.linear.z = m_linAccel.vertical_accel;

    pacmod3_msgs::msg::YawRateRpt yawRateMsg;
    yawRateMsg.header.stamp = this->now();
    yawRateMsg.header.frame_id = "pacmod";

    yawRateMsg.yaw_rate = m_twistWithCovariance.twist.twist.linear.x * tan(m_twistWithCovariance.twist.twist.angular.z / 14.8) / WHEELBASE;

    m_pub_linAccel_->publish(m_linAccel);
    m_pub_yawRate_->publish(yawRateMsg);
    m_pub_vehicleTwist_->publish(m_twistWithCovariance);
    m_pub_vehicleAccel_->publish(accelWithCovariance);
}


void PacmodExtender::canCallback(const can_msgs::msg::Frame::SharedPtr msg)
{
    if (msg->id == pacmodDefinitions.M_LIN_ACCEL_ID)
    {
        m_linAccel = pacmodDefinitions.decodeLinAccel(msg);
    }
}


void PacmodExtender::twistCallback(const geometry_msgs::msg::TwistStamped::SharedPtr msg)
{
    m_twistWithCovariance.header = msg->header;
    m_twistWithCovariance.twist.twist = msg->twist;
    // twist callback has tire angle in angular.z, the published message should have yaw rate
    m_twistWithCovariance.twist.twist.angular.z = msg->twist.linear.x * tan(msg->twist.angular.z) / WHEELBASE;
}


int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<PacmodExtender>());
    rclcpp::shutdown();
    return 0;
}