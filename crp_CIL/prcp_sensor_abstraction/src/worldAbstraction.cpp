#include "prcp_sensor_abstraction/worldAbstraction.hpp"


crp::cil::WorldAbstraction::WorldAbstraction() : Node("world_abstraction")
{
    m_sub_staticMapFromFile_ = this->create_subscription<autoware_map_msgs::msg::LaneletMapBin>(
        "map/global_static_map_from_file/lanelet2_map", 10, std::bind(&WorldAbstraction::staticMapFromFileCallback, this, std::placeholders::_1));
    m_sub_staticMapFromServer_ = this->create_subscription<autoware_map_msgs::msg::LaneletMapBin>(
        "map/global_static_map_from_server/lanelet2_map", 10, std::bind(&WorldAbstraction::staticMapFromServerCallback, this, std::placeholders::_1));

    m_pub_staticMap_     = this->create_publisher<autoware_map_msgs::msg::LaneletMapBin>("cai/global_static_map/lanelet2_map", 10);
    m_pub_movingObjects_ = this->create_publisher<autoware_map_msgs::msg::LaneletMapBin>("cai/global_moving_objects", 10);
}


void crp::cil::WorldAbstraction::staticMapFromFileCallback(const autoware_map_msgs::msg::LaneletMapBin::SharedPtr msg)
{
    m_pub_staticMap_->publish(*msg);
}


void crp::cil::WorldAbstraction::staticMapFromServerCallback(const autoware_map_msgs::msg::LaneletMapBin::SharedPtr msg)
{
    // TODO
    return;
}


int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::cil::WorldAbstraction>());
    rclcpp::shutdown();
    return 0;
}
