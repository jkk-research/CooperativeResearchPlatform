#include <radar_pointcloud_processor/pointcloudAggregator.hpp>


crp::cil::PointcloudAggregator::PointcloudAggregator() : Node("pointcloud_aggregator")
{
    this->declare_parameter<std::string>("input_pcl_topic", "points");
    this->declare_parameter<float>("aggregation_time_window_sec", 0.5f);

    std::string inputTopic;
    this->get_parameter<std::string>("input_pcl_topic", inputTopic);
    this->get_parameter<float>("aggregation_time_window_sec", m_aggregationTimeWindow_sec);

    rclcpp::PublisherOptions pubOptions;
    pubOptions.qos_overriding_options = rclcpp::QosOverridingOptions::with_default_policies();

    m_sub_inPcl_ = this->create_subscription<sensor_msgs::msg::PointCloud2>(
        inputTopic,
        rclcpp::SensorDataQoS().keep_last(5),
        std::bind(&PointcloudAggregator::pclCallback, this, std::placeholders::_1)
    );
}

void crp::cil::PointcloudAggregator::pclCallback(const sensor_msgs::msg::PointCloud2::SharedPtr msg)
{
    while (
        m_aggregationPool.size() > 0
        && (this->get_clock()->now()-m_aggregationPool.front().header.stamp).seconds() > m_aggregationTimeWindow_sec
    )
    { // remove point clouds outside of time window
        m_aggregationPool.pop_front();
    }

    m_aggregationPool.push_back(*msg); // add new pcl to the pool

    // TODO: transform every element in pool to the reference (last pcl)
    for (auto it=m_aggregationPool.begin(); it!=m_aggregationPool.end(); it++)
    {
        // pcl_ros::transformpointcloud
    }


    // TODO: merge pcls


    // sensor_msgs::msg::PointCloud2 compPcl;

    //     compPcl.header = msg->header;
    
    //     compPcl.height = 1; // unorganized
    //     compPcl.width = msg->width;

    //     sensor_msgs::PointCloud2Modifier modifier(compPcl);
    //     modifier.setPointCloud2Fields(
    //         10,
    //         "x",          1, sensor_msgs::msg::PointField::FLOAT32,
    //         "y",          1, sensor_msgs::msg::PointField::FLOAT32,
    //         "z",          1, sensor_msgs::msg::PointField::FLOAT32,
    //         "dist",       1, sensor_msgs::msg::PointField::FLOAT32,
    //         "azi",        1, sensor_msgs::msg::PointField::FLOAT32,
    //         "ele",        1, sensor_msgs::msg::PointField::FLOAT32,
    //         "measStatus", 1, sensor_msgs::msg::PointField::UINT8,
    //         "rcs",        1, sensor_msgs::msg::PointField::FLOAT32,
    //         "dv_raw",     1, sensor_msgs::msg::PointField::FLOAT32,
    //         "dv_comp",    1, sensor_msgs::msg::PointField::FLOAT32 // new field
    //     );
    //     modifier.resize(msg->width);

    //     sensor_msgs::PointCloud2Iterator<float> comp_iter_x          (compPcl, "x");
    //     sensor_msgs::PointCloud2Iterator<float> comp_iter_y          (compPcl, "y");
    //     sensor_msgs::PointCloud2Iterator<float> comp_iter_z          (compPcl, "z");
    //     sensor_msgs::PointCloud2Iterator<float> comp_iter_dist       (compPcl, "dist");
    //     sensor_msgs::PointCloud2Iterator<float> comp_iter_azi        (compPcl, "azi");
    //     sensor_msgs::PointCloud2Iterator<float> comp_iter_ele        (compPcl, "ele");
    //     sensor_msgs::PointCloud2Iterator<float> comp_iter_measStatus (compPcl, "measStatus");
    //     sensor_msgs::PointCloud2Iterator<float> comp_iter_rcs        (compPcl, "rcs");
    //     sensor_msgs::PointCloud2Iterator<float> comp_iter_dv_raw     (compPcl, "dv_raw");
    //     sensor_msgs::PointCloud2Iterator<float> comp_iter_dv_comp    (compPcl, "dv_comp"); // new field

    //     for (
    //         ; 
    //         raw_iter_x != raw_iter_x.end();
    //         ++raw_iter_x,  ++raw_iter_y,  ++raw_iter_z,  ++raw_iter_dist,  ++raw_iter_azi,  ++raw_iter_ele,  ++raw_iter_measStatus,  ++raw_iter_rcs,  ++raw_iter_dv_raw,
    //         ++comp_iter_x, ++comp_iter_y, ++comp_iter_z, ++comp_iter_dist, ++comp_iter_azi, ++comp_iter_ele, ++comp_iter_measStatus, ++comp_iter_rcs, ++comp_iter_dv_raw, ++comp_iter_dv_comp
    //     ) {
    //         *comp_iter_x          = *raw_iter_x;
    //         *comp_iter_y          = *raw_iter_y;
    //         *comp_iter_z          = *raw_iter_z;
    //         *comp_iter_dist       = *raw_iter_dist;
    //         *comp_iter_azi        = *raw_iter_azi;
    //         *comp_iter_ele        = *raw_iter_ele;
    //         *comp_iter_measStatus = *raw_iter_measStatus;
    //         *comp_iter_rcs        = *raw_iter_rcs;
    //         *comp_iter_dv_raw     = *raw_iter_dv_raw;
            
    //         float vEgoShift[2] = {
    //             vEgo[0] - yawRateEgo * ego2radTranslation[1],
    //             vEgo[1] - yawRateEgo * ego2radTranslation[0]
    //             // assume z is 0
    //         };

    //         float vEgoAtRadar[2] = {
    //             cos(ego2radRotation) * vEgoShift[0]  + sin(ego2radRotation) * vEgoShift[1],
    //             -sin(ego2radRotation) * vEgoShift[0] + cos(ego2radRotation) * vEgoShift[1]
    //         };

    //         *comp_iter_dv_comp = (*raw_iter_dv_raw) + (vEgoAtRadar[0] * cos(*raw_iter_azi)) + (vEgoAtRadar[1] * sin(*raw_iter_azi));
    //     }

    //     m_pub_compPcl_->publish(compPcl);
    // }
}
