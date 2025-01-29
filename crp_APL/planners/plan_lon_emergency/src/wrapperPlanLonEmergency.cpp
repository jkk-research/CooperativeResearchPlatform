#include "plan_lon_emergency/wrapperPlanLonEmergency.hpp"


crp::apl::WrapperPlanLonEmergency::WrapperPlanLonEmergency() : WrapperBase("plan_lon_emergency")
{
    RCLCPP_INFO(this->get_logger(), "plan_lon_emergency has been started");
}


void crp::apl::WrapperPlanLonEmergency::plan(const PlannerInput & input, PlannerOutput & output)
{
    // TODO
    return;
}


int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::apl::WrapperPlanLonEmergency>());
    rclcpp::shutdown();
    return 0;
}
