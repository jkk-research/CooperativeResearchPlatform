#include "plan_lat_lane_follow/handler.hpp"


crp::apl::PlanLatLaneFollowHandler::PlanLatLaneFollowHandler() : HandlerBase("plan_lat_lane_follow")
{
}


void crp::apl::PlanLatLaneFollowHandler::plan(const PlannerInput & input, PlannerOutput & output)
{
    // TODO
}


int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::apl::PlanLatLaneFollowHandler>());
    rclcpp::shutdown();
    return 0;
}
