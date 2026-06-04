#include "prcp_sensor_abstraction/scenarioAbstraction.hpp"
#include "crp_srs_if/msg/radar_gen_four_input.hpp"


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

    m_sub_radarInput_ = this->create_subscription<crp_srs_if::msg::RadarGenFourInput>(
    "/crp/radar_gen4_input", 10, std::bind(&ScenarioAbstraction::radarInputCallback, this, std::placeholders::_1));   

    m_pub_movingObjects_   = this->create_publisher<autoware_perception_msgs::msg::PredictedObjects>("cai/local_moving_objects", 10);
    m_pub_obstacles_       = this->create_publisher<autoware_perception_msgs::msg::PredictedObjects>("cai/local_obstacles", 10);
    m_pub_lanePath_        = this->create_publisher<tier4_planning_msgs::msg::PathWithLaneId>("cai/local_lane/path", 10);
    m_pub_drivableSurface_ = this->create_publisher<nav_msgs::msg::OccupancyGrid>("cai/local_drivable_surface", 10);

    m_publishTimer_ = this->create_wall_timer(
        std::chrono::milliseconds(33), std::bind(&ScenarioAbstraction::publishCallback, this));
    
    RCLCPP_INFO(this->get_logger(), "scenario_abstraction has been started");
}

tier4_planning_msgs::msg::PathWithLaneId crp::cil::ScenarioAbstraction::calculateLocalPathFromMap()
{
    tier4_planning_msgs::msg::PathWithLaneId outPath;
    outPath.header.stamp = this->now();

    if (!m_isMapLoaded)
        return outPath;

    float localPathLength;
    this->get_parameter("local_path_length", localPathLength);

    lanelet::BasicPoint2d currentPoint(m_egoPoseMapFrame.pose.pose.position.x, m_egoPoseMapFrame.pose.pose.position.y);

    // get nearest lanelet to ego
    std::vector<std::pair<double, lanelet::Lanelet>> actuallyNearestLanelets = 
        lanelet::geometry::findNearest(m_laneletMap->laneletLayer, currentPoint, 1);
    lanelet::Lanelet egoLanelet = actuallyNearestLanelets.front().second;

    lanelet::traffic_rules::SpeedLimitInformation limit = m_trafficRules->speedLimit(egoLanelet);

    uint16_t nearestPointIdx = m_abstractionUtils.getGPSNNPointIdx(currentPoint, egoLanelet); // nearest point to ego on lane

    

    float currentPathLength = 0;
    float egoLaneletSpeedLimit = limit.speedLimit.value();
    lanelet::ConstLineString2d egoCenterline = egoLanelet.centerline2d();
    tier4_planning_msgs::msg::PathPointWithLaneId prevPoint = m_abstractionUtils.laneletPtToPathPoint(egoCenterline[nearestPointIdx],egoLaneletSpeedLimit); 
    outPath.points.push_back(
        m_abstractionUtils.transformToLocal(
            m_abstractionUtils.laneletPtToPathPoint(egoCenterline[nearestPointIdx],egoLaneletSpeedLimit),
            m_egoPoseMapFrame
        )
    ); // add ego point
    uint16_t currentPointIdx = nearestPointIdx + 1;
    while (currentPathLength < localPathLength && currentPointIdx < egoCenterline.size())
    {
        // add points to path from ego lanelet
        tier4_planning_msgs::msg::PathPointWithLaneId currentPoint = m_abstractionUtils.laneletPtToPathPoint(egoCenterline[currentPointIdx],egoLaneletSpeedLimit);
        currentPathLength += m_abstractionUtils.distanceBetweenPoints(prevPoint, currentPoint);
        outPath.points.push_back(
            m_abstractionUtils.transformToLocal(currentPoint, m_egoPoseMapFrame)
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
            float currentLaneletSpeedLimit = m_trafficRules->speedLimit(currentLanelet).speedLimit.value();

            lanelet::ConstLineString2d currentCenterline = currentLanelet.centerline2d();

            while (currentPathLength < localPathLength && currentPointIdx < currentCenterline.size())
            {
                // add points to path from current lanelet
                tier4_planning_msgs::msg::PathPointWithLaneId currentPoint = m_abstractionUtils.laneletPtToPathPoint(currentCenterline[currentPointIdx],currentLaneletSpeedLimit);
                float distanceBetweenPoints = m_abstractionUtils.distanceBetweenPoints(prevPoint, currentPoint);
                if (distanceBetweenPoints > 0.01f)
                {
                    currentPathLength += m_abstractionUtils.distanceBetweenPoints(prevPoint, currentPoint);
                    outPath.points.push_back(
                        m_abstractionUtils.transformToLocal(currentPoint, m_egoPoseMapFrame)
                    );
                }
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

    // calculate path orientations
    m_abstractionUtils.calcPathOrientation(outPath);

    return outPath;
}


void crp::cil::ScenarioAbstraction::publishCallback()
{
    tier4_planning_msgs::msg::PathWithLaneId outPath=calculateLocalPathFromMap();
    

    m_pub_lanePath_->publish(outPath);
    m_pub_movingObjects_->publish(m_msg_movingObjects);
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
    if (!m_isMapLoaded)
        return;
    if (!m_isGpsTransformSet)
    {
        tf2_ros::Buffer tfBuffer(this->get_clock());
        tf2_ros::TransformListener tfListener(tfBuffer);
        m_gps2mapTransform = tfBuffer.lookupTransform(m_mapFrameId, msg->header.frame_id, rclcpp::Time(0), rclcpp::Duration(5, 0));
        m_isGpsTransformSet = true;
    }
    // transform ego pose to map frame
    tf2::doTransform(*msg, m_egoPoseMapFrame, m_gps2mapTransform);
}
void crp::cil::ScenarioAbstraction::radarInputCallback(const crp_srs_if::msg::RadarGenFourInput::SharedPtr msg)
{
    
    m_msg_movingObjects.header = msg->zzz_header;
    m_msg_movingObjects.header.frame_id = "base_link";
    m_msg_movingObjects.objects.clear();

    // 1. Ego lane
    if (msg->object_distance_ego_lane_m > 0) {
        autoware_perception_msgs::msg::PredictedObject obj;
        
        obj.kinematics.initial_pose_with_covariance.pose.position.x = msg->object_distance_ego_lane_m;
        obj.kinematics.initial_pose_with_covariance.pose.position.y = 0.0;
           
        obj.kinematics.initial_twist_with_covariance.twist.linear.x = msg->object_velocity_ego_lane_mps;
        obj.kinematics.initial_acceleration_with_covariance.accel.linear.x = msg->object_acceleration_ego_lane;
        
        m_msg_movingObjects.objects.push_back(obj);
    }

    // 2. Left lane
    if (msg->object_distance_left_neighbouring_lane_m > 0) {
        autoware_perception_msgs::msg::PredictedObject obj;
       
        obj.kinematics.initial_pose_with_covariance.pose.position.x = msg->object_distance_left_neighbouring_lane_m;
        obj.kinematics.initial_pose_with_covariance.pose.position.y = 0;
        
        obj.kinematics.initial_twist_with_covariance.twist.linear.x = msg->object_velocity_left_neighbouring_lane_mps;
        obj.kinematics.initial_acceleration_with_covariance.accel.linear.x = msg->object_acceleration_left_neighbouring_lane;
        
        m_msg_movingObjects.objects.push_back(obj);
    }

    // 3. right lane
    if (msg->object_distance_right_neighbouring_lane_m > 0) {
        autoware_perception_msgs::msg::PredictedObject obj;
        
        obj.kinematics.initial_pose_with_covariance.pose.position.x = msg->object_distance_right_neighbouring_lane_m;
        obj.kinematics.initial_pose_with_covariance.pose.position.y = 0;
        
        obj.kinematics.initial_twist_with_covariance.twist.linear.x = msg->object_velocity_right_neighbouring_lane_mps;
        obj.kinematics.initial_acceleration_with_covariance.accel.linear.x = msg->object_acceleration_right_neighbouring_lane;
        
        m_msg_movingObjects.objects.push_back(obj);
    }

    
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::cil::ScenarioAbstraction>());
    rclcpp::shutdown();
    return 0;
}
