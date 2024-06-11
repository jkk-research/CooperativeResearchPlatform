#include "plan_lon_intelligent_speed_adjust/wrapperPlanLonIntelligentSpeedAdjust.hpp"


crp::apl::WrapperPlanLonIntelligentSpeedAdjust::WrapperPlanLonIntelligentSpeedAdjust() : WrapperBase("plan_lon_intelligent_speed_adjust")
{
}


void crp::apl::WrapperPlanLonIntelligentSpeedAdjust::plan(const PlannerInput & input, PlannerOutput & output)
{
    // TODO
    return;
}


int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::apl::WrapperPlanLonIntelligentSpeedAdjust>());
    rclcpp::shutdown();
    return 0;
}
