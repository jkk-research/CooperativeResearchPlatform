#include "plan_lon_emergency/wrapper.hpp"


crp::apl::PlanLonEmergencyWrapper::PlanLonEmergencyWrapper() : WrapperBase("plan_lon_emergency")
{
}


void crp::apl::PlanLonEmergencyWrapper::plan(const PlannerInput & input, PlannerOutput & output)
{
    // TODO
    return;
}


int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::apl::PlanLonEmergencyWrapper>());
    rclcpp::shutdown();
    return 0;
}
