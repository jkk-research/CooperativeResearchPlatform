#include "prcp_sensor_abstraction/scenarioAbstraction.hpp"


crp::cil::ScenarioAbstraction::ScenarioAbstraction() : Node("scenario_abstraction")
{
    this->declare_parameter<float>("local_path_length", 75.0);

    m_laneletMap = std::make_shared<lanelet::LaneletMap>();

    m_sub_staticMapFromFile_ = this->create_subscription<autoware_map_msgs::msg::LaneletMapBin>(
        "map/global_static_map_from_file/lanelet2_map", rclcpp::QoS{1}.transient_local(), std::bind(&ScenarioAbstraction::staticMapFromFileCallback, this, std::placeholders::_1));

    m_sub_pose_ = this->create_subscription<geometry_msgs::msg::PoseWithCovarianceStamped>(
        "/sensing/gnss/pose_with_covariance", 10, std::bind(&ScenarioAbstraction::poseCallback, this, std::placeholders::_1));

    m_pub_movingObjects_   = this->create_publisher<autoware_perception_msgs::msg::PredictedObjects>("cai/local_moving_objects", 10);
    m_pub_obstacles_       = this->create_publisher<autoware_perception_msgs::msg::PredictedObjects>("cai/local_obstacles", 10);
    m_pub_lanePath_        = this->create_publisher<tier4_planning_msgs::msg::PathWithLaneId>("cai/local_lane/path", 10);
    m_pub_drivableSurface_ = this->create_publisher<nav_msgs::msg::OccupancyGrid>("cai/local_drivable_surface", 10);

    m_publishTimer_ = this->create_wall_timer(
        std::chrono::milliseconds(50), std::bind(&ScenarioAbstraction::publishCallback, this));
}


float crp::cil::ScenarioAbstraction::distanceBetweenPoints(const lanelet::BasicPoint2d a, const lanelet::ConstPoint2d b)
{
    return sqrt(pow(b.x()-a.x(), 2)+pow(b.y()-a.y(), 2));
}


float crp::cil::ScenarioAbstraction::distanceBetweenPoints(const tier4_planning_msgs::msg::PathPointWithLaneId a, const tier4_planning_msgs::msg::PathPointWithLaneId b)
{
    return sqrt(pow(b.point.pose.position.x-a.point.pose.position.x, 2)+pow(b.point.pose.position.y-a.point.pose.position.y, 2));
}


uint16_t crp::cil::ScenarioAbstraction::getGPSNNPointIdx(const lanelet::BasicPoint2d & currentPos, const lanelet::ConstLanelet & lane)
{
    lanelet::ConstLineString2d centerline = lane.centerline2d();
    double minDist = distanceBetweenPoints(currentPos, centerline.front());
    uint16_t minDistIdx = 0;
    for (uint16_t i = 1; i < centerline.size(); i++)
    {
        double currentDist = distanceBetweenPoints(currentPos, centerline[i]);
        if (currentDist < minDist)
        {
            minDist = currentDist;
            minDistIdx = i;
        }
    }
    return minDistIdx;
}


tier4_planning_msgs::msg::PathPointWithLaneId crp::cil::ScenarioAbstraction::laneletPtToPathPoint(const lanelet::ConstPoint2d & pt)
{
    tier4_planning_msgs::msg::PathPointWithLaneId pathPoint;
    pathPoint.point.pose.position.x = pt.x();
    pathPoint.point.pose.position.y = pt.y();
    return pathPoint;
}


float crp::cil::ScenarioAbstraction::getYawFromQuaternion(const geometry_msgs::msg::Quaternion & quaternion)
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


tier4_planning_msgs::msg::PathPointWithLaneId crp::cil::ScenarioAbstraction::transformToLocal(
    const tier4_planning_msgs::msg::PathPointWithLaneId & pathPoint)
{
    float egoYaw = getYawFromQuaternion(m_egoPoseMapFrame.pose.pose.orientation);
    float R[2]{
        cos(egoYaw),
        sin(egoYaw)};
    float translatedX{0.0};
    float translatedY{0.0};

    translatedX = pathPoint.point.pose.position.x - m_egoPoseMapFrame.pose.pose.position.x;
    translatedY = pathPoint.point.pose.position.y - m_egoPoseMapFrame.pose.pose.position.y;

    tier4_planning_msgs::msg::PathPointWithLaneId outPoint;
    outPoint.point.pose.position.x = R[0] * translatedX + R[1] * translatedY;
    outPoint.point.pose.position.y = -R[1] * translatedX + R[0] * translatedY;

    return outPoint;
}


tier4_planning_msgs::msg::PathPointWithLaneId crp::cil::ScenarioAbstraction::addYawAngle(
    const tier4_planning_msgs::msg::PathPointWithLaneId & prevPoint,
    const tier4_planning_msgs::msg::PathPointWithLaneId & pathPoint)
{
    tier4_planning_msgs::msg::PathPointWithLaneId outPoint;
    outPoint.point.pose.position.x = pathPoint.point.pose.position.x;
    outPoint.point.pose.position.y = pathPoint.point.pose.position.y;
    
    // calculate yaw angle with derivative
    float yaw = atan2(
        pathPoint.point.pose.position.y - prevPoint.point.pose.position.y,
        pathPoint.point.pose.position.x - prevPoint.point.pose.position.x
    );

    tf2::Quaternion quat;
    quat.setRPY(0.0f, 0.0f, yaw);
    outPoint.point.pose.orientation = tf2::toMsg(quat);

    return outPoint;
}


