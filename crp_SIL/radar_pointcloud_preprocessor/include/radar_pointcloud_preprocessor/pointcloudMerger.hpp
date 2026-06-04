#ifndef CRP_CIL_POINTCLOUD_MERGER_RADARPOINTCLOUDPROCESSOR_HPP
#define CRP_CIL_POINTCLOUD_MERGER_RADARPOINTCLOUDPROCESSOR_HPP

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <pcl_conversions/pcl_conversions.h>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_listener.h>
#include <tf2_sensor_msgs/tf2_sensor_msgs.h>

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

            std::vector<rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr> m_subs_pcdSubscriptions;

            rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr m_pub_mergedPcd_;

            std::vector<sensor_msgs::msg::PointCloud2> m_pointclouds;
            rclcpp::Time m_lastReferenceRadarPclTime;
            
            std::string m_egoFrame;
            float m_mergeTriggerTimeoutSec = 0.1f;
            uint8_t m_mergeTriggerPcdIdx = 0;
            std::unique_ptr<tf2_ros::Buffer> m_tfBuffer;
            std::unique_ptr<tf2_ros::TransformListener> m_tfListener;
    };

} // namespace cil
} // namespace vil

#endif