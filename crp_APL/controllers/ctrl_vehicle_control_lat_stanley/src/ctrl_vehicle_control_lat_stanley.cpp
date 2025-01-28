#include <ctrl_vehicle_control_lat_stanley/ctrl_vehicle_control_lat_stanley.hpp>


crp::apl::CtrlVehicleControlLatStanley::CtrlVehicleControlLatStanley() : Node("CtrlVehicleControlLatStanley")
{
    m_timer_ = this->create_wall_timer(std::chrono::milliseconds(33), std::bind(&CtrlVehicleControlLatStanley::loop, this));  
    m_pub_cmd_ = this->create_publisher<autoware_control_msgs::msg::Lateral>("/control/command/control_cmdLat", 30);

    m_sub_traj_ = this->create_subscription<autoware_planning_msgs::msg::Trajectory>("/plan/trajectory", 10, std::bind(&CtrlVehicleControlLatStanley::trajCallback, this, std::placeholders::_1));
    m_sub_egoVehicle_ = this->create_subscription<crp_msgs::msg::Ego>("/ego", 10, std::bind(&CtrlVehicleControlLatStanley::egoVehicleCallback, this, std::placeholders::_1));

    this->declare_parameter("/ctrl/stanley/k_gain", 0.5f);
    this->declare_parameter("/ctrl/stanley/wheelbase", 2.7f);

    RCLCPP_INFO(this->get_logger(), "CtrlVehicleControlLatStanley has been started");
}

void crp::apl::CtrlVehicleControlLatStanley::trajCallback(const autoware_planning_msgs::msg::Trajectory input_msg)
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

void crp::apl::CtrlVehicleControlLatStanley::egoVehicleCallback(const crp_msgs::msg::Ego input_msg)
{
    m_input.vxEgo = input_msg.twist.twist.linear.x;
    m_input.egoSteeringAngle = input_msg.tire_angle_front;

    m_input.egoPoseGlobal[0] = input_msg.pose.pose.position.x;
    m_input.egoPoseGlobal[1] = input_msg.pose.pose.position.y;
}

void crp::apl::CtrlVehicleControlLatStanley::error_calculation(double &lateralError, double &headingError)
{
    float target_x = m_params.wheelbase;
    float target_y = 0;
    int ind = 0;

    float dist = std::numeric_limits<float>::max();

    for(int i = 0; i < m_input.m_path_x.size(); i++)
    {
        float ds = std::sqrt(std::pow(m_input.m_path_x[i] - target_x, 2) + std::pow(m_input.m_path_y[i] - target_y, 2));
        
        if(dist > ds)
        {
            ind = i;
            dist = ds;
        }
    }

    std::vector<double> frontAxleVec = {
        -std::cos(M_PI / 2),  // Use std::cos for cosine
        -std::sin(M_PI / 2)   // Use std::sin for sine
    };

    double dx = m_input.m_path_x[ind];
    double dy = m_input.m_path_y[ind];

    // Dot product calculation
    lateralError = dx * frontAxleVec[0] + dy * frontAxleVec[1];

    // calculate the heading error
    headingError = atan2(m_input.m_path_y[ind], m_input.m_path_x[ind]);

}


void crp::apl::CtrlVehicleControlLatStanley::stanleyControl()
{
    // implement the stanley control algorithm

    // calculate the steering angle
    m_output.steeringAngleTarget = 0.0f;

    double frontAxleError = 0.0f;
    double theta_e = 0.0f;

    error_calculation(frontAxleError, theta_e);

    float theta_d = atan2(m_params.k_gain * frontAxleError, m_input.vxEgo);

    m_output.steeringAngleTarget = theta_e + theta_d;

}

void crp::apl::CtrlVehicleControlLatStanley::loop()
{
    // parameter assignments
    m_params.k_gain= this->get_parameter("/ctrl/stanley/k_gain").as_double();
    m_params.wheelbase = this->get_parameter("/ctrl/stanley/wheelbase").as_double();

    // control algorithm
    if(m_input.m_path_x.size() > 0 && m_input.m_path_y.size() > 0)
    {
        stanleyControl();

        // steering angle and steering angle gradiant
        m_ctrlCmdMsg.stamp = this->now();
        m_ctrlCmdMsg.steering_tire_angle = m_output.steeringAngleTarget;
        m_ctrlCmdMsg.steering_tire_rotation_rate = 0.0f;

        m_pub_cmd_->publish(m_ctrlCmdMsg);
    }
}


int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::apl::CtrlVehicleControlLatStanley>());
    rclcpp::shutdown();
    return 0;
}