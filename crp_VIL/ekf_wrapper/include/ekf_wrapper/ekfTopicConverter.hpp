#ifndef CRP_VIL_EKF_TOPIC_CONVERTER_EKFWRAPPER_HPP
#define CRP_VIL_EKF_TOPIC_CONVERTER_EKFWRAPPER_HPP

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <geometry_msgs/msg/pose_with_covariance_stamped.hpp>

namespace crp
{
namespace vil
{

class EKFTopicConverter : public rclcpp::Node
{
public:
    EKFTopicConverter();
    
private:
    void ekfPoseCallback(const geometry_msgs::msg::PoseStamped::SharedPtr msg);

    rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr sub_ekfPose_;

    rclcpp::Publisher<geometry_msgs::msg::PoseWithCovarianceStamped>::SharedPtr pub_ekfPose_;

    std::string m_ekfFrame;
};

} // namespace vil
} // namespace crp
#endif //CRP_VIL_EKF_TOPIC_CONVERTER_EKFWRAPPER_HPP