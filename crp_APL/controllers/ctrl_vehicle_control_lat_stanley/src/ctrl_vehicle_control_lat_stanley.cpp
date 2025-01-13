#include <ctrl_vehicle_control_lat_stanley/ctrl_vehicle_control_lat_stanley.hpp>

using namespace std::chrono_literals;
using std::placeholders::_1;


crp::apl::CtrlVehicleControlLat::CtrlVehicleControlLat() : Node("CtrlVehicleControlLatStanley")
{
    timer_ = this->create_wall_timer(std::chrono::milliseconds(33), std::bind(&CtrlVehicleControlLat::loop, this));  
    m_pub_cmd_ = this->create_publisher<autoware_control_msgs::msg::Lateral>("/control/command/control_cmdLat", 30);

    m_traj_sub_ = this->create_subscription<autoware_planning_msgs::msg::Trajectory>("/plan/trajectory", 10, std::bind(&CtrlVehicleControlLat::trajCallback, this, std::placeholders::_1));
    m_egoVehicle_sub_ = this->create_subscription<crp_msgs::msg::Ego>("/ego", 10, std::bind(&CtrlVehicleControlLat::egoVehicleCallback, this, std::placeholders::_1));

    this->declare_parameter("/ctrl/k_gain", 0.5f);
    this->declare_parameter("/ctrl/wheelbase", 2.7f);

    RCLCPP_INFO(this->get_logger(), "ctrl_vehicle_control has been started");
}

void crp::apl::CtrlVehicleControlLat::trajCallback(const autoware_planning_msgs::msg::Trajectory input_msg)
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
        m_input.m_target_speed = input_msg.points.at(0).longitudinal_velocity_mps;
    else
        m_input.m_target_speed = 0.0f;

}

void crp::apl::CtrlVehicleControlLat::egoVehicleCallback(const crp_msgs::msg::Ego input_msg)
{
    m_input.m_vxEgo = input_msg.twist.twist.linear.x;
    m_input.m_egoSteeringAngle = input_msg.tire_angle_front;

    m_input.m_egoPoseGlobal[0] = input_msg.pose.pose.position.x;
    m_input.m_egoPoseGlobal[1] = input_msg.pose.pose.position.y;
}

void crp::apl::CtrlVehicleControlLat::error_calculation(double &lateral_error, double &heading_error)
{
    // calculate the front axle error
    float front_axle_error = 0.0f;

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

    std::vector<double> front_axle_vec = {
        -std::cos(M_PI / 2),  // Use std::cos for cosine
        -std::sin(M_PI / 2)   // Use std::sin for sine
    };

    double dx = m_input.m_path_x[ind];
    double dy = m_input.m_path_y[ind];

    // Dot product calculation
    lateral_error = dx * front_axle_vec[0] + dy * front_axle_vec[1];

    // calculate the heading error
    heading_error = atan2(m_input.m_path_y[ind], m_input.m_path_x[ind]);

}


void crp::apl::CtrlVehicleControlLat::stanleyControl()
{
    // implement the stanley control algorithm

    // calculate the steering angle
    m_output.m_steeringAngleTarget = 0.0f;

    double front_axle_error = 0.0;
    double theta_e = 0.0;

    error_calculation(front_axle_error, theta_e);

    float theta_d = atan2(m_params.K_GAIN * front_axle_error, m_input.m_vxEgo);

    m_output.m_steeringAngleTarget = theta_e + theta_d;

}

void crp::apl::CtrlVehicleControlLat::loop()
{
    // parameter assignments
    m_params.K_GAIN= this->get_parameter("/ctrl/k_gain").as_double();
    m_params.wheelbase = this->get_parameter("/ctrl/wheelbase").as_double();

    // control algorithm
    if(m_input.m_path_x.size() > 0 && m_input.m_path_y.size() > 0)
    {
        stanleyControl();

        // steering angle and steering angle gradiant
        m_ctrlCmdMsg.stamp = this->now();
        m_ctrlCmdMsg.steering_tire_angle = m_output.m_steeringAngleTarget;
        m_ctrlCmdMsg.steering_tire_rotation_rate = 0.0f;

        m_pub_cmd_->publish(m_ctrlCmdMsg);
    }
}


int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::apl::CtrlVehicleControlLat>());
    rclcpp::shutdown();
    return 0;
}