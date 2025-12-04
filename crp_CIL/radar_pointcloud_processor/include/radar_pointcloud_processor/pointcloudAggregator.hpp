#ifndef CRP_CIL_POINTCLOUD_AGGREGATOR_RADARPOINTCLOUDPROCESSOR_HPP
#define CRP_CIL_POINTCLOUD_AGGREGATOR_RADARPOINTCLOUDPRSSOR_HPP

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/pose_with_covariance_stamped.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <sensor_msgs/point_cloud2_iterator.hpp>
#include <tf2/LinearMath/Transform.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <tf2_sensor_msgs/tf2_sensor_msgs.hpp>
#include <pcl_conversions/pcl_conversions.h>


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
        void poseCallback(const geometry_msgs::msg::PoseWithCovarianceStamped::SharedPtr msg);


        rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr m_sub_inPcl_;
        rclcpp::Subscription<geometry_msgs::msg::PoseWithCovarianceStamped>::SharedPtr m_sub_pose_;

        rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr m_pub_outPcl_;

        std::deque<sensor_msgs::msg::PointCloud2> m_aggregationPool;
        std::string m_egoFrame;
        float m_aggregationTimeWindow_sec;
        geometry_msgs::msg::PoseStamped m_prevPcdPose;
        geometry_msgs::msg::PoseStamped m_currentPcdPose;
        bool m_isPoseAcquired = false;
    };

} // namespace cil
} // namespace crp


#endif // CRP_CIL_POINTCLOUD_AGGREGATOR_RADARPOINTCLOUDPROCESSOR_HPP