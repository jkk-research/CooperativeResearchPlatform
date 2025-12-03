#ifndef CRP_CIL_POINTCLOUD_AGGREGATOR_RADARPOINTCLOUDPROCESSOR_HPP
#define CRP_CIL_POINTCLOUD_AGGREGATOR_RADARPOINTCLOUDPRSSOR_HPP

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <sensor_msgs/msg/point_field.hpp>
#include <sensor_msgs/point_cloud2_iterator.hpp>


#include <deque>

namespace crp
{
namespace cil
{

    class PointcloudAggregator : public rclcpp::Node
    {
        public:
        PointcloudAggregator();

        private:
        void pclCallback(const sensor_msgs::msg::PointCloud2::SharedPtr msg);

        rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr m_sub_inPcl_;

        rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr m_pub_outPcl_;

        std::deque<sensor_msgs::msg::PointCloud2> m_aggregationPool;
        std::string m_egoFrame;
        float m_aggregationTimeWindow_sec;
    };

} // namespace cil
} // namespace crp


#endif // CRP_CIL_POINTCLOUD_AGGREGATOR_RADARPOINTCLOUDPROCESSOR_HPP