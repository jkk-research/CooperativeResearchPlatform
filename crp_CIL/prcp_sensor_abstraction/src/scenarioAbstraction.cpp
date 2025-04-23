#include "prcp_sensor_abstraction/scenarioAbstraction.hpp"


crp::cil::ScenarioAbstraction::ScenarioAbstraction() : Node("scenario_abstraction")
{
    this->declare_parameter<float>("local_path_length", 75.0);
    this->declare_parameter<std::string>("localization_source", "ekf");

    std::string localizationSource;
    this->get_parameter("localization_source", localizationSource);

    std::string localizationTopic = "";
    if (localizationSource == "ekf")
        localizationTopic = "/sensing/ekf/estimated_pose";
    else if (localizationSource == "gnss")
        localizationTopic = "/sensing/gnss/pose_with_covariance";
    else
        throw std::runtime_error("Unknown localization source: " + localizationSource);

    m_laneletMap = std::make_shared<lanelet::LaneletMap>();

    m_sub_staticMapFromFile_ = this->create_subscription<autoware_map_msgs::msg::LaneletMapBin>(
        "map/global_static_map_from_file/lanelet2_map", rclcpp::QoS{1}.transient_local(), std::bind(&ScenarioAbstraction::staticMapFromFileCallback, this, std::placeholders::_1));

    m_sub_pose_ = this->create_subscription<geometry_msgs::msg::PoseWithCovarianceStamped>(
        localizationTopic, 10, std::bind(&ScenarioAbstraction::poseCallback, this, std::placeholders::_1));

    m_pub_movingObjects_   = this->create_publisher<autoware_perception_msgs::msg::PredictedObjects>("cai/local_moving_objects", 10);
    m_pub_obstacles_       = this->create_publisher<autoware_perception_msgs::msg::PredictedObjects>("cai/local_obstacles", 10);
    m_pub_possiblePaths_   = this->create_publisher<crp_msgs::msg::PathWithTrafficRulesArray>("cai/local_lane/possible_paths", 10);
    m_pub_drivableSurface_ = this->create_publisher<nav_msgs::msg::OccupancyGrid>("cai/local_drivable_surface", 10);

    m_publishTimer_ = this->create_wall_timer(
        std::chrono::milliseconds(20), std::bind(&ScenarioAbstraction::publishCallback, this));
    
    RCLCPP_INFO(this->get_logger(), "scenario_abstraction has been started");
}


crp_msgs::msg::StopPose crp::cil::ScenarioAbstraction::extractStopPoseFromLine(
    const lanelet::ConstLineString3d& stopLine,
    const uint8_t&                    stopLineType
)
{
    crp_msgs::msg::StopPose stopPose;
    // average out the stop line points
    geometry_msgs::msg::Pose stopPoint;
    for (lanelet::BasicPoint3d stopLinePt : stopLine)
    {
        stopPoint.position.x += stopLinePt.x();
        stopPoint.position.y += stopLinePt.y();
        stopPoint.position.z += stopLinePt.z();
    }
    stopPoint.position.x /= stopLine.size();
    stopPoint.position.y /= stopLine.size();
    stopPoint.position.z /= stopLine.size();
    
    stopPose.pose = stopPoint;
    stopPose.type = stopLineType;
    stopPose.confidence = 1.0f;

    return stopPose;
}


std::vector<crp_msgs::msg::StopPose> crp::cil::ScenarioAbstraction::extractStopSigns(
    const lanelet::ConstLanelet& lanelet
)
{
    std::vector<crp_msgs::msg::StopPose> stopPoses;
    std::vector<std::shared_ptr<const lanelet::TrafficSign>> trafficSigns = lanelet.regulatoryElementsAs<lanelet::TrafficSign>();
    
    for (std::shared_ptr<const lanelet::TrafficSign> sign : trafficSigns)
    {
        if (sign->type() == "stop_sign")
        {
            if (sign->refLines().empty())
                continue;
            
            // add stop lines
            for (lanelet::ConstLineString3d& stopLine : sign->refLines())
            {
                crp_msgs::msg::StopPose stopPose = extractStopPoseFromLine(stopLine, crp_msgs::msg::StopPose::STOP_SIGN);
                
                if (m_abstractionUtils.distanceBetweenPoints(stopPose.pose, m_egoPoseMapFrame.pose.pose) <= m_localPathLength)
                {
                    stopPoses.push_back(stopPose);
                }
            }
        }
    }

    return stopPoses;
}


