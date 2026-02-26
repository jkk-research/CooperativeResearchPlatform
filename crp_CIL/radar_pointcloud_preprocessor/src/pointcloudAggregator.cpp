#include <radar_pointcloud_preprocessor/pointcloudAggregator.hpp>

#include <cmath>


crp::cil::PointcloudAggregator::PointcloudAggregator() : Node("pointcloud_aggregator")
{
    this->declare_parameter<std::string>("pcdagg/input_pcd_topic",          "points");
    this->declare_parameter<std::string>("pcdagg/output_pcd_topic",         "aggregated_points");
    this->declare_parameter<std::string>("pcdagg/twist_topic",              "/sensing/gnss/twist");
    this->declare_parameter<float>("pcdagg/aggregation_time_window_sec",    0.5f);
    this->declare_parameter<std::string>("pcdagg/override_ego_twist_frame", "");

    std::string inputTopic, outputTopic, twistTopic;
    this->get_parameter<std::string>("pcdagg/input_pcd_topic",          inputTopic);
    this->get_parameter<std::string>("pcdagg/output_pcd_topic",         outputTopic);
    this->get_parameter<std::string>("pcdagg/twist_topic",              twistTopic);
    this->get_parameter<float>("pcdagg/aggregation_time_window_sec",    m_aggregationTimeWindow_sec);
    this->get_parameter<std::string>("pcdagg/override_ego_twist_frame", m_frameIdEgo);

    m_sub_inPcl_ = this->create_subscription<sensor_msgs::msg::PointCloud2>(
        inputTopic,
        rclcpp::SensorDataQoS().keep_last(3),
        std::bind(&PointcloudAggregator::pclCallback, this, std::placeholders::_1)
    );

    m_sub_twist_ = this->create_subscription<geometry_msgs::msg::TwistWithCovarianceStamped>(
        twistTopic,
        rclcpp::SensorDataQoS().keep_last(3),
        std::bind(&PointcloudAggregator::twistCallback, this, std::placeholders::_1)
    );
    
    rclcpp::PublisherOptions pubOptions;
    pubOptions.qos_overriding_options = rclcpp::QosOverridingOptions::with_default_policies();

    m_pub_outPcl_ = this->create_publisher<sensor_msgs::msg::PointCloud2>(
        outputTopic, rclcpp::SensorDataQoS().keep_last(3), pubOptions
    );
}

void crp::cil::PointcloudAggregator::twistCallback(const geometry_msgs::msg::TwistWithCovarianceStamped::SharedPtr msg)
{
    // TODO: transform to base_link if frame_id differs    
    m_newVx      = msg->twist.twist.linear.x;
    m_newVy      = msg->twist.twist.linear.y;
    m_newYawrate = msg->twist.twist.angular.z;

    if (!m_isTwistAcquired)
    {
        m_lastIntegrationStamp = msg->header.stamp;
        m_isTwistAcquired = true;
    }

    integrateTwist(msg->header.stamp);
}

void crp::cil::PointcloudAggregator::integrateTwist(const builtin_interfaces::msg::Time & stamp)
{
    if (!m_isTwistAcquired)
        return;

    const rclcpp::Time tNow(stamp);
    const rclcpp::Time tPrev(m_lastIntegrationStamp);
    const double dt = (tNow - tPrev).seconds();

    m_yaw += m_newYawrate * dt;
    const double cosYaw = std::cos(m_yaw);
    const double sinYaw = std::sin(m_yaw);

    const double dx_w = (m_newVx * cosYaw - m_newVy * sinYaw) * dt;
    const double dy_w = (m_newVx * sinYaw + m_newVy * cosYaw) * dt;

    m_x += dx_w;
    m_y += dy_w;
    m_lastIntegrationStamp = stamp;

    m_currentPcdPose.header.stamp = stamp;
    m_currentPcdPose.header.frame_id = m_egoFrame;
    m_currentPcdPose.pose.position.x = m_x;
    m_currentPcdPose.pose.position.y = m_y;
    m_currentPcdPose.pose.position.z = 0.0;
    tf2::Quaternion q;
    q.setRPY(0.0, 0.0, m_yaw);
    m_currentPcdPose.pose.orientation = tf2::toMsg(q);

    if (!m_isIntegrationInitialized)
    {
        m_isIntegrationInitialized = true;
    }
}

