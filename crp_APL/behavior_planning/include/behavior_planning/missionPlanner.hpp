#ifndef CRP_APL_MISSIONPLANNER_HPP
#define CRP_APL_MISSIONPLANNER_HPP

#include <rclcpp/rclcpp.hpp>
#include <crp_msgs/msg/world.hpp>
#include <autoware_planning_msgs/msg/lanelet_route.hpp>

class MissionPlanner : public rclcpp::Node
{
public:
    MissionPlanner();

private:
    void worldCallback(const crp_msgs::msg::World::SharedPtr msg);

    rclcpp::Subscription<crp_msgs::msg::World>::SharedPtr m_sub_world;
    rclcpp::Publisher<autoware_planning_msgs::msg::LaneletRoute>::SharedPtr m_pub_route;
};

#endif //CRP_APL_MISSIONPLANNER_HPP