std::vector<crp_msgs::msg::StopPose> crp::cil::ScenarioAbstraction::extractCrosswalk(
    const lanelet::ConstLanelet& lanelet
)
{
    std::vector<crp_msgs::msg::StopPose> stopPoses;
    lanelet::routing::RoutingGraphContainer overall_graphs({m_routingGraphVehicle, m_routingGraphPedestrian});
    lanelet::ConstLanelets crosswalks = overall_graphs.conflictingInGraph(lanelet, 1);

    for (lanelet::ConstLanelet& crosswalk : crosswalks)
    {
        // add crosswalk stop poses
        lanelet::ConstLineString3d crosswalkLine = crosswalk.centerline3d();

        crp_msgs::msg::StopPose stopPose = extractStopPoseFromLine(crosswalkLine, crp_msgs::msg::StopPose::CROSSWALK);

        if (m_abstractionUtils.distanceBetweenPoints(stopPose.pose, m_egoPoseMapFrame.pose.pose) <= m_localPathLength)
        {
            stopPoses.push_back(stopPose);
        }
    }

    return stopPoses;
}


std::vector<crp_msgs::msg::StopPose> crp::cil::ScenarioAbstraction::filterStopPoses(
    const std::vector<crp_msgs::msg::StopPose>& globalStopPoses,
    const lanelet::ConstLineString2d&           centerline,
    const uint16_t&                             egoPointIdx
)
{
    if (
        egoPointIdx == 0 ||
        globalStopPoses.empty()
    )
        return globalStopPoses;

    // check if stop poses are valid (not passed yet)
    // set index jump interval (default point distance interval: 5m)
    uint8_t meterIdxJumpApprox = 1;
    if (centerline.size() > 1)
    {
        float distance = m_abstractionUtils.distanceBetweenPoints(
            m_abstractionUtils.laneletPtToPathPoint(centerline[0], 0),
            m_abstractionUtils.laneletPtToPathPoint(centerline[1], 0)
        );
        meterIdxJumpApprox = std::max<uint8_t>(static_cast<uint8_t>(1.0f / distance), 1u);
    }

    // find closest points to stop poses
    std::vector<std::pair<uint16_t, float>> stopPoseClosestPoints(globalStopPoses.size(), {0, std::numeric_limits<float>::max()}); // index, distance

    for (uint16_t stopPoseIdx = 0; stopPoseIdx < globalStopPoses.size(); stopPoseIdx++)
    {
        for (uint16_t checkPointIdx = egoPointIdx; checkPointIdx < centerline.size(); )
        {
            geometry_msgs::msg::Pose currentPoint;
            currentPoint.position.x = centerline[checkPointIdx].x();
            currentPoint.position.y = centerline[checkPointIdx].y();

            float currentDistance = m_abstractionUtils.distanceBetweenPoints(currentPoint, globalStopPoses[stopPoseIdx].pose);
            if (currentDistance < stopPoseClosestPoints[stopPoseIdx].second)
            {
                stopPoseClosestPoints[stopPoseIdx].first = checkPointIdx;
                stopPoseClosestPoints[stopPoseIdx].second = currentDistance;
            }
            checkPointIdx += std::max<uint16_t>(static_cast<uint16_t>(currentDistance / meterIdxJumpApprox), 1u);
        }
    }

    std::vector<crp_msgs::msg::StopPose> filteredStopPoses;
    // filter stop poses by closes point position
    for (uint16_t stopPoseIdx = 0; stopPoseIdx < globalStopPoses.size(); stopPoseIdx++)
    {
        // if stop sign is close to ego [5m] then check by local x coordinate
        // and check distance from point
        if (
            stopPoseClosestPoints[stopPoseIdx].first <= egoPointIdx + meterIdxJumpApprox * 5 &&
            (
                m_abstractionUtils.transformToLocal(globalStopPoses[stopPoseIdx].pose, m_egoPoseMapFrame).position.x < 0.0f ||
                stopPoseClosestPoints[stopPoseIdx].second > 5.0f
            )
        )
        {
            // stop pose is passed by ego so skip it
            continue;
        }

        filteredStopPoses.push_back(globalStopPoses[stopPoseIdx]);
    }

    return filteredStopPoses;
}


