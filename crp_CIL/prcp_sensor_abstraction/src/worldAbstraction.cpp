#include "prcp_sensor_abstraction/worldAbstraction.hpp"


crp::cil::WorldAbstraction::WorldAbstraction() : Node("world_abstraction")
{
    m_pub_static_map_     = this->create_publisher<autoware_map_msgs::msg::LaneletMapBin>("cai/global_static_map/lanelet2_map", 10);
    m_pub_moving_objects_ = this->create_publisher<autoware_map_msgs::msg::LaneletMapBin>("cai/global_moving_objects", 10);
}


int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::cil::WorldAbstraction>());
    rclcpp::shutdown();
    return 0;
}
