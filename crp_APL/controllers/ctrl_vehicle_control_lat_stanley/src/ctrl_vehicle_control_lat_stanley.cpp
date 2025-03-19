#include <ctrl_vehicle_control_lat_stanley/ctrl_vehicle_control_lat_stanley.hpp>


crp::apl::CtrlVehicleControlLatStanley::CtrlVehicleControlLatStanley() : CtrlLatWrapperBase("ctrl_vehicle_control_lat_stanley")
{
    this->declare_parameter("/ctrl/stanley/k_gain", 0.5f);
    this->declare_parameter("/vehicle_params/wheelbase", 2.7f);

    RCLCPP_INFO(this->get_logger(), "ctrl_vehicle_control_lat_stanley has been started");
}

void crp::apl::CtrlVehicleControlLatStanley::error_calculation(const ControlInput & input, double &lateralError, double &headingError)
{
    float target_x = m_params.wheelbase;
    float target_y = 0;
    int ind = 0;

    float dist = std::numeric_limits<float>::max();

    for(unsigned int i = 0; i < input.pathX.size(); i++)
    {
        float ds = std::sqrt(std::pow(input.pathX[i] - target_x, 2) + std::pow(input.pathY[i] - target_y, 2));
        
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

    double dx = input.pathX[ind];
    double dy = input.pathY[ind];

    // Dot product calculation
    lateralError = dx * frontAxleVec[0] + dy * frontAxleVec[1];

    // calculate the heading error
    headingError = atan2(input.pathY[ind], input.pathX[ind]);
}

void crp::apl::CtrlVehicleControlLatStanley::stanleyControl(const ControlInput & input, LatControlOutput & output)
{
    // implement the stanley control algorithm

    // calculate the steering angle
    output.steeringAngleTarget = 0.0f;

    double frontAxleError = 0.0f;
    double theta_e = 0.0f;

    error_calculation(input, frontAxleError, theta_e);

    float theta_d = atan2(m_params.k_gain * frontAxleError, input.vxEgo);

    output.steeringAngleTarget = theta_e + theta_d;

}

void crp::apl::CtrlVehicleControlLatStanley::controlLoop(const ControlInput & input, LatControlOutput & output)
{
    // parameter assignments
    m_params.k_gain= this->get_parameter("/ctrl/stanley/k_gain").as_double();
    m_params.wheelbase = this->get_parameter("/vehicle_params/wheelbase").as_double();

    // control algorithm
    if(input.pathX.size() > 0 && input.pathY.size() > 0)
    {
        stanleyControl(input, output);
    }
}


int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::apl::CtrlVehicleControlLatStanley>());
    rclcpp::shutdown();
    return 0;
}