#include "radar_pointcloud_processor/dopplerCompensator.hpp"


crp::cil::DopplerCompensator::DopplerCompensator() : Node("doppler_compensator")
{
    this->declare_parameter<std::string>("doppcomp/input_pcl_topic",          "points");
    this->declare_parameter<std::string>("doppcomp/twist_topic",              "/sensing/vehicle/twist");
    this->declare_parameter<std::string>("doppcomp/output_pcl_topic",         "compensated_points");
    this->declare_parameter<std::string>("doppcomp/override_ego_twist_frame", "");

    std::string inputPclTopic, twistTopic, outputPclTopic;
    this->get_parameter<std::string>("doppcomp/input_pcl_topic",          inputPclTopic);
    this->get_parameter<std::string>("doppcomp/twist_topic",              twistTopic);
    this->get_parameter<std::string>("doppcomp/output_pcl_topic",         outputPclTopic);
    this->get_parameter<std::string>("doppcomp/override_ego_twist_frame", frameIdEgo);

    m_isEgo2radTransformSet = frameIdEgo!="";

    m_sub_pcl_ = this->create_subscription<sensor_msgs::msg::PointCloud2>(
        inputPclTopic,
        rclcpp::SensorDataQoS().keep_last(5),
        std::bind(&DopplerCompensator::pclCallback, this, std::placeholders::_1)
    );
    m_sub_twist_ = this->create_subscription<geometry_msgs::msg::TwistWithCovarianceStamped>(
        twistTopic,
        rclcpp::SensorDataQoS().keep_last(5),
        std::bind(&DopplerCompensator::twistCallback, this, std::placeholders::_1)
    );

    rclcpp::PublisherOptions pubOptions;
    pubOptions.qos_overriding_options = rclcpp::QosOverridingOptions::with_default_policies();

    m_pub_compPcl_ = this->create_publisher<sensor_msgs::msg::PointCloud2>(
        outputPclTopic, rclcpp::SensorDataQoS().keep_last(5), pubOptions
    );
}

float getYawFromQuaternion(const geometry_msgs::msg::Quaternion & quaternion)
{
    tf2::Quaternion q(
        quaternion.x,
        quaternion.y,
        quaternion.z,
        quaternion.w);
    tf2::Matrix3x3 m(q);
    double roll, pitch, yaw;
    m.getRPY(roll, pitch, yaw);

    return yaw;
}

