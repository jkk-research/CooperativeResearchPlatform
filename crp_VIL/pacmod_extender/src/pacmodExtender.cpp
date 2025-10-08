#include "pacmod_extender/pacmodExtender.hpp"


PacmodExtender::PacmodExtender() : Node("pacmod_extender_node")
{
    m_sub_can_      = this->create_subscription<can_msgs::msg::Frame>("pacmod/can_tx", 1, std::bind(&PacmodExtender::canCallback, this, std::placeholders::_1));
    m_sub_twist_    = this->create_subscription<geometry_msgs::msg::TwistStamped>("vehicle_status", 1, std::bind(&PacmodExtender::twistCallback, this, std::placeholders::_1));
    m_sub_steering_ = this->create_subscription<pacmod3_msgs::msg::SystemRptFloat>("pacmod/steering_rpt", 1, std::bind(&PacmodExtender::steeringCallback, this, std::placeholders::_1));
    m_sub_blinker_  = this->create_subscription<pacmod3_msgs::msg::SystemRptInt>("pacmod/turn_rpt", 1, std::bind(&PacmodExtender::blinkerCallback, this, std::placeholders::_1));

    m_pub_vehicleTwist_      = this->create_publisher<geometry_msgs::msg::TwistWithCovarianceStamped>("/sensing/vehicle/twist", 1);
    m_pub_vehicleAccel_      = this->create_publisher<geometry_msgs::msg::AccelWithCovarianceStamped>("/sensing/vehicle/accel", 1);
    m_pub_linAccel_          = this->create_publisher<pacmod3_msgs::msg::LinearAccelRpt>("pacmod/linear_accel_rpt", 1);
    m_pub_yawRate_           = this->create_publisher<pacmod3_msgs::msg::YawRateRpt>("pacmod/yaw_rate_calc_rpt", 1);
    m_pub_tireAngle_         = this->create_publisher<std_msgs::msg::Float32>("/sensing/vehicle/tire_angle", 1);
    m_pub_steeringWheelRate_ = this->create_publisher<std_msgs::msg::Float32>("/sensing/vehicle/steering_wheel_rate", 1);
    m_pub_blinker_           = this->create_publisher<std_msgs::msg::Int8>("/sensing/vehicle/blinker", 1);

    m_timer_ = this->create_wall_timer(std::chrono::milliseconds(33), std::bind(&PacmodExtender::publishMessages, this));

    RCLCPP_INFO(this->get_logger(), "pacmod_extender_node has been started");
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

    yawRateMsg.yaw_rate = m_twistWithCovariance.twist.twist.angular.z;

    m_pub_linAccel_->publish(m_linAccel);
    m_pub_yawRate_->publish(yawRateMsg);
    m_pub_vehicleTwist_->publish(m_twistWithCovariance);
    m_pub_vehicleAccel_->publish(accelWithCovariance);
    m_pub_steeringWheelRate_->publish(m_steeringRate);

}

void PacmodExtender::canCallback(const can_msgs::msg::Frame::SharedPtr msg)
{
    if (msg->id == pacmodDefinitions.M_LIN_ACCEL_ID)
    {
        m_linAccel = pacmodDefinitions.decodeLinAccel(msg);
    }
}

void PacmodExtender::steeringCallback(const pacmod3_msgs::msg::SystemRptFloat::SharedPtr msg)
{
    float steeringRate = (msg->output - m_prevSteeringRpt.output) / ((msg->header.stamp.sec+msg->header.stamp.nanosec * 1e-9) - (m_prevSteeringRpt.header.stamp.sec + m_prevSteeringRpt.header.stamp.nanosec * 1e-9));
    m_steeringRate.data = 0.4 * steeringRate + (1.0 - 0.4) * m_prevSteeringRate; 

    m_prevSteeringRate = m_steeringRate.data;
    m_prevSteeringRpt = *msg;
}

void PacmodExtender::blinkerCallback(const pacmod3_msgs::msg::SystemRptInt::SharedPtr msg)
{
    std_msgs::msg::Int8 blinkerMsg;

    if (msg->output == msg->TURN_LEFT)
    {
        blinkerMsg.data = 1;
    }
    else if (msg->output == msg->TURN_RIGHT)
    {
        blinkerMsg.data = 2;
    }
    else if (msg->output == msg->TURN_HAZARDS)
    {
        blinkerMsg.data = 3;
    }
    else
    {
        blinkerMsg.data = 0;
    }

    m_pub_blinker_->publish(blinkerMsg);
}

void PacmodExtender::twistCallback(const geometry_msgs::msg::TwistStamped::SharedPtr msg)
{
    m_twistWithCovariance.header = msg->header;
    m_twistWithCovariance.twist.twist = msg->twist;
    // twist callback has tire angle in angular.z, the published message should have yaw rate
    m_twistWithCovariance.twist.twist.angular.z = msg->twist.linear.x * tan(msg->twist.angular.z) / WHEELBASE;

    std_msgs::msg::Float32 tireAngleMsg;
    tireAngleMsg.data = msg->twist.angular.z/14.8;

    m_pub_tireAngle_->publish(tireAngleMsg);
}

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<PacmodExtender>());
    rclcpp::shutdown();
    return 0;
}