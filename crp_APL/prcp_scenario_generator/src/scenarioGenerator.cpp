#include "prcp_scenario_generator/scenarioGenerator.hpp"


crp::apl::ScenarioGenerator::ScenarioGenerator() : Node("prcp_scenario_generator")
{
    m_sub_egoKinematicState_ = this->create_subscription<autoware_localization_msgs::msg::KinematicState>(
        "/cai/kinematic_state", 10, std::bind(&ScenarioGenerator::egoKinematicStateCallback, this, std::placeholders::_1)
    );

    /* GNSS signals are not used for better robustness (no dependency on GNSS status)*/

    m_sub_egoStatus_ = this->create_subscription<crp_msgs::msg::EgoStatus>(
        "/cai/ego_status", 10, std::bind(&ScenarioGenerator::egoStatusCallback, this, std::placeholders::_1)
    );

    m_pub_scenario_ = this->create_publisher<crp_msgs::msg::Scenario>("/scenario", 10);

    m_publishTimer_ = this->create_wall_timer(std::chrono::milliseconds(50), std::bind(&ScenarioGenerator::publishCallback, this));

    RCLCPP_INFO(this->get_logger(), "scenario_generator has been started");
}

void crp::apl::ScenarioGenerator::publishCallback()
{

    if(!m_initialized)
    {
        m_followedObject.kinematics.initial_pose_with_covariance.pose.position.x = m_object_x0;
        m_followedObject.kinematics.initial_pose_with_covariance.pose.position.y = m_object_y0;
        m_followedObject.kinematics.initial_pose_with_covariance.pose.position.z = 0.0f;

        m_followedObject.kinematics.initial_pose_with_covariance.pose.orientation.x = 0.0f;
        m_followedObject.kinematics.initial_pose_with_covariance.pose.orientation.y = 0.0f;
        m_followedObject.kinematics.initial_pose_with_covariance.pose.orientation.z = 0.0f;
        m_followedObject.kinematics.initial_pose_with_covariance.pose.orientation.w = 0.0f;

        m_initialized = true;
    }
    else
    {
        // calculate the odometry of the ego vehicle
        double yawRate = tan(m_ego.tire_angle_front) / p_axleDistance * m_ego.twist.twist.linear.x;

        m_ego_orientation = m_ego_orientation + 0.05 * yawRate;
        double ds = 0.05*m_ego.twist.twist.linear.x;

        m_ego_x = m_ego_x + ds * cos(m_ego_orientation);
        m_ego_y = m_ego_y + ds * sin(m_ego_orientation);

        m_followedObject.kinematics.initial_pose_with_covariance.pose.position.x = 
            (m_object_x0 - m_ego_x) * cos(m_ego_orientation) + (m_object_y0 - m_ego_y) * sin(m_ego_orientation);
        m_followedObject.kinematics.initial_pose_with_covariance.pose.position.y = 
            -1.0 * (m_object_x0 - m_ego_x) * sin(m_ego_orientation) + (m_object_y0 - m_ego_y) * cos(m_ego_orientation);

        m_objectOrientation = -m_ego_orientation;

        m_followedObject.kinematics.initial_pose_with_covariance.pose.orientation.x = 0.0f;
        m_followedObject.kinematics.initial_pose_with_covariance.pose.orientation.y = 0.0f;
        m_followedObject.kinematics.initial_pose_with_covariance.pose.orientation.z = sin(m_objectOrientation*0.5f);
        m_followedObject.kinematics.initial_pose_with_covariance.pose.orientation.w = cos(m_objectOrientation*0.5f);

        autoware_perception_msgs::msg::ObjectClassification objectClassification;
        objectClassification.label = 1U;
        m_followedObject.classification.push_back(objectClassification);

        m_followedObject.kinematics.initial_twist_with_covariance.twist.linear.x = 0.0f;
        m_followedObject.kinematics.initial_acceleration_with_covariance.accel.linear.x = 0.0f;
    }

    crp_msgs::msg::Scenario scenario;
    scenario.local_moving_objects.objects.push_back(m_followedObject);

    // Lanes
    // ego lane
    crp_msgs::msg::PathWithTrafficRules pathWithTrafficRules;
    pathWithTrafficRules.header.stamp = this->now();
    pathWithTrafficRules.header.frame_id = "base_link";

    tier4_planning_msgs::msg::PathWithLaneId path;

    double x = m_ego_x;

    while(x < m_object_x0*1.1)
    {
        tier4_planning_msgs::msg::PathPointWithLaneId pathPoint;
        geometry_msgs::msg::Point leftBoundPoint;
        geometry_msgs::msg::Point rightBoundPoint;
        double global_x = x;
        double global_y = 0;

        pathPoint.point.pose.position.x = (global_x - m_ego_x)*std::cos(m_ego_orientation) + (global_y - m_ego_y)*std::sin(m_ego_orientation);
        pathPoint.point.pose.position.y = -(global_x - m_ego_x)*std::sin(m_ego_orientation) + (global_y - m_ego_y)*std::cos(m_ego_orientation);
        pathPoint.point.pose.position.z = 0.0;

        leftBoundPoint.x = pathPoint.point.pose.position.x;
        leftBoundPoint.y = pathPoint.point.pose.position.y + 1.875;

        rightBoundPoint.x = pathPoint.point.pose.position.x;
        rightBoundPoint.y = pathPoint.point.pose.position.y - 1.875;

        crp::apl::Quaternion q = yawToQuaternion(- m_ego_orientation);
        pathPoint.point.pose.orientation.x = q.x;
        pathPoint.point.pose.orientation.y = q.y;
        pathPoint.point.pose.orientation.z = q.z;
        pathPoint.point.pose.orientation.w = q.w;

        x = x + 1.0;

        path.points.push_back(pathPoint);
        path.left_bound.push_back(leftBoundPoint);
        path.right_bound.push_back(rightBoundPoint);
    }

    pathWithTrafficRules.path = path;

    scenario.paths.push_back(pathWithTrafficRules);

    // opposite lane    
    crp_msgs::msg::PathWithTrafficRules opponentPathWithTrafficRules;
    opponentPathWithTrafficRules.header.stamp = this->now();
    opponentPathWithTrafficRules.header.frame_id = "base_link";

    tier4_planning_msgs::msg::PathWithLaneId opponentPath;

    x = m_ego_x;

    while (x < m_object_x0*1.1)
    {
        tier4_planning_msgs::msg::PathPointWithLaneId pathPoint;
        geometry_msgs::msg::Point leftBoundPoint;
        geometry_msgs::msg::Point rightBoundPoint;
        double global_x = x;
        double global_y = 3.75;

        pathPoint.point.pose.position.x = (global_x - m_ego_x)*std::cos(m_ego_orientation) + (global_y - m_ego_y)*std::sin(m_ego_orientation);
        pathPoint.point.pose.position.y = -(global_x - m_ego_x)*std::sin(m_ego_orientation) + (global_y - m_ego_y)*std::cos(m_ego_orientation);
        pathPoint.point.pose.position.z = 0.0;

        leftBoundPoint.x = pathPoint.point.pose.position.x;
        leftBoundPoint.y = pathPoint.point.pose.position.y + 1.875;

        rightBoundPoint.x = pathPoint.point.pose.position.x;
        rightBoundPoint.y = pathPoint.point.pose.position.y - 1.875;

        crp::apl::Quaternion q = yawToQuaternion(- m_ego_orientation);
        pathPoint.point.pose.orientation.x = q.x;
        pathPoint.point.pose.orientation.y = q.y;
        pathPoint.point.pose.orientation.z = q.z;
        pathPoint.point.pose.orientation.w = q.w;

        x = x + 1.0;

        opponentPath.points.push_back(pathPoint);
        opponentPath.left_bound.push_back(leftBoundPoint);
        opponentPath.right_bound.push_back(rightBoundPoint);
    }

    opponentPathWithTrafficRules.path = opponentPath;

    scenario.paths.push_back(opponentPathWithTrafficRules);

    scenario.header.stamp = this->now();
    scenario.header.frame_id = "base_link";

    m_pub_scenario_->publish(scenario);
}

