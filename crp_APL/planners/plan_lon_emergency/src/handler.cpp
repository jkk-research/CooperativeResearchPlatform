#include "plan_lon_emergency/handler.hpp"


crp::apl::PlanLonEmergencyHandler::PlanLonEmergencyHandler() : HandlerBase("plan_lon_emergency")
{
}


void crp::apl::PlanLonEmergencyHandler::plan(const PlannerInput & input, PlannerOutput & output)
{
    // TODO
    return;
}


int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::apl::PlanLonEmergencyHandler>());
    rclcpp::shutdown();
    return 0;
}
