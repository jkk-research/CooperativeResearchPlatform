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

    m_pub_trajectory_viz_ = this->create_publisher<visualization_msgs::msg::Marker>("plan/trajectoryVisualization", 10);

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
            point.pose.position.y = outputPoint.pose.position.y;
            point.pose.orientation = getYawFromQuaternion(outputPoint.pose.orientation);
            point.velocity = outputPoint.longitudinal_velocity_mps;
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

double crp::apl::MotionHandler::pointDistance(const crp::apl::Point3D & p1, const crp::apl::Point3D & p2)
{
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

void crp::apl::MotionHandler::transformPoint(const crp::apl::Point3D & inPoint, const crp::apl::Pose3D & origo, crp::apl::Point3D & outPoint)
{
    float cosR = cos(origo.orientation);
    float sinR = sin(origo.orientation);

    float translatedX{0.0};
    float translatedY{0.0};
    translatedX = inPoint.x - origo.position.x;
    translatedY = inPoint.y - origo.position.y;

    outPoint.x = cosR * translatedX + sinR * translatedY;
    outPoint.y = -sinR * translatedX + cosR * translatedY;
}

void crp::apl::MotionHandler::findNeighbouringPointsLocal(const PlannerOutputTrajectory & trajectory, const Pose3D & targetPose, const uint32_t & closestPtIdx, int32_t & outIpPointIdx1, int32_t & outIpPointIdx2)
{
    int32_t ipPointIdx1 = -1;
    int32_t ipPointIdx2 = -1;
    // transform closest point to local coordinate system
    Point3D closestPointTransformed;
    transformPoint(trajectory[closestPtIdx].pose.position, targetPose, closestPointTransformed);
    if (closestPointTransformed.x == 0)
    {
        // exact match
        ipPointIdx1 = closestPtIdx;
        ipPointIdx2 = closestPtIdx;
    }
    else if (closestPointTransformed.x > 0)
    {
        // search backwards for sign change
        int32_t j = closestPtIdx - 1;
        int32_t lastPointIdx = closestPtIdx;
        while (j >= 0)
        {
            Point3D pointTransformed;
            transformPoint(trajectory[j].pose.position, targetPose, pointTransformed);
            if (pointTransformed.x < 0)
            {
                ipPointIdx1 = j;
                break;
            }
            lastPointIdx = j;

            j--;
        }
        ipPointIdx2 = lastPointIdx;
    }
    else
    {
        // search forwards for sign change
        uint32_t j = closestPtIdx + 1;
        uint32_t lastPointIdx = closestPtIdx;
        while (j < trajectory.size())
        {
            Point3D pointTransformed;
            transformPoint(trajectory[j].pose.position, targetPose, pointTransformed);
            if (pointTransformed.x > 0)
            {
                ipPointIdx2 = j;
                break;
            }
            lastPointIdx = j;

            j++;
        }
        ipPointIdx1 = lastPointIdx;
    }

    outIpPointIdx1 = ipPointIdx1;
    outIpPointIdx2 = ipPointIdx2;

}

void crp::apl::MotionHandler::interpolateSpeed(autoware_planning_msgs::msg::Trajectory & outputTrajectory, const PlannerOutput & longitudinalTrajectory)
{
    uint32_t lastFirstMatchIdx = 0; // first (by index) closest match in longitudinal trajectory the last time
    float lastSpeed = 0.0; // last speed used for interpolation
    for (uint32_t i=0; i<outputTrajectory.points.size(); i++)
    {
        // find neighboring points in longitudinal trajectory with two stage filtering
        // find closest point (euclidean distance)
        Point3D pt{outputTrajectory.points[i].pose.position.x, outputTrajectory.points[i].pose.position.y, 0};

        double minDist = std::numeric_limits<double>::max();
        uint32_t closestSpeedIdx;

        // find the closest point in longitudinal trajectory
        for (uint32_t j=lastFirstMatchIdx; j<longitudinalTrajectory.trajectory.size(); j++){
            double d = pointDistance(pt, longitudinalTrajectory.trajectory[j].pose.position);
            if (d < minDist)
            {
                minDist = d;
                closestSpeedIdx = j;
            }
            else
            {
                break; // stop when distance begins to grow
            }
        }

        // filter after transformation
        Pose3D targetPose{outputTrajectory.points[i].pose.position.x, outputTrajectory.points[i].pose.position.y, getYawFromQuaternion(outputTrajectory.points[i].pose.orientation)};
        int32_t ipPointIdx1, ipPointIdx2;
        findNeighbouringPointsLocal(longitudinalTrajectory.trajectory, targetPose, closestSpeedIdx, ipPointIdx1, ipPointIdx2);

        // interpolate based on found points
        if (ipPointIdx1 == -1 && ipPointIdx2 == -1)
        {
            // no points found
            outputTrajectory.points[i].longitudinal_velocity_mps = lastSpeed;
            RCLCPP_WARN(this->get_logger(), "No points found in longitudinal trajectory for interpolation");
        }
        else if (ipPointIdx1 == -1)
        {
            // only one point found
            outputTrajectory.points[i].longitudinal_velocity_mps = longitudinalTrajectory.trajectory[ipPointIdx2].velocity;
            lastFirstMatchIdx = ipPointIdx2;
        }
        else if (ipPointIdx2 == -1)
        {
            // only one point found
            outputTrajectory.points[i].longitudinal_velocity_mps = longitudinalTrajectory.trajectory[ipPointIdx1].velocity;
            lastFirstMatchIdx = ipPointIdx1;
        }
        else
        {
            // interpolate speed
            double dist1 = pointDistance(pt, longitudinalTrajectory.trajectory[ipPointIdx1].pose.position);
            double dist2 = pointDistance(pt, longitudinalTrajectory.trajectory[ipPointIdx2].pose.position);
            if (dist1 == 0)
            {
                // exact match, prevents division by zero
                outputTrajectory.points[i].longitudinal_velocity_mps = longitudinalTrajectory.trajectory[ipPointIdx1].velocity;
            }
            else if (dist2 == 0)
            {
                // exact match, prevents division by zero
                outputTrajectory.points[i].longitudinal_velocity_mps = longitudinalTrajectory.trajectory[ipPointIdx2].velocity;
            }
            else
            {
                outputTrajectory.points[i].longitudinal_velocity_mps = (longitudinalTrajectory.trajectory[ipPointIdx1].velocity * dist2 + longitudinalTrajectory.trajectory[ipPointIdx2].velocity * dist1) / (dist1 + dist2);
            }
            lastFirstMatchIdx = ipPointIdx1;
        }
    }
}

void crp::apl::MotionHandler::mapIncomingInputs()
{
    m_outputTrajectory.points.clear();
    // This method maps the m_lateralTrajectory and m_longitudinalTrajectory members to the output
    if (m_currentStrategy == "laneFollowWithDefaultSpeed")
    {
        // requirement: both longitudinal and lateral trajectory available and of same size
        if (m_lateralTrajectory.trajectory.size() > 0)
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
                point.longitudinal_velocity_mps = m_lateralTrajectory.trajectory.at(n).velocity;
                m_outputTrajectory.points.push_back(point);
            }
        }
    }
    else if (m_currentStrategy == "laneFollowWithSpeedAdjust")
    {
        // requirement: both longitudinal and lateral trajectory available and of same size
        if (m_lateralTrajectory.trajectory.size() > 0)
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
                m_outputTrajectory.points.push_back(point);
            }
            interpolateSpeed(m_outputTrajectory, m_longitudinalTrajectory);
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