crp_msgs::msg::PathWithTrafficRules crp::cil::ScenarioAbstraction::extractLaneletToCompleteLength(
    const lanelet::ConstLanelet&                   lanelet,
    const float&                                   currentCompletePathLength,
    tier4_planning_msgs::msg::PathPointWithLaneId& prevPointGlobal,
    uint16_t                                       currentPointIdx=0
)
{
    crp_msgs::msg::PathWithTrafficRules lanePath;

    // extract stop poses
    std::vector<crp_msgs::msg::StopPose> globalStopPoses;
    {
        // extract stop signs
        std::vector<crp_msgs::msg::StopPose> stopSigns = extractStopSigns(lanelet);

        // TODO: add crosswalks
        // ! crosswalk extraction takes too long with large lanelets
        // ? only extract crosswalks once and cache them
        // // extract crosswalks
        // std::vector<crp_msgs::msg::StopPose> crosswalks = extractCrosswalk(lanelet);
        
        globalStopPoses.reserve(stopSigns.size()); // + crosswalks.size());

        globalStopPoses.insert(globalStopPoses.end(), stopSigns.begin(), stopSigns.end());
        // globalStopPoses.insert(globalStopPoses.end(), crosswalks.begin(), crosswalks.end());
    }

    // extract lanelet line
    float laneletSpeedLimit = m_trafficRulesVehicle->speedLimit(lanelet).speedLimit.value();
    lanePath.path_length = 0.0f;
    lanelet::ConstLineString2d currentCenterline = lanelet.centerline2d();

    // check stop poses are valid (not passed yet)
    std::vector<crp_msgs::msg::StopPose> filteredStopPoses = filterStopPoses(globalStopPoses, currentCenterline, currentPointIdx);
    
    // transform stop poses to local frame and add to lanelet
    for (crp_msgs::msg::StopPose stopPose : filteredStopPoses)
    {
        stopPose.pose = m_abstractionUtils.transformToLocal(stopPose.pose, m_egoPoseMapFrame);
        lanePath.traffic_rules.stop_poses.push_back(stopPose);
    }

    // extract path
    tier4_planning_msgs::msg::PathPointWithLaneId currentPrevPoint = prevPointGlobal;
    while (lanePath.path_length+currentCompletePathLength < m_localPathLength && currentPointIdx < currentCenterline.size())
    {
        // get current point
        tier4_planning_msgs::msg::PathPointWithLaneId currentPoint = m_abstractionUtils.laneletPtToPathPoint(currentCenterline[currentPointIdx],laneletSpeedLimit);
        
        // add point to lanelet
        lanePath.path_length += m_abstractionUtils.distanceBetweenPoints(currentPrevPoint, currentPoint);
        lanePath.path.points.push_back(
            m_abstractionUtils.transformToLocal(currentPoint, m_egoPoseMapFrame)
        );
        currentPrevPoint = currentPoint;
        currentPointIdx++;
    }

    // set prevPoint for next lanelet
    prevPointGlobal = currentPrevPoint;

    return lanePath;
}


void crp::cil::ScenarioAbstraction::extractPossiblePaths(
    const lanelet::ConstLanelet&                   startLanelet,
    tier4_planning_msgs::msg::PathPointWithLaneId& prevPointGlobal,
    crp_msgs::msg::PathWithTrafficRulesArray&      outPaths,
    const float                                    currentCompletePathLength=0,
    const uint16_t                                 currentPointIdx=0
)
{
    crp_msgs::msg::PathWithTrafficRules currentPath = extractLaneletToCompleteLength(startLanelet, currentCompletePathLength, prevPointGlobal, currentPointIdx);

    if (currentPath.path_length >= m_localPathLength)
    {
        // return if current path is long enough
        // this could only happen with the first ever lanelet, because path extracton takes the whole path length into account not just the current lanelet
        outPaths.paths.push_back(currentPath);
        return;
    }

    if (currentPath.path_length + currentCompletePathLength >= m_localPathLength)
    {
        // return if the complete path is long enough
        outPaths.paths.push_back(currentPath);
        return;
    }

    // query next possible lanelets
    lanelet::routing::LaneletPaths paths = m_routingGraphVehicle->possiblePaths(startLanelet, 1, 0, true);

    if (paths.empty())
    {
        // length not reached but no possible paths after lanelet
        outPaths.paths.push_back(currentPath);
        return;
    }

    for (lanelet::routing::LaneletPath selectedPath : paths)
    {
        // add points to path from consecutive possible lanelets
        crp_msgs::msg::PathWithTrafficRulesArray childPaths;
        extractPossiblePaths(selectedPath[1], prevPointGlobal, childPaths, currentCompletePathLength+currentPath.path_length, 0);

        for (crp_msgs::msg::PathWithTrafficRules childPath : childPaths.paths)
        {
            // add child paths to current path
            crp_msgs::msg::PathWithTrafficRules outPath;
            outPath.path.points = currentPath.path.points;
            outPath.path.points.insert(outPath.path.points.end(), childPath.path.points.begin(), childPath.path.points.end());
            outPath.path_length = currentPath.path_length + childPath.path_length;
            outPath.traffic_rules.stop_poses = currentPath.traffic_rules.stop_poses;
            outPath.traffic_rules.stop_poses.insert(outPath.traffic_rules.stop_poses.end(), childPath.traffic_rules.stop_poses.begin(), childPath.traffic_rules.stop_poses.end());

            outPaths.paths.push_back(outPath);
        }
    }
    
    return;
}


