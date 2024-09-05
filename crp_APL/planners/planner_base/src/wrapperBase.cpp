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
    // TODO: fill the input
    return;
}


void crp::apl::WrapperBase::targetSpaceCallback(const crp_msgs::msg::TargetSpace::SharedPtr msg)
{
    // target pose
    m_input.targetPose.pose.position.x = msg->target_pose.pose.position.x;
    m_input.targetPose.pose.position.y = msg->target_pose.pose.position.y;
    m_input.targetPose.pose.theta = getYawFromQuaternion(msg->target_pose.pose.orientation);

    // free space
    m_input.freeSpace = convertMsgToOccupancyGrid(msg->free_space);

    // traffic rules
    m_input.trafficRules.stoppingPose.pose.position.x = msg->path.traffic_rules.stop_pose.pose.position.x;
    m_input.trafficRules.stoppingPose.pose.position.y = msg->path.traffic_rules.stop_pose.pose.position.y;
    m_input.trafficRules.stoppingPose.pose.theta = getYawFromQuaternion(msg->path.traffic_rules.stop_pose.pose.orientation);
    m_input.trafficRules.laneEdgeTypeLeft = msg->path.traffic_rules.lane_edge_type_left;
    m_input.trafficRules.laneEdgeTypeRight = msg->path.traffic_rules.lane_edge_type_right;
    m_input.maximumSpeed = msg->path.traffic_rules.maximum_speed;

    // path
    m_input.path.pathPoints.clear();
    for (const tier4_planning_msgs::msg::PathPointWithLaneId & pathPoint : msg->path.path.points)
    {
        PlannerInputPose2D inputPose;
        inputPose.pose.position.x = pathPoint.point.pose.position.x;
        inputPose.pose.position.y = pathPoint.point.pose.position.y;
        inputPose.pose.theta = getYawFromQuaternion(pathPoint.point.pose.orientation);
        m_input.path.pathPoints.push_back(inputPose);
    }
    // TODO: decide the function of laneID
    // TODO: targetSpeed

    // TODO: relevant objects
    // TODO: relevant obstacles

    
    // TODO: fill the input
}


void crp::apl::WrapperBase::egoCallback(const crp_msgs::msg::Ego::SharedPtr msg)
{
    // TODO: fill the input
    return;
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
    for (int i = 0; i < msg.info.width; i++)
    {
        std::vector<float> row;
        for (int j = 0; j < msg.info.height; j++)
        {
            row.push_back(msg.data[i*msg.info.width + j]);
        }
        grid.push_back(row);
    }
    return grid;
}


void crp::apl::WrapperBase::outputCPP2ROS(const PlannerOutput & output, autoware_planning_msgs::msg::Trajectory & msg)
{
    for (const auto & outputPoint : output.trajectory)
    {
        autoware_planning_msgs::msg::TrajectoryPoint msgPoint;
        msgPoint.pose.position.x = outputPoint.pose.position.x;
        msgPoint.pose.position.y = outputPoint.pose.position.y;
        tf2::Quaternion(tf2::Vector3(0, 0, 1), outputPoint.pose.theta);
        msgPoint.pose.orientation = tf2::toMsg(tf2::Quaternion(tf2::Vector3(0, 0, 1), outputPoint.pose.theta));
        msgPoint.longitudinal_velocity_mps = outputPoint.velocity;
        msg.points.push_back(msgPoint);
    }
}


void crp::apl::WrapperBase::publishTrajectory(const PlannerOutput & trajectory)
{
    autoware_planning_msgs::msg::Trajectory msg;
    outputCPP2ROS(trajectory, msg);
    m_pub_trajectory_->publish(msg);
}


void crp::apl::WrapperBase::run()
{
    plan(m_input, m_output);
    publishTrajectory(m_output);
}
