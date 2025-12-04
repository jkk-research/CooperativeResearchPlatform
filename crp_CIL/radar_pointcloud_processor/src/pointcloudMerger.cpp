#include <radar_pointcloud_processor/pointcloudMerger.hpp>

crp::cil::PointcloudMerger::PointcloudMerger() : Node("pointcloud_merger")
{
    // params
    this->declare_parameter<std::vector<std::string>>("pcdmerger/in_pcd_topics",             std::vector<std::string>{});
    this->declare_parameter<std::string>             ("pcdmerger/out_topic",                 "merged_points");
    this->declare_parameter<std::string>             ("pcdmerger/ego_frame",                 "base_link");
    this->declare_parameter<float>                   ("pcdmerger/merge_trigger_timeout_sec", 0.1f);

    std::vector<std::string> pclTopics;
    std::string outTopic;
    this->get_parameter<std::vector<std::string>>("pcdmerger/in_pcd_topics",             pclTopics);
    this->get_parameter<std::string>             ("pcdmerger/out_topic",                 outTopic);
    this->get_parameter<std::string>             ("pcdmerger/ego_frame",                 m_egoFrame);
    this->get_parameter<float>                   ("pcdmerger/merge_trigger_timeout_sec", m_mergeTriggerTimeoutSec);

    if (pclTopics.empty()) {
        RCLCPP_WARN(this->get_logger(), "No topics provided.");
    }

    // tf listener for ego transform
    m_tfBuffer   = std::make_unique<tf2_ros::Buffer>(this->get_clock());
    m_tfListener = std::make_unique<tf2_ros::TransformListener>(*m_tfBuffer);

    // subscribers
    m_pointclouds.reserve(pclTopics.size());
    
    for (uint8_t i = 0; i < pclTopics.size(); i++) {
        auto sub = this->create_subscription<sensor_msgs::msg::PointCloud2>(
            pclTopics[i],
            1,
            [this, i](sensor_msgs::msg::PointCloud2::SharedPtr msg) {
                this->pclCallback(msg, i);
            }
        );
        m_subs_pcdSubscriptions.push_back(sub);
        RCLCPP_INFO(this->get_logger(), "Subscribed to: %s", pclTopics[i].c_str());
    }

    m_pub_mergedPcd_ = this->create_publisher<sensor_msgs::msg::PointCloud2>(
        outTopic, 1
    );

    m_pointclouds.resize(m_subs_pcdSubscriptions.size());

    m_lastReferenceRadarPclTime = this->get_clock()->now();
}

void crp::cil::PointcloudMerger::pclCallback(const sensor_msgs::msg::PointCloud2::SharedPtr msg, uint8_t index)
{
    // select next pcd source as reference if the current one stagnates
    if ((this->get_clock()->now() - m_lastReferenceRadarPclTime).seconds() > m_mergeTriggerTimeoutSec)
        m_mergeTriggerPcdIdx = (m_mergeTriggerPcdIdx > m_subs_pcdSubscriptions.size()) ? ++m_mergeTriggerPcdIdx : 0;
    
    // transform to ego
    geometry_msgs::msg::TransformStamped toEgoTransform = m_tfBuffer->lookupTransform(m_egoFrame, msg->header.frame_id, rclcpp::Time(0), rclcpp::Duration(5, 0));
    sensor_msgs::msg::PointCloud2 pcdAtEgo;
    tf2::doTransform(*msg, pcdAtEgo, toEgoTransform);
    m_pointclouds.at(index) = pcdAtEgo;

    // merge point clouds
    sensor_msgs::msg::PointCloud2 mergedPcd;

    for (const sensor_msgs::msg::PointCloud2& pcd : m_pointclouds)
    {
        sensor_msgs::msg::PointCloud2 result;
        pcl::concatenatePointCloud(mergedPcd, pcd, result);
        mergedPcd = result;
    }

    // set metadatas
    mergedPcd.row_step = mergedPcd.width * mergedPcd.point_step;
    mergedPcd.header.stamp = msg->header.stamp;
    mergedPcd.header.frame_id = m_egoFrame;

    // trigger
    if (index == m_mergeTriggerPcdIdx)
    {
        m_lastReferenceRadarPclTime = this->get_clock()->now();

        m_pub_mergedPcd_->publish(mergedPcd);
    }
}

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::cil::PointcloudMerger>());
    rclcpp::shutdown();
    return 0;
}