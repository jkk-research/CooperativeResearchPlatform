#include <ctrl_vehicle_control_lat_pure_p/ctrl_vehicle_control_lat_pure_p.hpp>


crp::apl::CtrlVehicleControlLatPureP::CtrlVehicleControlLatPureP() : Node("ctrl_vehicle_control_lat_pure_pursuit")
{
    m_timer_ = this->create_wall_timer(std::chrono::milliseconds(33), std::bind(&CtrlVehicleControlLatPureP::loop, this));  
    m_pub_cmd = this->create_publisher<autoware_control_msgs::msg::Lateral>("/control/command/control_cmdLat", 30);

    m_sub_traj_ = this->create_subscription<autoware_planning_msgs::msg::Trajectory>("/plan/trajectory", 10, std::bind(&CtrlVehicleControlLatPureP::trajCallback, this, std::placeholders::_1));
    m_sub_egoVehicle_ = this->create_subscription<crp_msgs::msg::Ego>("/ego", 10, std::bind(&CtrlVehicleControlLatPureP::egoVehicleCallback, this, std::placeholders::_1));

    this->declare_parameter("/ctrl/pure_p/lookahead_time", 1.0f);
    this->declare_parameter("/ctrl/pure_p/wheelbase", 2.7f);

    RCLCPP_INFO(this->get_logger(), "CtrlVehicleControlLatPurePursuit has been started");
}

void crp::apl::CtrlVehicleControlLatPureP::trajCallback(const autoware_planning_msgs::msg::Trajectory input_msg)
{
    m_input.m_path_x.clear();
    m_input.m_path_y.clear();

    // this callback maps the input trajectory to the internal interface
    for (long unsigned int i=0; i<input_msg.points.size(); i++)
    {
        m_input.m_path_x.push_back(input_msg.points.at(i).pose.position.x);
        m_input.m_path_y.push_back(input_msg.points.at(i).pose.position.y);
    }

    if (input_msg.points.size() > 0)
        m_input.targetSpeed = input_msg.points.at(0).longitudinal_velocity_mps;
    else
        m_input.targetSpeed = 0.0f;
}

void crp::apl::CtrlVehicleControlLatPureP::egoVehicleCallback(const crp_msgs::msg::Ego input_msg)
{
    m_input.vxEgo = input_msg.twist.twist.linear.x;
    m_input.egoSteeringAngle = input_msg.tire_angle_front;

    m_input.egoPoseGlobal[0] = input_msg.pose.pose.position.x;
    m_input.egoPoseGlobal[1] = input_msg.pose.pose.position.y;
}

void crp::apl::CtrlVehicleControlLatPureP::pure_p_control()
{
    float distance_to_index = std::max(m_input.vxEgo * m_params.lookaheadTime, 2.0);

    float currentDistance = 0.0f;
    int target_index = 0;

    for (int i = 0; i < m_input.m_path_x.size()-1; i++)
    {
        currentDistance += sqrt(pow(m_input.m_path_x.at(i+1) - m_input.m_path_x.at(i), 2) + pow(m_input.m_path_y.at(i+1) - m_input.m_path_y.at(i), 2));
        if (currentDistance >= distance_to_index)
        {
            target_index = i;
            break;
        }
    }

    // calculate the steering angle

    float alpha = atan2(m_input.m_path_y[target_index], m_input.m_path_x[target_index]);

    m_output.steeringAngleTarget = atan2(2.0f * m_params.wheelbase * sin(alpha) / distance_to_index, 1);
}

void crp::apl::CtrlVehicleControlLatPureP::loop()
{
    // parameter assignments
    m_params.lookaheadTime = this->get_parameter("/ctrl/pure_p/lookahead_time").as_double();
    m_params.wheelbase = this->get_parameter("/ctrl/pure_p/wheelbase").as_double();

    // control algorithm
    if(m_input.m_path_x.size() > 0 && m_input.m_path_y.size() > 0)
    {
        pure_p_control();
        // steering angle and steering angle gradiant
        m_ctrlCmdMsg.stamp = this->now();
        m_ctrlCmdMsg.steering_tire_angle = m_output.steeringAngleTarget;
        m_ctrlCmdMsg.steering_tire_rotation_rate = 0.0f;

        m_pub_cmd->publish(m_ctrlCmdMsg);
    }
}


int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::apl::CtrlVehicleControlLatPureP>());
    rclcpp::shutdown();
    return 0;
}