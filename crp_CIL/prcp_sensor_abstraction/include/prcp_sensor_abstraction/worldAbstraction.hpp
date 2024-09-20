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
    void staticMapFromFileCallback(const autoware_map_msgs::msg::LaneletMapBin::SharedPtr msg);
    void staticMapFromServerCallback(const autoware_map_msgs::msg::LaneletMapBin::SharedPtr msg);

    rclcpp::Subscription<autoware_map_msgs::msg::LaneletMapBin>::SharedPtr m_sub_staticMapFromFile_;
    rclcpp::Subscription<autoware_map_msgs::msg::LaneletMapBin>::SharedPtr m_sub_staticMapFromServer_;

    rclcpp::Publisher<autoware_map_msgs::msg::LaneletMapBin>::SharedPtr m_pub_staticMap_;
    rclcpp::Publisher<autoware_map_msgs::msg::LaneletMapBin>::SharedPtr m_pub_movingObjects_;
};

} // namespace cil
} // namespace crp
#endif // CRP_CIL_WORLD_ABSTRACTION_SCENARIOABSTRACTION_HPP