void crp::cil::PointcloudAggregator::pclCallback(const sensor_msgs::msg::PointCloud2::SharedPtr msg)
{
    if (!m_isEgo2radTransformSet)
    {
        if (m_frameIdEgo == "")
            return;
        
        tf2_ros::Buffer tfBuffer(this->get_clock());
        tf2_ros::TransformListener tfListener(tfBuffer);
        RCLCPP_INFO(this->get_logger(), "Waiting for transform: %s -> %s", m_frameIdEgo.c_str(), msg->header.frame_id.c_str());
        m_ego2radTransform = tfBuffer.lookupTransform(m_frameIdEgo, msg->header.frame_id, rclcpp::Time(0), rclcpp::Duration(5, 0));

        m_isEgo2radTransformSet = true;
        RCLCPP_INFO(this->get_logger(), "Transform acquired");
    }

    if (!m_isTwistAcquired)
        return;

    if (!m_isIntegrationInitialized)
        return;
    
    // remove point clouds outside of time window
    while (
        m_aggregationPool.size() > 0
        && (
            (rclcpp::Time(msg->header.stamp) - rclcpp::Time(m_aggregationPool.front().cloud.header.stamp)).seconds() > m_aggregationTimeWindow_sec
            || (rclcpp::Time(msg->header.stamp) - rclcpp::Time(m_aggregationPool.front().cloud.header.stamp)).seconds() < 0
        )
    )
    {
        m_aggregationPool.pop_front();
    }
    
    sensor_msgs::msg::PointCloud2 mergedPcd;

    tf2::Transform tEgoCurrent;
    tf2::fromMsg(m_currentPcdPose.pose, tEgoCurrent);

    tf2::Transform tEgoToRadar;
    tf2::fromMsg(m_ego2radTransform.transform, tEgoToRadar);

    // ego to radar transform
    const tf2::Transform tRadarCurrent = tEgoCurrent * tEgoToRadar;

    for (const TimedCloud& item : m_aggregationPool)
    {
        tf2::Transform tEgoItem;
        tf2::fromMsg(item.pose.pose, tEgoItem);

        const tf2::Transform tRadarItem = tEgoItem * tEgoToRadar;

        const tf2::Transform tToCurrent = tRadarCurrent.inverse() * tRadarItem;

        geometry_msgs::msg::TransformStamped transform;
        transform.header.stamp = msg->header.stamp;
        transform.transform = tf2::toMsg(tToCurrent);

        sensor_msgs::msg::PointCloud2 transformedPcd;
        tf2::doTransform(item.cloud, transformedPcd, transform);
        transformedPcd.header = item.cloud.header;

        sensor_msgs::msg::PointCloud2 result;
        pcl::concatenatePointCloud(mergedPcd, transformedPcd, result);
        mergedPcd = result;
    }

    // store the new pcd in the pool
    TimedCloud currentTimedCloud;
    currentTimedCloud.cloud = *msg;
    currentTimedCloud.pose = m_currentPcdPose;
    m_aggregationPool.push_back(currentTimedCloud);
    
    sensor_msgs::msg::PointCloud2 result;
    pcl::concatenatePointCloud(mergedPcd, *msg, result);
    mergedPcd = result;

    mergedPcd.header = msg->header;
    mergedPcd.row_step = mergedPcd.width * mergedPcd.point_step;
    
    m_pub_outPcl_->publish(mergedPcd);
}

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::cil::PointcloudAggregator>());
    rclcpp::shutdown();
    return 0;
}
