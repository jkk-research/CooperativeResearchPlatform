#include <radar_pointcloud_processor/pointcloudMerger.hpp>

crp::cil::PointcloudMerger::PointcloudMerger() : Node("pointcloud_merger")
{
    this->declare_parameter<std::vector<std::string>>("pcl_topics", std::vector<std::string>{});

    std::vector<std::string> pclTopics;
    this->get_parameter<std::vector<std::string>>("pcl_topics", pclTopics);

    if (pclTopics.empty()) {
        RCLCPP_WARN(this->get_logger(), "No topics provided.");
    }

    m_lastReferenceRadarPclTime = this->get_clock()->now();
    m_pointclouds.reserve(pclTopics.size());

    for (uint8_t i = 0; i < pclTopics.size(); i++) {
        auto sub = this->create_subscription<sensor_msgs::msg::PointCloud2>(
            pclTopics[i],
            1,
            [this, i](sensor_msgs::msg::PointCloud2::SharedPtr msg) {
                this->pclCallback(msg, i);
            }
        );
        m_subs_pclSubscriptions_.push_back(sub);
        RCLCPP_INFO(this->get_logger(), "Subscribed to: %s", pclTopics[i].c_str());
    }
}

void crp::cil::PointcloudMerger::pclCallback(const sensor_msgs::msg::PointCloud2::SharedPtr msg, uint8_t index)
{
    if (
        index == 0
        || (this->get_clock()->now() - m_lastReferenceRadarPclTime).seconds() > 0.1
    )
        m_lastReferenceRadarPclTime = this->get_clock()->now();

    m_pointclouds[index] = *msg;
}

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::cil::PointcloudMerger>());
    rclcpp::shutdown();
    return 0;
}