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

    m_pub_egoVisualization_ = this->create_publisher<foxglove_msgs::msg::SceneUpdate>(
        "/egoVisualization", 10);

    m_publishTimer_ = this->create_wall_timer(std::chrono::milliseconds(20), std::bind(&EgoFusion::publishCallback, this));

    RCLCPP_INFO(this->get_logger(), "ego_fusion has been started");
}

void crp::apl::EgoFusion::publishCallback()
{
    // correct the orientation
    double orientationQuaternion[4];
    orientationQuaternion[0] = m_ego.pose.pose.orientation.x;
    orientationQuaternion[1] = m_ego.pose.pose.orientation.y;
    orientationQuaternion[2] = m_ego.pose.pose.orientation.z;
    orientationQuaternion[3] = m_ego.pose.pose.orientation.w;
    double positionVector[2];
    positionVector[0] = m_ego.pose.pose.position.x;
    positionVector[1] = m_ego.pose.pose.position.y;

    m_poseCorrector.run(orientationQuaternion, positionVector, m_ego.twist.twist.linear.x);
    m_ego.orientation = m_poseCorrector.getMeasuredOrientation();
    m_ego.estimated_orientation = m_poseCorrector.getEstimatedOrientation();
    m_pub_ego_->publish(m_ego);
}

void crp::apl::EgoFusion::egoKinematicStateCallback(const autoware_localization_msgs::msg::KinematicState::SharedPtr msg)
{
    m_ego.header = msg->header;
    m_ego.pose = msg->pose_with_covariance;
    m_ego.twist = msg->twist_with_covariance;
    m_ego.accel = msg->accel_with_covariance;

    // visualization
    foxglove_msgs::msg::SceneUpdate visu_msg;

    // Create an entity (represents your ego vehicle)
    foxglove_msgs::msg::SceneEntity entity;
    entity.id = "ego_vehicle";
    entity.timestamp = this->now();   // entity timestamp
    entity.frame_id = "base_link";    // or "map", depending on your TF setup
    entity.frame_locked = false;

    // Create a cube primitive
    foxglove_msgs::msg::CubePrimitive cube;
    cube.pose.position.x = 0.0;
    cube.pose.position.y = 0.0;
    cube.pose.position.z = 0.75;      // half height to sit on ground
    // mimic roll and pitch angle based on acceleration values
    float mimicPitchAngle = -m_ego.accel.accel.linear.x/180.0*3.14159/9.81*20.0;
    float mimicRollAngle = m_ego.accel.accel.linear.y/180.0*3.14159/9.81*20.0;
    float cr = cos(mimicRollAngle * 0.5);
    float sr = sin(mimicRollAngle * 0.5);
    float cp = cos(mimicPitchAngle * 0.5);
    float sp = sin(mimicPitchAngle * 0.5);
    cube.pose.orientation.x = sr*cp;
    cube.pose.orientation.y = cr*sp;
    cube.pose.orientation.z = -sr*sp;
    cube.pose.orientation.w = cr*cp;
    cube.size.x = 4.5;   // length
    cube.size.y = 2.0;   // width
    cube.size.z = 1.5;   // height
    cube.color.r = 0.0f;
    cube.color.g = 0.5f;
    cube.color.b = 1.0f;
    cube.color.a = 0.5f;

    foxglove_msgs::msg::TextPrimitive textSpeed;
    textSpeed.billboard = true;
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << m_ego.twist.twist.linear.x*3.6; // 2 digits after decimal
    textSpeed.text = "vx:" + ss.str() + "km/h";
    textSpeed.pose.position.x = 2.25;
    textSpeed.pose.position.y = 2.0;
    textSpeed.pose.position.z = 1.5;
    textSpeed.font_size = 24.0;
    textSpeed.scale_invariant = true;
    textSpeed.color.a = 1.0f;

    foxglove_msgs::msg::TextPrimitive textAcceleration;
    textAcceleration.billboard = true;
    std::stringstream ss_accel;
    ss_accel << std::fixed << std::setprecision(2) << m_ego.accel.accel.linear.x; // 2 digits after decimal
    textAcceleration.text = "ax:" + ss_accel.str() + "m/s^2";
    textAcceleration.pose.position.x = 2.25;
    textAcceleration.pose.position.y = 2.0;
    textAcceleration.pose.position.z = 1.0;
    textAcceleration.font_size = 24.0;
    textAcceleration.scale_invariant = true;
    textAcceleration.color.a = 1.0f;

    // Attach cube to entity
    entity.cubes.push_back(cube);
    entity.texts.push_back(textSpeed);
    entity.texts.push_back(textAcceleration);

    // Attach entity to scene update
    visu_msg.entities.push_back(entity);

    m_pub_egoVisualization_->publish(visu_msg);
    
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
    m_ego.blinker = msg->blinker;
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::apl::EgoFusion>());
    rclcpp::shutdown();
    return 0;
}