void crp::apl::ScenarioGenerator::egoKinematicStateCallback(const autoware_localization_msgs::msg::KinematicState::SharedPtr msg)
{
    m_ego.header = msg->header;
    m_ego.pose = msg->pose_with_covariance;
    m_ego.twist = msg->twist_with_covariance;
    m_ego.accel = msg->accel_with_covariance;    
}

void crp::apl::ScenarioGenerator::gnssFixCallback(const sensor_msgs::msg::NavSatFix::SharedPtr msg)
{
    m_ego.header = msg->header;
    m_ego.gnss_fix = *msg;
}

void crp::apl::ScenarioGenerator::egoStatusCallback(const crp_msgs::msg::EgoStatus::SharedPtr msg)
{
    m_ego.header = msg->header;
    m_ego.tire_angle_front = msg->tire_angle_front;
    m_ego.steering_wheel_rate = msg->steering_wheel_rate;
    m_ego.blinker = msg->blinker;
}

crp::apl::Quaternion crp::apl::ScenarioGenerator::yawToQuaternion(double yaw_rad)
{
  crp::apl::Quaternion q;

  // roll = pitch = 0
  q.x = 0.0;
  q.y = 0.0;
  q.z = std::sin(yaw_rad * 0.5);
  q.w = std::cos(yaw_rad * 0.5);

  return q;
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::apl::ScenarioGenerator>());
    rclcpp::shutdown();
    return 0;
}
