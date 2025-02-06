#include <ctrl_vehicle_control_lat_compensatory/ctrl_vehicle_control_lat_compensatory.hpp>


crp::apl::CtrlVehicleControlLatCompensatory::CtrlVehicleControlLatCompensatory() : Node("ctrl_vehicle_control_lat_compensatory")
{
    m_timer_ = this->create_wall_timer(std::chrono::milliseconds(33), std::bind(&CtrlVehicleControlLatCompensatory::loop, this));  
    m_pub_cmd = this->create_publisher<autoware_control_msgs::msg::Lateral>("/control/command/control_cmdLat", 30);

    m_sub_traj_ = this->create_subscription<autoware_planning_msgs::msg::Trajectory>("/plan/trajectory", 10, std::bind(&CtrlVehicleControlLatCompensatory::trajCallback, this, std::placeholders::_1));
    m_sub_egoVehicle_ = this->create_subscription<crp_msgs::msg::Ego>("/ego", 10, std::bind(&CtrlVehicleControlLatCompensatory::egoVehicleCallback, this, std::placeholders::_1));

    this->declare_parameter("/ctrl/compensatory/ffLookAheadTime", 0.68f);
    this->declare_parameter("/ctrl/compensatory/steeringAngleLPFilter", 0.2f);
    this->declare_parameter("/ctrl/compensatory/fbLookAheadTime", 0.0f);
    this->declare_parameter("/ctrl/compensatory/fbPGain", 0.8f);
    this->declare_parameter("/ctrl/compensatory/fbDGain", 1.1f);
    this->declare_parameter("/ctrl/compensatory/fbIGain", 0.0f);
    this->declare_parameter("/ctrl/compensatory/fbIntegralLimit", 3.0f);
    this->declare_parameter("/ctrl/compensatory/trajectory_distance", 50.0f);

    this->declare_parameter("/ctrl/compensatory/sigma_thetaFP", 0.25f);
    this->declare_parameter("/ctrl/compensatory/maxThetaFP", 0.3f);
    this->declare_parameter("/ctrl/compensatory/targetAccelerationFF_lpFilterCoeff", 0.99f);
    this->declare_parameter("/ctrl/compensatory/targetAccelerationFB_lpFilterCoeff", 0.99f);
        
    RCLCPP_INFO(this->get_logger(), "ctrl_vehicle_control_lat_compensatory has been started");
}

void crp::apl::CtrlVehicleControlLatCompensatory::trajCallback(const autoware_planning_msgs::msg::Trajectory input_msg)
{
    m_input.path_x.clear();
    m_input.path_y.clear();
    m_input.path_theta.clear();
    double quaternion[4];

    // this callback maps the input trajectory to the internal interface
    for (long unsigned int i=0; i<input_msg.points.size(); i++)
    {      
        m_input.path_x.push_back(input_msg.points.at(i).pose.position.x);
        m_input.path_y.push_back(input_msg.points.at(i).pose.position.y);
        quaternion[0] = input_msg.points.at(i).pose.orientation.x; 
        quaternion[1] = input_msg.points.at(i).pose.orientation.y; 
        quaternion[2] = input_msg.points.at(i).pose.orientation.z; 
        quaternion[3] = input_msg.points.at(i).pose.orientation.w; 
        m_input.path_theta.push_back(m_geometricOperator.transformQuatToEuler(quaternion));
    }

    if (input_msg.points.size() > 0)
        m_input.targetSpeed = input_msg.points.at(0).longitudinal_velocity_mps;
    else
        m_input.targetSpeed = 0.0f;
}

void crp::apl::CtrlVehicleControlLatCompensatory::egoVehicleCallback(const crp_msgs::msg::Ego input_msg)
{
    m_input.vxEgo = input_msg.twist.twist.linear.x;
    m_input.egoSteeringAngle = input_msg.tire_angle_front;

    m_input.egoPoseGlobal[0] = input_msg.pose.pose.position.x;
    m_input.egoPoseGlobal[1] = input_msg.pose.pose.position.y;
    double quaternion[4];
    quaternion[0] = input_msg.pose.pose.orientation.x; 
    quaternion[1] = input_msg.pose.pose.orientation.y; 
    quaternion[2] = input_msg.pose.pose.orientation.z; 
    quaternion[3] = input_msg.pose.pose.orientation.w; 
    double theta = m_geometricOperator.transformQuatToEuler(quaternion);
    m_input.egoPoseGlobal[2] = theta;
}

void crp::apl::CtrlVehicleControlLatCompensatory::loop()
{
    // parameter assignments
    m_params.ffLookAheadTime       = this->get_parameter("/ctrl/compensatory/ffLookAheadTime").as_double();
    m_params.steeringAngleLPFilter = this->get_parameter("/ctrl/compensatory/steeringAngleLPFilter").as_double();
    m_params.fbLookAheadTime       = this->get_parameter("/ctrl/compensatory/fbLookAheadTime").as_double();
    m_params.fbPGain               = this->get_parameter("/ctrl/compensatory/fbPGain").as_double();
    m_params.fbDGain               = this->get_parameter("/ctrl/compensatory/fbDGain").as_double();
    m_params.fbIGain               = this->get_parameter("/ctrl/compensatory/fbIGain").as_double();
    m_params.fbIntegralLimit       = this->get_parameter("/ctrl/compensatory/fbIntegralLimit").as_double();
    m_params.trajectory_distance   = this->get_parameter("/ctrl/compensatory/trajectory_distance").as_double();

    m_params.sigma_thetaFP                      = this->get_parameter("/ctrl/compensatory/sigma_thetaFP").as_double();
    m_params.maxThetaFP                         = this->get_parameter("/ctrl/compensatory/maxThetaFP").as_double();
    m_params.targetAccelerationFF_lpFilterCoeff = this->get_parameter("/ctrl/compensatory/targetAccelerationFF_lpFilterCoeff").as_double();
    m_params.targetAccelerationFB_lpFilterCoeff = this->get_parameter("/ctrl/compensatory/targetAccelerationFB_lpFilterCoeff").as_double();

    m_compensatoryModel.run(m_input, m_output, m_params);

    // steering angle and steering angle gradiant
    m_ctrlCmdMsg.stamp = this->now();
    m_ctrlCmdMsg.steering_tire_angle = m_output.steeringAngleTarget;
    m_ctrlCmdMsg.steering_tire_rotation_rate = 0.0f;

    m_pub_cmd->publish(m_ctrlCmdMsg);
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::apl::CtrlVehicleControlLatCompensatory>());
    rclcpp::shutdown();
    return 0;
}