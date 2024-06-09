#include "plan_lon_intelligent_speed_adjust/wrapper.hpp"


crp::apl::PlanLonIntelligentSpeedAdjustWrapper::PlanLonIntelligentSpeedAdjustWrapper() : WrapperBase("plan_lon_intelligent_speed_adjust")
{
}


void crp::apl::PlanLonIntelligentSpeedAdjustWrapper::plan(const PlannerInput & input, PlannerOutput & output)
{
    // TODO
    return;
}


int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::apl::PlanLonIntelligentSpeedAdjustWrapper>());
    rclcpp::shutdown();
    return 0;
}
