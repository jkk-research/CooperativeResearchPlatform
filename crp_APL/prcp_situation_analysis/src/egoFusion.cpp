#include "prcp_situation_analysis/egoFusion.hpp"


crp::apl::EgoFusion::EgoFusion() : Node("ego_fusion")
{
    m_sub_egoKinematicState_ = this->create_subscription<autoware_localization_msgs::msg::KinematicState>(
        "/cai/kinematic_state", 10, std::bind(&EgoFusion::egoKinematicStateCallback, this, std::placeholders::_1)
    );
    m_sub_gnssFix_ = this->create_subscription<sensor_msgs::msg::NavSatFix>(
        "/cai/gnss_fix", 10, std::bind(&EgoFusion::gnssFixCallback, this, std::placeholders::_1)
    );
    m_sub_egoStatus_ = this->create_subscription<crp_msgs::msg::EgoStatus>(
        "/cai/ego_status", 10, std::bind(&EgoFusion::egoStatusCallback, this, std::placeholders::_1)
    );

    m_pub_ego_ = this->create_publisher<crp_msgs::msg::Ego>("/ego", 10);

    m_publishTimer_ = this->create_wall_timer(std::chrono::milliseconds(50), std::bind(&EgoFusion::publishCallback, this));

    RCLCPP_INFO(this->get_logger(), "ego_fusion has been started");
}

void crp::apl::EgoFusion::publishCallback()
{
    m_pub_ego_->publish(m_ego);
}

void crp::apl::EgoFusion::egoKinematicStateCallback(const autoware_localization_msgs::msg::KinematicState::SharedPtr msg)
{
    m_ego.header = msg->header;
    m_ego.pose = msg->pose_with_covariance;
    m_ego.twist = msg->twist_with_covariance;
    m_ego.accel = msg->accel_with_covariance;
}

void crp::apl::EgoFusion::gnssFixCallback(const sensor_msgs::msg::NavSatFix::SharedPtr msg)
{
    m_ego.header = msg->header;
    m_ego.gnss_fix = *msg;
}

void crp::apl::EgoFusion::egoStatusCallback(const crp_msgs::msg::EgoStatus::SharedPtr msg)
{
    m_ego.header = msg->header;
    m_ego.tire_angle_front = msg->tire_angle_front;
    m_ego.steering_wheel_rate = msg->steering_wheel_rate;
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::apl::EgoFusion>());
    rclcpp::shutdown();
    return 0;
}
