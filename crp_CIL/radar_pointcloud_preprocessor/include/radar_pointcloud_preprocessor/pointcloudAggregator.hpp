#ifndef CRP_CIL_POINTCLOUD_AGGREGATOR_RADARPOINTCLOUDPROCESSOR_HPP
#define CRP_CIL_POINTCLOUD_AGGREGATOR_RADARPOINTCLOUDPRSSOR_HPP

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist_with_covariance_stamped.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <sensor_msgs/point_cloud2_iterator.hpp>
#include <tf2/LinearMath/Transform.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <tf2_sensor_msgs/tf2_sensor_msgs.hpp>
#include <pcl_conversions/pcl_conversions.h>
#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_listener.h>


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
        struct TimedCloud
        {
            sensor_msgs::msg::PointCloud2 cloud;
            geometry_msgs::msg::PoseStamped pose;
        };

        void pclCallback(const sensor_msgs::msg::PointCloud2::SharedPtr msg);
    void twistCallback(const geometry_msgs::msg::TwistWithCovarianceStamped::SharedPtr msg);

    void integrateTwist(const builtin_interfaces::msg::Time & stamp);


        rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr m_sub_inPcl_;
    rclcpp::Subscription<geometry_msgs::msg::TwistWithCovarianceStamped>::SharedPtr m_sub_twist_;

        rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr m_pub_outPcl_;

        std::deque<TimedCloud> m_aggregationPool;
        std::string m_egoFrame;
        float m_aggregationTimeWindow_sec;
        geometry_msgs::msg::PoseStamped m_currentPcdPose;

        // Minimal dead-reckoning state from twist integration
        builtin_interfaces::msg::Time m_lastIntegrationStamp;
        bool m_isTwistAcquired = false;
        bool m_isIntegrationInitialized = false;
        double m_newVx = 0.0;
        double m_newVy = 0.0;
        double m_newYawrate = 0.0;
        double m_x = 0.0;
        double m_y = 0.0;
        double m_yaw = 0.0;

        bool m_isEgo2radTransformSet = false;
        std::string m_frameIdEgo = "base_link";
        geometry_msgs::msg::TransformStamped m_ego2radTransform;
    };

} // namespace cil
} // namespace crp


#endif // CRP_CIL_POINTCLOUD_AGGREGATOR_RADARPOINTCLOUDPROCESSOR_HPP