#include "plan_lon_intelligent_speed_adjust/handler.hpp"


crp::apl::PlanLonIntelligentSpeedAdjustHandler::PlanLonIntelligentSpeedAdjustHandler() : HandlerBase("plan_lon_intelligent_speed_adjust")
{
}


void crp::apl::PlanLonIntelligentSpeedAdjustHandler::plan(const PlannerInput & input, PlannerOutput & output)
{
    // TODO
    return;
}


int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::apl::PlanLonIntelligentSpeedAdjustHandler>());
    rclcpp::shutdown();
    return 0;
}