void crp::cil::ScenarioAbstraction::publishCallback()
{
    if (!m_isMapLoaded || !m_isGpsTransformSet)
        return;
    
    this->get_parameter("local_path_length", m_localPathLength);

    lanelet::BasicPoint2d currentPoint(m_egoPoseMapFrame.pose.pose.position.x, m_egoPoseMapFrame.pose.pose.position.y);

    // get nearest lanelet to ego
    std::vector<std::pair<double, lanelet::Lanelet>> actuallyNearestLanelets = 
        lanelet::geometry::findNearest(m_laneletMap->laneletLayer, currentPoint, 1);
    lanelet::Lanelet egoLanelet = actuallyNearestLanelets.front().second;

    uint16_t nearestPointIdx = m_abstractionUtils.getGPSNNPointIdx(currentPoint, egoLanelet); // nearest point to ego on lane

    lanelet::ConstLineString2d egoCenterline = egoLanelet.centerline2d();
    tier4_planning_msgs::msg::PathPointWithLaneId prevPoint = m_abstractionUtils.laneletPtToPathPoint(egoCenterline[nearestPointIdx],0);
    
    crp_msgs::msg::PathWithTrafficRulesArray outPaths;
    outPaths.header.stamp = this->now();

    extractPossiblePaths(egoLanelet, prevPoint, outPaths, 0, nearestPointIdx);

    for (crp_msgs::msg::PathWithTrafficRules& path : outPaths.paths)
    {
        m_abstractionUtils.calcPathOrientation(path);
    }

    m_pub_possiblePaths_->publish(outPaths);
}


void crp::cil::ScenarioAbstraction::staticMapFromFileCallback(const autoware_map_msgs::msg::LaneletMapBin::SharedPtr msg)
{
    RCLCPP_INFO(this->get_logger(), "Loading map from topic");
    lanelet::utils::conversion::fromBinMsg(*msg, m_laneletMap);
    m_trafficRulesVehicle = lanelet::traffic_rules::TrafficRulesFactory::create(lanelet::Locations::Germany, lanelet::Participants::Vehicle);
    m_trafficRulesPedestrian = lanelet::traffic_rules::TrafficRulesFactory::create(lanelet::Locations::Germany, lanelet::Participants::Pedestrian);
    m_routingGraphVehicle = lanelet::routing::RoutingGraph::build(*m_laneletMap, *m_trafficRulesVehicle);
    m_routingGraphPedestrian = lanelet::routing::RoutingGraph::build(*m_laneletMap, *m_trafficRulesPedestrian);

    m_mapFrameId = msg->header.frame_id;

    m_isMapLoaded = true;
    RCLCPP_INFO(this->get_logger(), "Map loaded");
}


void crp::cil::ScenarioAbstraction::poseCallback(const geometry_msgs::msg::PoseWithCovarianceStamped::SharedPtr msg)
{
    if (!m_isMapLoaded)
        return;
    if (!m_isGpsTransformSet)
    {
        if (m_mapFrameId != msg->header.frame_id)
        {
            tf2_ros::Buffer tfBuffer(this->get_clock());
            tf2_ros::TransformListener tfListener(tfBuffer);
            m_gps2mapTransform = tfBuffer.lookupTransform(m_mapFrameId, msg->header.frame_id, rclcpp::Time(0), rclcpp::Duration(5, 0));
        }
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
