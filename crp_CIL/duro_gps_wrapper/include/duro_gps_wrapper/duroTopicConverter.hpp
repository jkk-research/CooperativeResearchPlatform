#ifndef CRP_VIL_DURO_TOPIC_CONVERTER_DUROGPSLAUNCHER_HPP
#define CRP_VIL_DURO_TOPIC_CONVERTER_DUROGPSLAUNCHER_HPP

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <geometry_msgs/msg/pose_with_covariance_stamped.hpp>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>


namespace crp
{
namespace vil
{

class DuroTopicConverter : public rclcpp::Node
{
public:
    DuroTopicConverter();

private:
    void currentPoseCallback(const geometry_msgs::msg::PoseStamped::SharedPtr msg);
    float getYawFromQuaternion(const geometry_msgs::msg::Quaternion & quaternion);

    rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr m_sub_currentPose_;

    rclcpp::Publisher<geometry_msgs::msg::PoseWithCovarianceStamped>::SharedPtr m_pub_currentPoseWithCovariance_;
};

} // namespace vil
} // namespace crp

#endif // CRP_VIL_DURO_TOPIC_CONVERTER_DUROGPSLAUNCHER_HPP