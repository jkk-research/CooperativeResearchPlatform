#include <radar_pointcloud_preprocessor/pointcloudAggregator.hpp>


crp::cil::PointcloudAggregator::PointcloudAggregator() : Node("pointcloud_aggregator")
{
    this->declare_parameter<std::string>("pcdagg/input_pcd_topic",      "points");
    this->declare_parameter<std::string>("pcdagg/output_pcd_topic",     "aggregated_points");
    this->declare_parameter<std::string>("pcdagg/localization_topic",   "/sensing/gnss/pose_with_covariance");
    this->declare_parameter<float>("pcdagg/aggregation_time_window_sec", 0.5f);

    std::string inputTopic, outputTopic, localizationTopic;
    this->get_parameter<std::string>("pcdagg/input_pcd_topic",       inputTopic);
    this->get_parameter<std::string>("pcdagg/output_pcd_topic",      outputTopic);
    this->get_parameter<std::string>("pcdagg/localization_topic",    localizationTopic);
    this->get_parameter<float>("pcdagg/aggregation_time_window_sec", m_aggregationTimeWindow_sec);

    m_sub_inPcl_ = this->create_subscription<sensor_msgs::msg::PointCloud2>(
        inputTopic,
        rclcpp::SensorDataQoS().keep_last(1),
        std::bind(&PointcloudAggregator::pclCallback, this, std::placeholders::_1)
    );

    m_sub_pose_ = this->create_subscription<geometry_msgs::msg::PoseWithCovarianceStamped>(
        localizationTopic,
        1,
        std::bind(&PointcloudAggregator::poseCallback, this, std::placeholders::_1)
    );

    m_pub_outPcl_ = this->create_publisher<sensor_msgs::msg::PointCloud2>(
        outputTopic, 1
    );
}

void crp::cil::PointcloudAggregator::poseCallback(const geometry_msgs::msg::PoseWithCovarianceStamped::SharedPtr msg)
{
    m_currentPcdPose.header = msg->header;
    m_currentPcdPose.pose   = msg->pose.pose;

    if (!m_isPoseAcquired)
    {
        m_prevPcdPose = m_currentPcdPose;
        m_isPoseAcquired = true;
    }
}

void crp::cil::PointcloudAggregator::pclCallback(const sensor_msgs::msg::PointCloud2::SharedPtr msg)
{
    if (!m_isPoseAcquired)
        return;
    
    // remove point clouds outside of time window
    while (
        m_aggregationPool.size() > 0
        && (
            (rclcpp::Time(msg->header.stamp) - rclcpp::Time(m_aggregationPool.front().header.stamp)).seconds() > m_aggregationTimeWindow_sec
            || (rclcpp::Time(msg->header.stamp) - rclcpp::Time(m_aggregationPool.front().header.stamp)).seconds() < 0
        )
    )
    {
        m_aggregationPool.pop_front();
    }

    // get the transform from the pool to the reference (currend pcd)
    geometry_msgs::msg::TransformStamped transform;
    transform.header.stamp = msg->header.stamp;
    
    tf2::Transform tCurrent, tPrev;
    
    tf2::fromMsg(m_currentPcdPose.pose, tCurrent);
    tf2::fromMsg(m_prevPcdPose.pose,    tPrev);
    
    tf2::Transform tToCurrent = tCurrent.inverse() * tPrev;
    
    transform.transform = tf2::toMsg(tToCurrent);

    // transform the pool to the reference
    for (
        std::deque<sensor_msgs::msg::PointCloud2>::iterator it=m_aggregationPool.begin(); 
        it != m_aggregationPool.end(); 
        ++it)
    {
        sensor_msgs::msg::PointCloud2 transformedPcd;
        tf2::doTransform(*it, transformedPcd, transform);
        transformedPcd.header = it->header;
        *it = transformedPcd;
    }
    
    // add the new pcd to the pool
    m_aggregationPool.push_back(*msg);
    m_prevPcdPose = m_currentPcdPose;
    
    sensor_msgs::msg::PointCloud2 mergedPcd;

    for (const sensor_msgs::msg::PointCloud2& cloudMsg : m_aggregationPool)
    {
        sensor_msgs::msg::PointCloud2 result;
        pcl::concatenatePointCloud(mergedPcd, cloudMsg, result);
        mergedPcd = result;
    }
    
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
