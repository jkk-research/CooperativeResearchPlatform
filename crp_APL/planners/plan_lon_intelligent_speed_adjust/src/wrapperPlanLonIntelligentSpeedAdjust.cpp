#include "plan_lon_intelligent_speed_adjust/wrapperPlanLonIntelligentSpeedAdjust.hpp"


crp::apl::WrapperPlanLonIntelligentSpeedAdjust::WrapperPlanLonIntelligentSpeedAdjust() : PlanWrapperBase("plan_lon_intelligent_speed_adjust")
{
    RCLCPP_INFO(this->get_logger(), "plan_lon_intelligent_speed_adjust has been started");
}


void crp::apl::WrapperPlanLonIntelligentSpeedAdjust::planLoop(const PlannerInput & input, PlannerOutput & output)
{
    m_planLongitudinalTrajectory.run(input, output);
    return;
}


int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::apl::WrapperPlanLonIntelligentSpeedAdjust>());
    rclcpp::shutdown();
    return 0;
}