void crp::cil::DopplerCompensator::pclCallback(const sensor_msgs::msg::PointCloud2::SharedPtr msg)
{
    if (!m_isEgo2radTransformSet)
    {
        if (frameIdEgo == "")
            return;
        
        tf2_ros::Buffer tfBuffer(this->get_clock());
        tf2_ros::TransformListener tfListener(tfBuffer);
        RCLCPP_INFO(this->get_logger(), "Waiting for transform: %s -> %s", frameIdEgo, msg->header.frame_id);
        geometry_msgs::msg::TransformStamped ego2radTransform = tfBuffer.lookupTransform(msg->header.frame_id, frameIdEgo, rclcpp::Time(0), rclcpp::Duration(5, 0));
        
        ego2radTranslation[0] = ego2radTransform.transform.translation.x;
        ego2radTranslation[1] = ego2radTransform.transform.translation.z;
        ego2radTranslation[2] = ego2radTransform.transform.translation.x;
        ego2radRotation = getYawFromQuaternion(ego2radTransform.transform.rotation);

        m_isEgo2radTransformSet = true;
        RCLCPP_INFO(this->get_logger(), "Transform acquired");
    }
    else
    {
        sensor_msgs::msg::PointCloud2 compPcl;

        compPcl.header = msg->header;
    
        compPcl.height = 1; // unorganized
        compPcl.width = msg->width;

        sensor_msgs::PointCloud2Modifier modifier(compPcl);
        modifier.setPointCloud2Fields(
            10,
            "x",          1, sensor_msgs::msg::PointField::FLOAT32,
            "y",          1, sensor_msgs::msg::PointField::FLOAT32,
            "z",          1, sensor_msgs::msg::PointField::FLOAT32,
            "dist",       1, sensor_msgs::msg::PointField::FLOAT32,
            "azi",        1, sensor_msgs::msg::PointField::FLOAT32,
            "ele",        1, sensor_msgs::msg::PointField::FLOAT32,
            "measStatus", 1, sensor_msgs::msg::PointField::UINT8,
            "rcs",        1, sensor_msgs::msg::PointField::FLOAT32,
            "dv_raw",     1, sensor_msgs::msg::PointField::FLOAT32,
            "dv_comp",    1, sensor_msgs::msg::PointField::FLOAT32 // new field
        );
        modifier.resize(msg->width);

        sensor_msgs::PointCloud2Iterator<float> raw_iter_x           (*msg, "x");
        sensor_msgs::PointCloud2Iterator<float> raw_iter_y           (*msg, "y");
        sensor_msgs::PointCloud2Iterator<float> raw_iter_z           (*msg, "z");
        sensor_msgs::PointCloud2Iterator<float> raw_iter_dist        (*msg, "dist");
        sensor_msgs::PointCloud2Iterator<float> raw_iter_azi         (*msg, "azi");
        sensor_msgs::PointCloud2Iterator<float> raw_iter_ele         (*msg, "ele");
        sensor_msgs::PointCloud2Iterator<float> raw_iter_measStatus  (*msg, "measStatus");
        sensor_msgs::PointCloud2Iterator<float> raw_iter_rcs         (*msg, "rcs");
        sensor_msgs::PointCloud2Iterator<float> raw_iter_dv_raw      (*msg, "dv_raw");

        sensor_msgs::PointCloud2Iterator<float> comp_iter_x          (compPcl, "x");
        sensor_msgs::PointCloud2Iterator<float> comp_iter_y          (compPcl, "y");
        sensor_msgs::PointCloud2Iterator<float> comp_iter_z          (compPcl, "z");
        sensor_msgs::PointCloud2Iterator<float> comp_iter_dist       (compPcl, "dist");
        sensor_msgs::PointCloud2Iterator<float> comp_iter_azi        (compPcl, "azi");
        sensor_msgs::PointCloud2Iterator<float> comp_iter_ele        (compPcl, "ele");
        sensor_msgs::PointCloud2Iterator<float> comp_iter_measStatus (compPcl, "measStatus");
        sensor_msgs::PointCloud2Iterator<float> comp_iter_rcs        (compPcl, "rcs");
        sensor_msgs::PointCloud2Iterator<float> comp_iter_dv_raw     (compPcl, "dv_raw");
        sensor_msgs::PointCloud2Iterator<float> comp_iter_dv_comp    (compPcl, "dv_comp"); // new field

        for (
            ; 
            raw_iter_x != raw_iter_x.end();
            ++raw_iter_x,  ++raw_iter_y,  ++raw_iter_z,  ++raw_iter_dist,  ++raw_iter_azi,  ++raw_iter_ele,  ++raw_iter_measStatus,  ++raw_iter_rcs,  ++raw_iter_dv_raw,
            ++comp_iter_x, ++comp_iter_y, ++comp_iter_z, ++comp_iter_dist, ++comp_iter_azi, ++comp_iter_ele, ++comp_iter_measStatus, ++comp_iter_rcs, ++comp_iter_dv_raw, ++comp_iter_dv_comp
        ) {
            *comp_iter_x          = *raw_iter_x;
            *comp_iter_y          = *raw_iter_y;
            *comp_iter_z          = *raw_iter_z;
            *comp_iter_dist       = *raw_iter_dist;
            *comp_iter_azi        = *raw_iter_azi;
            *comp_iter_ele        = *raw_iter_ele;
            *comp_iter_measStatus = *raw_iter_measStatus;
            *comp_iter_rcs        = *raw_iter_rcs;
            *comp_iter_dv_raw     = *raw_iter_dv_raw;
            
            float vEgoShift[2] = {
                vEgo[0] - yawRateEgo * ego2radTranslation[1],
                vEgo[1] - yawRateEgo * ego2radTranslation[0]
                // assume z is 0
            };

            float vEgoAtRadar[2] = {
                cos(ego2radRotation) * vEgoShift[0]  + sin(ego2radRotation) * vEgoShift[1],
                -sin(ego2radRotation) * vEgoShift[0] + cos(ego2radRotation) * vEgoShift[1]
            };

            *comp_iter_dv_comp = (*raw_iter_dv_raw) + (vEgoAtRadar[0] * cos(*raw_iter_azi)) + (vEgoAtRadar[1] * sin(*raw_iter_azi));
        }

        m_pub_compPcl_->publish(compPcl);
    }
}

void crp::cil::DopplerCompensator::twistCallback(const geometry_msgs::msg::TwistWithCovarianceStamped::SharedPtr msg)
{
    frameIdEgo = msg->header.frame_id;
    vEgo[0] = msg->twist.twist.linear.x;
    vEgo[1] = msg->twist.twist.linear.y;
    yawRateEgo = msg->twist.twist.angular.z;
}


int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::cil::DopplerCompensator>());
    rclcpp::shutdown();
    return 0;
}