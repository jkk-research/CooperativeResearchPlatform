#include "actuator_control/actuator_control.hpp"

crp::vil::ActuatorControl::ActuatorControl() : Node("actuator_control")
{
    this->declare_parameter<float>("p_gain_accel", 0.29);
    this->declare_parameter<float>("i_gain_accel", 0.035);
    this->declare_parameter<float>("d_gain_accel", 0.0);
    this->declare_parameter<float>("p_gain_brake", 0.15);
    this->declare_parameter<float>("i_gain_brake", 0.038);
    this->declare_parameter<float>("d_gain_brake", 0.0);
    
    this->get_parameter("p_gain_accel", m_p_gain_accel);
    this->get_parameter("i_gain_accel", m_i_gain_accel);
    this->get_parameter("d_gain_accel", m_d_gain_accel);
    this->get_parameter("p_gain_brake", m_p_gain_brake);
    this->get_parameter("i_gain_brake", m_i_gain_brake);
    this->get_parameter("d_gain_brake", m_d_gain_brake);

    m_timer_ = this->create_wall_timer(std::chrono::milliseconds(33), std::bind(&ActuatorControl::run, this));  

    m_sub_ego_ = this->create_subscription<crp_msgs::msg::Ego>("/ego", 10, std::bind(&ActuatorControl::egoCallback, this, std::placeholders::_1));
    m_sub_controlCommand_ = this->create_subscription<autoware_control_msgs::msg::Control>("/control/command/ctrlCmd", 10, std::bind(&ActuatorControl::controlCmdCallback, this, std::placeholders::_1));
    m_sub_autonom_reinit_ = this->create_subscription<std_msgs::msg::Bool>("control_reinit", 10, std::bind(&ActuatorControl::autonomReinitCallback, this, std::placeholders::_1));
    m_sub_behavior_ = this->create_subscription<crp_msgs::msg::Behavior>(
        "/ui/behavior", 10,
        std::bind(&ActuatorControl::behaviorCallback, this, std::placeholders::_1));

    m_accel_pub_ = this->create_publisher<pacmod3_msgs::msg::SystemCmdFloat>("pacmod/accel_cmd", 10);
    m_brake_pub_ = this->create_publisher<pacmod3_msgs::msg::SystemCmdFloat>("pacmod/brake_cmd", 10);
    m_steer_pub_ = this->create_publisher<pacmod3_msgs::msg::SteeringCmd>("pacmod/steering_cmd", 10);
    m_enable_pub_ = this->create_publisher<std_msgs::msg::Bool>("pacmod/enable", 10);
    m_status_string_pub_ = this->create_publisher<std_msgs::msg::String>("control_status", 10);
    RCLCPP_INFO(this->get_logger(), "actuator_control has been started");
}

void crp::vil::ActuatorControl::behaviorCallback(const crp_msgs::msg::Behavior msg)
{
    if (msg.acceleration_mode.data == 1)
    {
      m_maximum_acceleration = 2.0;
    }
    else if(msg.acceleration_mode.data == 2)
    {
      m_maximum_acceleration = 3.5;
    }
    else if(msg.acceleration_mode.data == 3)
    {
      m_maximum_acceleration = 5.0;
    }
    else{
      m_maximum_acceleration = 1.0;
    }

    if (msg.deceleration_mode.data == 1)
    {
      m_maximum_deceleration = -2.0;
    }
    else if(msg.deceleration_mode.data == 2)
    {
      m_maximum_deceleration = -3.5;
    }
    else if(msg.deceleration_mode.data == 3)
    {
      m_maximum_deceleration = -5.0;
    }
    else{
      m_maximum_deceleration = -1.0;
    }

    if (msg.jerk_mode.data == 1)
    {
      m_maximum_jerk = 0.75;
      m_minimum_jerk = -0.75;
    }
    else if(msg.jerk_mode.data == 2)
    {
      m_maximum_jerk = 1.5;
      m_minimum_jerk = -1.5;
    }
    else if(msg.jerk_mode.data == 3)
    {
      m_maximum_jerk = 3.0;
      m_minimum_jerk = -3.0;
    }
    else{
      m_maximum_jerk = 1.0;
      m_minimum_jerk = -1.0;
    }
}

void crp::vil::ActuatorControl::autonomReinitCallback(const std_msgs::msg::Bool msg)
{
    m_autonom_status_changed = msg.data;
    std::string a_status = msg.data ? "true" : "false";
    RCLCPP_INFO_STREAM(this->get_logger(), lx_namespace << "/control_reinit: " << a_status);
}

void crp::vil::ActuatorControl::egoCallback(const crp_msgs::msg::Ego msg)
{
    m_vehicle_speed_actual = msg.twist.twist.linear.x;
    m_currentSteeringTireAngle = msg.tire_angle_front;
}

void crp::vil::ActuatorControl::controlCmdCallback(const autoware_control_msgs::msg::Control msg)
{
    m_vehicle_speed_reference = msg.longitudinal.velocity;
    m_vehicle_steering_reference = msg.lateral.steering_tire_angle * 14.8;
}

