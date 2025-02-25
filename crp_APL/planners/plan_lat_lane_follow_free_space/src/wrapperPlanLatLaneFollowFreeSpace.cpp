#include "plan_lat_lane_follow/wrapperPlanLatLaneFollowFreeSpace.hpp"


crp::apl::WrapperPlanLatLaneFollowFreeSpace::WrapperPlanLatLaneFollowFreeSpace() : WrapperBase("plan_lat_lane_follow")
{
}


void crp::apl::WrapperPlanLatLaneFollowFreeSpace::plan(const PlannerInput & input, PlannerOutput & output)
{
    // TODO
}


int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::apl::WrapperPlanLatLaneFollowFreeSpace>());
    rclcpp::shutdown();
    return 0;
}
