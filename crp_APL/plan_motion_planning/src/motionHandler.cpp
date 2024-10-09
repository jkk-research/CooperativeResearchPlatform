#include "plan_motion_planning/motionHandler.hpp"


crp::apl::MotionHandler::MotionHandler() : Node("motion_handler")
{
    m_sub_strategy_   = this->create_subscription<tier4_planning_msgs::msg::Scenario>(
        "plan/strategy", 10, std::bind(&MotionHandler::scenarioCallback, this, std::placeholders::_1));
    m_sub_plan_latLaneFollow_ = this->create_subscription<autoware_planning_msgs::msg::Trajectory>(
        "plan/lat_lane_follow/trajectory", 10, std::bind(&MotionHandler::planLatLaneFollowCallback, this, std::placeholders::_1));
    m_sub_plan_lonEmergency_ = this->create_subscription<autoware_planning_msgs::msg::Trajectory>(
        "plan/lon_emergency/trajectory", 10, std::bind(&MotionHandler::planLonEmergencyCallback, this, std::placeholders::_1));
    m_sub_plan_lonIntelligentSpeedAdjust_ = this->create_subscription<autoware_planning_msgs::msg::Trajectory>(
        "plan/lon_intelligent_speed_adjust/trajectory", 10, std::bind(&MotionHandler::planLonIntelligentSpeedAdjustCallback, this, std::placeholders::_1));

    m_pub_trajectory_ = this->create_publisher<autoware_planning_msgs::msg::Trajectory>("plan/trajectory", 10);

    m_timer_ = this->create_wall_timer(std::chrono::milliseconds(20), std::bind(&MotionHandler::run, this));
}

void crp::apl::MotionHandler::scenarioCallback(const tier4_planning_msgs::msg::Scenario::SharedPtr msg)
{
    if(msg->current_scenario == "laneFollowWithSpeedAdjust"){
        m_currentStrategy = "laneFollowWithSpeedAdjust";
    }
    else{
        m_currentStrategy = "off";
    }
    return;
    // TODO: in some cases there are planners who provide both longitudinal and lateral planning, as well. It means that incoming topic will contain both plans. A concept of how to handle that with strategy interface shall be found out.
}

void crp::apl::MotionHandler::planLatLaneFollowCallback(const autoware_planning_msgs::msg::Trajectory::SharedPtr msg)
{
    // waypoints are coming from the lane follow node - in case of strategy incl. lane follow lateral information should be handed over
    m_lateralTrajectory.trajectory.clear();
    if (m_currentStrategy == "laneFollowWithSpeedAdjust" ||
            m_currentStrategy == "laneFollow"
        ){
        for (const auto & outputPoint : msg->points)
        {
            TrajectoryPoint point;
            point.pose.position.x = outputPoint.pose.position.x;
            point.pose.position.x = outputPoint.pose.position.y;
            point.pose.orientation = getYawFromQuaternion(outputPoint.pose.orientation);
            m_lateralTrajectory.trajectory.push_back(point);
        }
    }
    // else do nothing
    return;
}


void crp::apl::MotionHandler::planLonEmergencyCallback(const autoware_planning_msgs::msg::Trajectory::SharedPtr msg)
{
    // TODO
    return;
}


void crp::apl::MotionHandler::planLonIntelligentSpeedAdjustCallback(const autoware_planning_msgs::msg::Trajectory::SharedPtr msg)
{
    m_longitudinalTrajectory.trajectory.clear();
    if (m_currentStrategy == "laneFollowWithSpeedAdjust" ||
            m_currentStrategy == "speedAdjust"
        ){
        for (const auto & outputPoint : msg->points)
        {
            TrajectoryPoint point;
            point.pose.position.x = outputPoint.pose.position.x;
            point.pose.position.x = outputPoint.pose.position.y;
            point.pose.orientation = getYawFromQuaternion(outputPoint.pose.orientation);
            point.velocity = outputPoint.longitudinal_velocity_mps;
            m_longitudinalTrajectory.trajectory.push_back(point);
        }
    }
    return;
}

float crp::apl::MotionHandler::getYawFromQuaternion(const geometry_msgs::msg::Quaternion & quaternion)
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

void crp::apl::MotionHandler::mapIncomingInputs()
{
    m_outputTrajectory.points.clear();
    // This method maps the m_lateralTrajectory and m_longitudinalTrajectory members to the output
    if (m_currentStrategy == "laneFollowWithSpeedAdjust")
    {
        // requirement: both longitudinal and lateral trajectory available and of same size
        if (m_lateralTrajectory.trajectory.size() > 0 && 
                m_longitudinalTrajectory.trajectory.size() > 0 && 
                m_lateralTrajectory.trajectory.size() == m_longitudinalTrajectory.trajectory.size())
        {
            unsigned long int N = m_lateralTrajectory.trajectory.size();
            // map lateral and longitudinal together
            autoware_planning_msgs::msg::TrajectoryPoint point;
            for (unsigned long int n=0; n<N; n++)
            {
                point.pose.position.x = m_lateralTrajectory.trajectory.at(n).pose.position.x;
                point.pose.position.y = m_lateralTrajectory.trajectory.at(n).pose.position.y;
                tf2::Quaternion(tf2::Vector3(0, 0, 1), m_lateralTrajectory.trajectory.at(n).pose.orientation);
                point.pose.orientation = tf2::toMsg(tf2::Quaternion(tf2::Vector3(0, 0, 1), m_lateralTrajectory.trajectory.at(n).pose.orientation));
                point.longitudinal_velocity_mps = m_longitudinalTrajectory.trajectory.at(n).velocity;
                m_outputTrajectory.points.push_back(point);
            }
        }
    }
    // else: do nothing
}

void crp::apl::MotionHandler::visualizeTrajectory()
{
    visualization_msgs::msg::Marker marker;
    marker.header.frame_id = "base_link";
    marker.header.stamp = this->now();
    marker.ns = "trajectory";
    marker.id = 0;
    marker.type = visualization_msgs::msg::Marker::LINE_STRIP;
    marker.action = visualization_msgs::msg::Marker::ADD;
    marker.pose.orientation.w = 1.0;
    marker.scale.x = 0.1;
    marker.color.r = 1.0;
    marker.color.a = 1.0;
    for (const auto & point : m_outputTrajectory.points)
    {
        geometry_msgs::msg::Point p;
        p.x = point.pose.position.x;
        p.y = point.pose.position.y;
        p.z = 0;
        marker.points.push_back(p);
    }

    m_pub_trajectory_viz_->publish(marker);


    return;
}

void crp::apl::MotionHandler::run()
{
    mapIncomingInputs();
    visualizeTrajectory();
    m_pub_trajectory_->publish(m_outputTrajectory);
}


int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::apl::MotionHandler>());
    rclcpp::shutdown();
    return 0;
}