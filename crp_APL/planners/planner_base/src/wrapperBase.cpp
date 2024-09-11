#include "planner_base/wrapperBase.hpp"


crp::apl::WrapperBase::WrapperBase(const std::string & node_name, const rclcpp::NodeOptions & options)
    : rclcpp::Node(node_name, options)
{
    m_sub_strategy_ = this->create_subscription<tier4_planning_msgs::msg::Scenario>(
        "plan/strategy", 10, std::bind(&WrapperBase::strategyCallback, this, std::placeholders::_1));
    m_sub_target_space_ = this->create_subscription<crp_msgs::msg::TargetSpace>(
        "plan/target_space", 10, std::bind(&WrapperBase::targetSpaceCallback, this, std::placeholders::_1));
    m_sub_ego_ = this->create_subscription<crp_msgs::msg::Ego>(
        "ego", 10, std::bind(&WrapperBase::egoCallback, this, std::placeholders::_1));

    std::string planner_name = node_name.substr(5, node_name.size()-5);
    m_pub_trajectory_ = this->create_publisher<autoware_planning_msgs::msg::Trajectory>("plan/"+planner_name+"/trajectory", 10);

    m_timer_ = this->create_wall_timer(std::chrono::milliseconds(20), std::bind(&WrapperBase::run, this));
}


void crp::apl::WrapperBase::strategyCallback(const tier4_planning_msgs::msg::Scenario::SharedPtr msg)
{
    m_input.currentScenario = msg->current_scenario;
}


void crp::apl::WrapperBase::targetSpaceCallback(const crp_msgs::msg::TargetSpace::SharedPtr msg)
{
    // target pose
    m_input.targetPose.position.x = msg->target_pose.pose.position.x;
    m_input.targetPose.position.y = msg->target_pose.pose.position.y;
    m_input.targetPose.orientation = getYawFromQuaternion(msg->target_pose.pose.orientation);

    // free space
    m_input.freeSpace = convertMsgToOccupancyGrid(msg->free_space);

    // traffic rules
    m_input.trafficRules.stoppingPose.position.x = msg->path.traffic_rules.stop_pose.pose.position.x;
    m_input.trafficRules.stoppingPose.position.y = msg->path.traffic_rules.stop_pose.pose.position.y;
    m_input.trafficRules.stoppingPose.position.z = msg->path.traffic_rules.stop_pose.pose.position.z;
    m_input.trafficRules.stoppingPose.orientation = getYawFromQuaternion(msg->path.traffic_rules.stop_pose.pose.orientation);
    m_input.trafficRules.laneEdgeTypeLeft = msg->path.traffic_rules.lane_edge_type_left;
    m_input.trafficRules.laneEdgeTypeRight = msg->path.traffic_rules.lane_edge_type_right;
    m_input.maximumSpeed = msg->path.traffic_rules.maximum_speed;

    // path
    m_input.path.pathPoints.clear();
    for (const tier4_planning_msgs::msg::PathPointWithLaneId & pathPoint : msg->path.path.points)
    {
        PlannerInputPathPoint inputPoint;
        inputPoint.pose.position.x = pathPoint.point.pose.position.x;
        inputPoint.pose.position.y = pathPoint.point.pose.position.y;
        inputPoint.pose.position.z = pathPoint.point.pose.position.z;
        inputPoint.pose.orientation = getYawFromQuaternion(pathPoint.point.pose.orientation);
        m_input.path.pathPoints.push_back(inputPoint);
        m_input.path.targetSpeed.push_back(pathPoint.point.longitudinal_velocity_mps);
    }
    // TODO: decide the function of laneID

    // relevant objects
    std::vector<PlannerInputObject> relevantObjects;
    for (const autoware_perception_msgs::msg::PredictedObject & object : msg->relevant_objects)
    {
        relevantObjects.push_back(convertMsgToObjects(object));
    }

    // relevant obstacles
    std::vector<PlannerInputObject> relevantObstacles;
    for (const autoware_perception_msgs::msg::PredictedObject & obstacle : msg->relevant_obstacles)
    {
        relevantObstacles.push_back(convertMsgToObjects(obstacle));
    }
}


void crp::apl::WrapperBase::egoCallback(const crp_msgs::msg::Ego::SharedPtr msg)
{
    m_input.egoPose.position.x = msg->pose.pose.position.x;
    m_input.egoPose.position.y = msg->pose.pose.position.y;
    m_input.egoPose.position.z = msg->pose.pose.position.z;
    m_input.egoPose.orientation = getYawFromQuaternion(msg->pose.pose.orientation);

    m_input.egoKinematics.vX = msg->twist.twist.linear.x;
    m_input.egoKinematics.vY = msg->twist.twist.linear.y;
    m_input.egoKinematics.yawRate = msg->twist.twist.angular.z;

    m_input.egoKinematics.aX = msg->accel.accel.linear.x;
    m_input.egoKinematics.aY = msg->accel.accel.linear.y;
    m_input.egoKinematics.yawAcceleration = msg->accel.accel.angular.z;
}


float crp::apl::WrapperBase::getYawFromQuaternion(const geometry_msgs::msg::Quaternion & quaternion)
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


crp::apl::OccupancyGrid crp::apl::WrapperBase::convertMsgToOccupancyGrid(const nav_msgs::msg::OccupancyGrid & msg)
{
    OccupancyGrid grid;
    for (uint32_t i = 0; i < msg.info.width; i++)
    {
        std::vector<float> row;
        for (uint32_t j = 0; j < msg.info.height; j++)
        {
            row.push_back(msg.data[i*msg.info.width + j]);
        }
        grid.push_back(row);
    }
    return grid;
}


