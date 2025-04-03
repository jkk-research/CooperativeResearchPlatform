#include <ctrl_vehicle_control_lat_pure_p/ctrl_vehicle_control_lat_pure_p.hpp>


crp::apl::CtrlVehicleControlLatPureP::CtrlVehicleControlLatPureP() : CtrlLatWrapperBase("ctrl_vehicle_control_lat_pure_pursuit")
{
    this->declare_parameter("/ctrl/pure_p/lookahead_time", 1.0f);
    this->declare_parameter("/vehicle_params/wheelbase", 2.7f);

    RCLCPP_INFO(this->get_logger(), "ctrl_vehicle_control_lat_pure_pursuit has been started");
}

void crp::apl::CtrlVehicleControlLatPureP::pure_p_control(const ControlInput & input, LatControlOutput & output)
{
    float distance_to_index = std::max(input.vxEgo * m_params.lookaheadTime, 2.0);

    float currentDistance = 0.0f;
    int target_index = 0;

    for (unsigned int i = 0; i < input.pathX.size()-1; i++)
    {
        currentDistance += sqrt(pow(input.pathX.at(i+1) - input.pathX.at(i), 2) + pow(input.pathY.at(i+1) - input.pathY.at(i), 2));
        if (currentDistance >= distance_to_index)
        {
            target_index = i;
            break;
        }
    }

    // calculate the steering angle

    float alpha = atan2(input.pathY[target_index], input.pathX[target_index]);

    output.steeringAngleTarget = atan2(2.0f * m_params.wheelbase * sin(alpha) / distance_to_index, 1);
}

void crp::apl::CtrlVehicleControlLatPureP::controlLoop(const ControlInput & input, LatControlOutput & output)
{
    // parameter assignments
    m_params.lookaheadTime = this->get_parameter("/ctrl/pure_p/lookahead_time").as_double();
    m_params.wheelbase = this->get_parameter("/vehicle_params/wheelbase").as_double();

    // control algorithm
    if(input.pathX.size() > 0 && input.pathY.size() > 0)
    {
        pure_p_control(input, output);
    }
}


int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::apl::CtrlVehicleControlLatPureP>());
    rclcpp::shutdown();
    return 0;
}
