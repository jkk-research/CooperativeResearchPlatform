#ifndef CRP_CIL_WORLD_ABSTRACTION_SCENARIOABSTRACTION_HPP
#define CRP_CIL_WORLD_ABSTRACTION_SCENARIOABSTRACTION_HPP


#include <rclcpp/rclcpp.hpp>
#include <autoware_map_msgs/msg/lanelet_map_bin.hpp>


namespace crp
{
namespace cil
{

class WorldAbstraction : public rclcpp::Node
{
public:
    WorldAbstraction();

private:
    // TODO: subscribers

    rclcpp::Publisher<autoware_map_msgs::msg::LaneletMapBin>::SharedPtr m_pub_static_map_;
    rclcpp::Publisher<autoware_map_msgs::msg::LaneletMapBin>::SharedPtr m_pub_moving_objects_;
};

} // namespace cil
} // namespace crp
#endif // CRP_CIL_WORLD_ABSTRACTION_SCENARIOABSTRACTION_HPP
