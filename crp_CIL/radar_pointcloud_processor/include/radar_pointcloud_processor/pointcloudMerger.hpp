#ifndef CRP_CIL_POINTCLOUD_MERGER_RADARPOINTCLOUDPROCESSOR_HPP
#define CRP_CIL_POINTCLOUD_MERGER_RADARPOINTCLOUDPROCESSOR_HPP

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>

#include <vector>

namespace crp 
{
namespace cil 
{
    
    class PointcloudMerger : public rclcpp::Node
    {
        public:
            PointcloudMerger();
        
        private:
            void pclCallback(const sensor_msgs::msg::PointCloud2::SharedPtr msg, uint8_t index);

            std::vector<rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr> m_subs_pclSubscriptions_;

            std::vector<sensor_msgs::msg::PointCloud2> m_pointclouds;
            rclcpp::Time m_lastReferenceRadarPclTime;
            
            float m_mergeTriggerTimeoutSec = 0.2f;
    };

} // namespace cil
} // namespace vil

#endif