void crp::vil::ActuatorControl::run()
{
    double current_time = (this->now()).seconds();
    
    m_steerCommandMsg.command = m_vehicle_steering_reference;
    double speed_diff = m_vehicle_speed_reference - m_vehicle_speed_actual;
    double brake_command_raw = 0.0;
    //  here a hysteresis is applied to avoid fluctuating behaviour at constant speed
    //  the bandwith of the hysteresis: e.g. 0.9 m/s (3.24 km/h)
    //  in this if statement control_state is in acceleration
    if ((speed_diff > -0.9 && m_control_state) || (speed_diff > 0.9))
    {
      m_control_state = true;
      m_statusStringMsg.data = "accel"; 
      // RCLCPP_INFO_STREAM(this->get_logger(), "accelerate");
      m_p_out_accel = speed_diff * m_p_gain_accel;
      if ((m_t_integ_accel + speed_diff * dt) <= m_max_i_accel)
      {
        m_t_integ_accel = m_t_integ_accel + speed_diff * dt;
      }
      m_i_out_accel = m_t_integ_accel * m_i_gain_accel;
      double t_derivative_accel = (speed_diff - m_speed_diff_prev) / dt;
      m_d_out_accel = m_d_gain_accel * t_derivative_accel;
      double accel_command_raw = m_p_out_accel + m_i_out_accel + m_d_out_accel; // P+I+D

      m_accelCommandMsg.command = accel_command_raw;
      m_brakeCommandMsg.command = 0.0;
      // acceleration upper limit
      m_accelCommandMsg.command = std::min(m_accelCommandMsg.command, m_maximum_acceleration);

      // gradient limit
      if ((m_accelCommandMsg.command - m_accel_command_prev) / dt > m_maximum_jerk && dt > 0.0)
      {
        m_accelCommandMsg.command = m_accel_command_prev + m_maximum_jerk * dt;
      }
      else if ((m_accelCommandMsg.command - m_accel_command_prev) / dt < m_minimum_jerk && dt > 0.0)
      {
        m_accelCommandMsg.command = m_accel_command_prev + m_minimum_jerk * dt;
      }
    }
    // hysteresis to avoid fluctuating behaviour at constant speeds
    // in this if statement control_state is in deceleration (brake)
    else if ((speed_diff < 0.9 && !m_control_state) || (speed_diff < -0.9))
    {
      m_control_state = false; // brake state
      m_statusStringMsg.data = "brake"; 
      // RCLCPP_INFO_STREAM(this->get_logger(), "brake");
      m_p_out_brake = speed_diff * m_p_gain_brake;
      // Standstill 1.38 m/s ~= 5.0 km/h
      if (m_vehicle_speed_reference < 1.38 && m_vehicle_speed_actual < 1.38){
        m_statusStringMsg.data = "standstill";  
        m_p_out_brake = m_p_out_brake * 1.2;
      }
      if ((m_t_integ_brake + speed_diff * dt) >= m_max_i_brake){
        m_t_integ_brake = m_t_integ_brake + speed_diff * dt;
      }   
      m_i_out_brake = m_t_integ_brake * m_i_gain_brake;
      m_t_derivative_brake = (speed_diff - m_speed_diff_prev) / dt;
      m_d_out_brake = m_d_gain_brake * m_t_derivative_brake;
      brake_command_raw = m_p_out_brake + m_i_out_brake + m_d_out_brake; // P+I+D

      // deceleration lower limit
      brake_command_raw= std::max(brake_command_raw, m_maximum_deceleration);

      // gradient limit
      if ((brake_command_raw - m_brake_command_prev) / dt < m_minimum_jerk && dt > 0.0)
      {
        brake_command_raw = m_brake_command_prev + m_minimum_jerk * dt;
      }

      // multiple -1.0 for positive number of brake command (sim. to pedal position)
      m_brakeCommandMsg.command = -1.0 * brake_command_raw;
      m_accelCommandMsg.command = 0.0;
      
      // Standstill 0.27 m/s ~= 1.0 km/h
      if (m_vehicle_speed_reference < 0.01 && m_vehicle_speed_actual < 0.27){
        m_statusStringMsg.data = "standstill1";  
        m_brakeCommandMsg.command = 0.3;
      }

    }
    m_steerCommandMsg.rotation_rate = 3.3;
    m_accelCommandMsg.header.frame_id = "pacmod";
    m_accelCommandMsg.enable = true;
    m_brakeCommandMsg.enable = true;
    m_steerCommandMsg.enable = true;
    if (m_autonom_status_changed)
    {
      m_accelCommandMsg.clear_override = true;
      m_brakeCommandMsg.clear_override = true;
      m_steerCommandMsg.clear_override = true;
      m_autonom_status_changed = false; // just once
    }
    else
    {
      m_accelCommandMsg.clear_override = false;
      m_brakeCommandMsg.clear_override = false;
      m_steerCommandMsg.clear_override = false;
    }

    if (!m_first_run)
    {
      dt = current_time - m_prev_time;
    }
    {
      dt = 0.033333;
    }
    if (m_first_run)
    {
      m_first_run = false;
      m_statusStringMsg.data = "longitudinal_control_started";
    }
    m_enableMsg.data = true;
    m_accel_pub_->publish(m_accelCommandMsg);
    m_brake_pub_->publish(m_brakeCommandMsg);
    m_steer_pub_->publish(m_steerCommandMsg);
    m_enable_pub_->publish(m_enableMsg);
    //status_string_msg.data = control_state ? "accel" : "brake";
    if (m_statusStringMsg.data.compare(""))
      m_status_string_pub_->publish(m_statusStringMsg);
    m_statusStringMsg.data = "";
    m_prev_time = current_time;
    m_accel_command_prev = m_accelCommandMsg.command;
    m_brake_command_prev = brake_command_raw;
    m_speed_diff_prev = speed_diff;
}


int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::vil::ActuatorControl>());
    rclcpp::shutdown();
    return 0;
}