void crp::cil::ScenarioAbstraction::publishCallback()
{
    if (!m_isMapLoaded)
        return;

    float localPathLength;
    this->get_parameter("local_path_length", localPathLength);

    lanelet::BasicPoint2d currentPoint(m_egoPoseMapFrame.pose.pose.position.x, m_egoPoseMapFrame.pose.pose.position.y);

    // get nearest lanelet to ego
    std::vector<std::pair<double, lanelet::Lanelet>> actuallyNearestLanelets = 
        lanelet::geometry::findNearest(m_laneletMap->laneletLayer, currentPoint, 1);
    lanelet::Lanelet egoLanelet = actuallyNearestLanelets.front().second;

    uint16_t nearestPointIdx = getGPSNNPointIdx(currentPoint, egoLanelet); // nearest point to ego on lane

    tier4_planning_msgs::msg::PathWithLaneId outPath;
    outPath.header.stamp = this->now();

    float currentPathLength = 0;
    lanelet::ConstLineString2d egoCenterline = egoLanelet.centerline2d();
    tier4_planning_msgs::msg::PathPointWithLaneId prevPoint = laneletPtToPathPoint(egoCenterline[nearestPointIdx]);
    outPath.points.push_back(
        transformToLocal(
            laneletPtToPathPoint(
                egoCenterline[nearestPointIdx]
            )
        )
    ); // add ego point
    uint16_t currentPointIdx = nearestPointIdx + 1;
    while (currentPathLength < localPathLength && currentPointIdx < egoCenterline.size())
    {
        // add points to path from ego lanelet
        tier4_planning_msgs::msg::PathPointWithLaneId currentPoint = laneletPtToPathPoint(egoCenterline[currentPointIdx]);
        currentPathLength += distanceBetweenPoints(prevPoint, currentPoint);
        outPath.points.push_back(
            addYawAngle(
                prevPoint,
                transformToLocal(currentPoint)
            )
        );
        prevPoint = currentPoint;
        currentPointIdx++;
    }

    if (currentPathLength < localPathLength)
    {
        // add points to path from consecutive lanelets

        // get first lanelet after ego
        lanelet::routing::LaneletPaths paths = m_routingGraph->possiblePaths(egoLanelet, 1, 0, false);
        lanelet::routing::LaneletPath selectedPath = paths.front();
        lanelet::ConstLanelet currentLanelet = selectedPath[1];

        currentPointIdx = 0;
        while (currentPathLength < localPathLength)
        {
            lanelet::ConstLineString2d currentCenterline = currentLanelet.centerline2d();

            while (currentPathLength < localPathLength && currentPointIdx < currentCenterline.size())
            {
                // add points to path from current lanelet
                tier4_planning_msgs::msg::PathPointWithLaneId currentPoint = laneletPtToPathPoint(currentCenterline[currentPointIdx]);
                currentPathLength += distanceBetweenPoints(prevPoint, currentPoint);
                outPath.points.push_back(
                    addYawAngle(
                        prevPoint,
                        transformToLocal(currentPoint)
                    )
                );
                prevPoint = currentPoint;
                currentPointIdx++;
            }

            // get next lanelet (first from possible paths)
            lanelet::routing::LaneletPaths paths = m_routingGraph->possiblePaths(currentLanelet, 1, 0, false);
            lanelet::routing::LaneletPath selectedPath = paths.front();
            if (selectedPath.size() < 2)
                break;
            currentLanelet = selectedPath[1];
        }
    }

    // TODO: FILTER TO DERIVATIVES

    m_pub_lanePath_->publish(outPath);
}


void crp::cil::ScenarioAbstraction::staticMapFromFileCallback(const autoware_map_msgs::msg::LaneletMapBin::SharedPtr msg)
{
    RCLCPP_INFO(this->get_logger(), "Loading map from file");
    lanelet::utils::conversion::fromBinMsg(*msg, m_laneletMap);
    m_trafficRules = lanelet::traffic_rules::TrafficRulesFactory::create(lanelet::Locations::Germany, lanelet::Participants::Vehicle);
    m_routingGraph = lanelet::routing::RoutingGraph::build(*m_laneletMap, *m_trafficRules);
    m_mapFrameId = msg->header.frame_id;

    m_isMapLoaded = true;
    RCLCPP_INFO(this->get_logger(), "Map loaded");
}


void crp::cil::ScenarioAbstraction::poseCallback(const geometry_msgs::msg::PoseWithCovarianceStamped::SharedPtr msg)
{
    if (!m_isGpsTransformSet)
    {
        tf2_ros::Buffer tfBuffer(this->get_clock());
        tf2_ros::TransformListener tfListener(tfBuffer);
        m_gps2mapTransform = tfBuffer.lookupTransform(m_mapFrameId, msg->header.frame_id, rclcpp::Time(0), rclcpp::Duration(3, 0));
        m_isGpsTransformSet = true;
    }
    // transform ego pose to map frame
    tf2::doTransform(*msg, m_egoPoseMapFrame, m_gps2mapTransform);
}


int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::cil::ScenarioAbstraction>());
    rclcpp::shutdown();
    return 0;
}
