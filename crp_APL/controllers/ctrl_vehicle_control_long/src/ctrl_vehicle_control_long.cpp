#include "ctrl_vehicle_control_long.hpp"


crp::apl::CtrlVehicleControlLong::CtrlVehicleControlLong() : CtrlLongWrapperBase("ctrl_vehicle_control_long")
{
    this->declare_parameter("/ctrl/long/speedControlLookAheadTime", 0.6f);

    RCLCPP_INFO(this->get_logger(), "ctrl_vehicle_control_long has been started");
}

void crp::apl::CtrlVehicleControlLong::controlLoop(const ControlInput & input, LongControlOutput & output)
{
    if (input.pathVelocity.size() == 0U)
    {
        output.velocityTarget = 0.0;
        return;
    }

    // parameter read in
    double p_speedControlLookAheadTime = this->get_parameter("/ctrl/long/speedControlLookAheadTime").as_double();
    double dT = 0.033;


    double lookAheadDistance = input.vxEgo*p_speedControlLookAheadTime; // in meters

    output.velocityTarget = input.pathVelocity.at(input.pathVelocity.size()-1U);
    
    for (long unsigned int wp=0U; wp<input.pathX.size(); wp++)
    {
        double distance = sqrt(std::pow(input.pathX.at(wp), 2) +
            std::pow(input.pathY.at(wp), 2));
        
        if(distance >= lookAheadDistance)
        {
            output.velocityTarget = input.pathVelocity.at(wp);
            break;
        }
    }
}    

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::apl::CtrlVehicleControlLong>());
    rclcpp::shutdown();
    return 0;
}