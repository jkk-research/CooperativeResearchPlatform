#include "ctrl_vehicle_control_long.hpp"


crp::apl::CtrlVehicleControlLong::CtrlVehicleControlLong() : CtrlLongWrapperBase("ctrl_vehicle_control_long")
{
    this->declare_parameter("/ctrl/long/axMax", 2.0f);
    this->declare_parameter("/ctrl/long/axMin", -2.0f);
    this->declare_parameter("/ctrl/long/jxMax", 1.0f);
    this->declare_parameter("/ctrl/long/jxMin", -1.0f);
    this->declare_parameter("/ctrl/long/speedControlLookAheadTime", 0.6f);

    RCLCPP_INFO(this->get_logger(), "ctrl_vehicle_control_long has been started");
}

void crp::apl::CtrlVehicleControlLong::controlLoop(const ControlInput & input, LongControlOutput & output)
{
    // parameter read in
    double p_axMax = this->get_parameter("/ctrl/long/axMax").as_double();
    double p_axMin = this->get_parameter("/ctrl/long/axMin").as_double();
    double p_jxMax = this->get_parameter("/ctrl/long/jxMax").as_double();
    double p_jxMin = this->get_parameter("/ctrl/long/jxMin").as_double();
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
            // filtering with acceleration
            if(input.pathVelocity.at(wp) >= (input.vxEgo+p_axMax*dT))
            {
                output.velocityTarget = input.vxEgo+p_axMax*dT;  
            }
            else if(input.pathVelocity.at(wp) <= (input.vxEgo+p_axMin*dT))
            {
                if (input.vxEgo+p_axMin*dT < 2.0)
                {
                    output.velocityTarget = 0.0;
                }
                else
                {
                    output.velocityTarget = input.vxEgo+p_axMin*dT;
                }
            }
            else
            {
                output.velocityTarget = input.pathVelocity.at(wp);
            }
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