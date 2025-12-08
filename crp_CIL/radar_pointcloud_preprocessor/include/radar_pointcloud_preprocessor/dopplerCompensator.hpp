#ifndef CRP_CIL_DOPPLER_COMPENSATOR_RADARPOINTCLOUDPROCESSOR_HPP
#define CRP_CIL_DOPPLER_COMPENSATOR_RADARPOINTCLOUDPROCESSOR_HPP

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <geometry_msgs/msg/twist_with_covariance_stamped.hpp>
#include <sensor_msgs/point_cloud2_iterator.hpp>
#include <sensor_msgs/msg/point_field.hpp>
#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_listener.h>


namespace crp
{
namespace cil
{

    class DopplerCompensator : public rclcpp::Node
    {
        public:
        DopplerCompensator();
            
        private:
        void pclCallback(const sensor_msgs::msg::PointCloud2::SharedPtr msg);
        void twistCallback(const geometry_msgs::msg::TwistWithCovarianceStamped::SharedPtr msg);

        rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr m_sub_pcl_;
        rclcpp::Subscription<geometry_msgs::msg::TwistWithCovarianceStamped>::SharedPtr m_sub_twist_;

        rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr m_pub_compPcl_;

        bool m_isEgo2radTransformSet = false;
        float m_vEgo[2] = {0.0f, 0.0f};
        float m_yawRateEgo = 0.0f;
        std::string m_frameIdEgo = "";
        float m_ego2radTranslation[3] = {0.0f, 0.0f, 0.0f};
        float m_ego2radRotation = 0.0f;
    };

} // namespace cil
} // namespace vil


#endif // CRP_CIL_DOPPLER_COMPENSATOR_RADARPOINTCLOUDPROCESSOR_HPP