crp::apl::PlannerInputObject crp::apl::WrapperBase::convertMsgToObjects(const autoware_perception_msgs::msg::PredictedObject & msg)
{
    PlannerInputObject outObj;

    // convert object ID from uint8[16] to uint32_t
    uint32_t objectId = 0;
    objectId |= static_cast<uint32_t>(msg.object_id.uuid[0]) << 24;
    objectId |= static_cast<uint32_t>(msg.object_id.uuid[1]) << 16;
    objectId |= static_cast<uint32_t>(msg.object_id.uuid[2]) << 8;
    objectId |= static_cast<uint32_t>(msg.object_id.uuid[3]);

    outObj.objectID = objectId;
    outObj.existenceProbability = msg.existence_probability;

    // select the classification with the highest probability
    uint8_t classificationId = 0;
    float maxClassificationProbability = 0.0f;
    for (const autoware_perception_msgs::msg::ObjectClassification & classification : msg.classification)
    {
        if (classification.probability > maxClassificationProbability)
        {
            classificationId = classification.label;
            maxClassificationProbability = classification.probability;
        }
    }
    outObj.classificationID = classificationId;
    outObj.classificationProbability = maxClassificationProbability;
    // pose
    outObj.initialPose.position.x = msg.kinematics.initial_pose_with_covariance.pose.position.x;
    outObj.initialPose.position.y = msg.kinematics.initial_pose_with_covariance.pose.position.y;
    outObj.initialPose.orientation = getYawFromQuaternion(msg.kinematics.initial_pose_with_covariance.pose.orientation);
    // velocity
    outObj.objectKinematics.vX = msg.kinematics.initial_twist_with_covariance.twist.linear.x;
    outObj.objectKinematics.vY = msg.kinematics.initial_twist_with_covariance.twist.linear.y;
    outObj.objectKinematics.yawRate = msg.kinematics.initial_twist_with_covariance.twist.angular.z;
    // acceleration
    outObj.objectKinematics.aX = msg.kinematics.initial_acceleration_with_covariance.accel.linear.x;
    outObj.objectKinematics.aY = msg.kinematics.initial_acceleration_with_covariance.accel.linear.y;
    outObj.objectKinematics.yawAcceleration = msg.kinematics.initial_acceleration_with_covariance.accel.angular.z;
    // predicted path (with the highest confidence)
    outObj.predictedPath.pathPoints.clear();
    if (msg.kinematics.predicted_paths.size() > 0)
    {
        // select the path with the highest confidence
        uint8_t maxConfidenceId = msg.kinematics.predicted_paths[0].confidence;
        float maxConfidence = 0.0f;
        for (uint8_t i = 1; i < msg.kinematics.predicted_paths.size(); i++)
        {
            const autoware_perception_msgs::msg::PredictedPath & path = msg.kinematics.predicted_paths.at(i);
            if (path.confidence > maxConfidence)
            {
                maxConfidence = path.confidence;
                maxConfidenceId = i;
            }
        }
        for (const geometry_msgs::msg::Pose & pathPoint : msg.kinematics.predicted_paths.at(maxConfidenceId).path)
        {
            PlannerInputPathPoint inputPoint;
            inputPoint.pose.position.x = pathPoint.position.x;
            inputPoint.pose.position.y = pathPoint.position.y;
            inputPoint.pose.position.z = pathPoint.position.z;
            inputPoint.pose.orientation = getYawFromQuaternion(pathPoint.orientation);
            outObj.predictedPath.pathPoints.push_back(inputPoint);
        }
    }
    // shape
    outObj.shape.type = static_cast<Shape::ShapeType>(msg.shape.type);
    for (const geometry_msgs::msg::Point32 & shapePoint : msg.shape.footprint.points)
    {
        Point3D point;
        point.x = shapePoint.x;
        point.y = shapePoint.y;
        point.z = shapePoint.z;
        outObj.shape.polygon.push_back(point);
    }
    outObj.shape.dimensions.x = msg.shape.dimensions.x;
    outObj.shape.dimensions.y = msg.shape.dimensions.y;
    outObj.shape.dimensions.z = msg.shape.dimensions.z;

    return outObj;
}


void crp::apl::WrapperBase::convertOutputToMsg(const PlannerOutput & output, autoware_planning_msgs::msg::Trajectory & msg)
{
    for (const auto & outputPoint : output.trajectory)
    {
        autoware_planning_msgs::msg::TrajectoryPoint msgPoint;
        msgPoint.pose.position.x = outputPoint.pose.position.x;
        msgPoint.pose.position.y = outputPoint.pose.position.y;
        tf2::Quaternion(tf2::Vector3(0, 0, 1), outputPoint.pose.orientation);
        msgPoint.pose.orientation = tf2::toMsg(tf2::Quaternion(tf2::Vector3(0, 0, 1), outputPoint.pose.orientation));
        msgPoint.longitudinal_velocity_mps = outputPoint.velocity;
        msg.points.push_back(msgPoint);
    }
}


void crp::apl::WrapperBase::publishTrajectory(const PlannerOutput & trajectory)
{
    autoware_planning_msgs::msg::Trajectory msg;
    convertOutputToMsg(trajectory, msg);
    m_pub_trajectory_->publish(msg);
}


void crp::apl::WrapperBase::run()
{
    plan(m_input, m_output);
    publishTrajectory(m_output);
}
