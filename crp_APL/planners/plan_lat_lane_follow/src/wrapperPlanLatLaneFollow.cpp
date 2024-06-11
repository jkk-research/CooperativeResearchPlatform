#include "plan_lat_lane_follow/wrapperPlanLatLaneFollow.hpp"


crp::apl::WrapperPlanLatLaneFollow::WrapperPlanLatLaneFollow() : WrapperBase("plan_lat_lane_follow")
{
}


void crp::apl::WrapperPlanLatLaneFollow::plan(const PlannerInput & input, PlannerOutput & output)
{
    // TODO
}


int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::apl::WrapperPlanLatLaneFollow>());
    rclcpp::shutdown();
    return 0;
}
