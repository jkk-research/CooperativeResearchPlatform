#include <ctrl_vehicle_control_test/ctrl_vehicle_control_test.hpp>


crp::apl::CtrlVehicleControlTest::CtrlVehicleControlTest() : Node("ctrl_vehicle_control_test")
{
    m_timer_ = this->create_wall_timer(std::chrono::milliseconds(10), std::bind(&CtrlVehicleControlTest::loop, this));  
    m_pub_cmdLat = this->create_publisher<autoware_control_msgs::msg::Lateral>("/control/command/control_cmdLat", 30);
    m_pub_cmdLong = this->create_publisher<autoware_control_msgs::msg::Longitudinal>("/control/command/control_cmdLong", 30);

    m_sub_traj_ = this->create_subscription<autoware_planning_msgs::msg::Trajectory>("/plan/trajectory", 10, std::bind(&CtrlVehicleControlTest::trajCallback, this, std::placeholders::_1));
    m_sub_egoVehicle_ = this->create_subscription<crp_msgs::msg::Ego>("/ego", 10, std::bind(&CtrlVehicleControlTest::egoVehicleCallback, this, std::placeholders::_1));

    this->declare_parameter("/ctrl/test/stepAmplitude", 0.2f);
    this->declare_parameter("/ctrl/test/sinusAmplitude", 0.1f);
    this->declare_parameter("/ctrl/test/sinusFrequency", 0.2f);
        
    RCLCPP_INFO(this->get_logger(), "ctrl_vehicle_control_test has been started");
}

void crp::apl::CtrlVehicleControlTest::trajCallback(const autoware_planning_msgs::msg::Trajectory input_msg)
{
    if (input_msg.points.size() > 0)
        m_target_speed = input_msg.points.at(0).longitudinal_velocity_mps;
    else
        m_target_speed = 0.0f;
    
    m_ctrlCmdLongMsg.velocity = m_target_speed;
}

void crp::apl::CtrlVehicleControlTest::egoVehicleCallback(const crp_msgs::msg::Ego input_msg)
{
    m_egoSpeed = input_msg.twist.twist.linear.x;
    m_steeringAngle = input_msg.tire_angle_front;
}

void crp::apl::CtrlVehicleControlTest::calculateSteeringAngle()
{
    m_steeringAngleTarget = p_stepAmplitude + std::sin(p_sinusFrequency*2*3.14159*t)*p_sinusAmplitude;
}

void crp::apl::CtrlVehicleControlTest::loop()
{
    // parameter assignments
    p_stepAmplitude       = this->get_parameter("/ctrl/test/stepAmplitude").as_double();
    p_sinusAmplitude =      this->get_parameter("/ctrl/test/sinusAmplitude").as_double();
    p_sinusFrequency       = this->get_parameter("/ctrl/test/sinusFrequency").as_double();

    if ((p_stepAmplitude == 0.0f && p_sinusAmplitude == 0.0f) || t > 100.0f)
    {
        t = 0.0f;
    }
    else
    {
        t = t + dT;
    }

    calculateSteeringAngle();

    // steering angle and steering angle gradiant
    m_ctrlCmdLatMsg.stamp = this->now();
    m_ctrlCmdLatMsg.steering_tire_angle = m_steeringAngleTarget;
    m_ctrlCmdLatMsg.steering_tire_rotation_rate = 0.0f;

    m_pub_cmdLat->publish(m_ctrlCmdLatMsg);
    m_pub_cmdLong->publish(m_ctrlCmdLongMsg);
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::apl::CtrlVehicleControlTest>());
    rclcpp::shutdown();
    return 0;
}