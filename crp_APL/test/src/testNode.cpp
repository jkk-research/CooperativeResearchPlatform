#include "testNode/testNode.hpp"


crp::apl::TestNode::TestNode() : Node("test_node")
{
    m_sub_ctrlCmd_   = this->create_subscription<autoware_control_msgs::msg::Control>(
        "/control/command/control_cmd", 10, std::bind(&TestNode::controlCommandCallback, this, std::placeholders::_1));

    // new setup
    m_pub_movingObjects_   = this->create_publisher<autoware_perception_msgs::msg::PredictedObjects>("cai/local_moving_objects", 10);
    m_pub_obstacles_       = this->create_publisher<autoware_perception_msgs::msg::PredictedObjects>("cai/local_obstacles", 10);
    m_pub_lanePath_        = this->create_publisher<tier4_planning_msgs::msg::PathWithLaneId>("cai/local_lane/path", 10);
    m_pub_drivableSurface_ = this->create_publisher<nav_msgs::msg::OccupancyGrid>("cai/local_drivable_surface", 10);

    m_pub_kinematicState_ = this->create_publisher<autoware_localization_msgs::msg::KinematicState>("cai/kinematic_state", 10);
    m_pub_egoStatus_      = this->create_publisher<crp_msgs::msg::EgoStatus>("cai/ego_status", 10);

    m_pub_behavior_       = this->create_publisher<crp_msgs::msg::Behavior>("/ui/behavior", 10);

    this->declare_parameter("/test/maximumSpeedInit", 20.0f); // in m/s
    this->declare_parameter("/test/previewDistance", 100.0f); // in m


    m_timer_ = this->create_wall_timer(std::chrono::milliseconds(20), std::bind(&TestNode::run, this));
}

void crp::apl::TestNode::controlCommandCallback(const autoware_control_msgs::msg::Control::SharedPtr msg)
{
    m_roadWheelAngleTarget = msg->lateral.steering_tire_angle;
    m_vehicleSpeedTarget = msg->longitudinal.velocity;
    return;
}

void crp::apl::TestNode::vehicleModel()
{
    m_vxEgo = m_vehicleSpeedTarget;
    // single track kinematic model of the vehicle
    float yawRate = m_vehicleSpeedTarget*std::tan(m_roadWheelAngleTarget)/p_wheelBase; // ackermann formula: tan(delta) = L*kappa, yawRate = vx*kappa
    x[2] = x[2]+dT*yawRate; // global orientation increase
    float dx = m_vxEgo*std::cos(x[2])*dT;
    float dy = m_vxEgo*std::sin(x[2])*dT;
    x[0] = x[0] + dx*dT;
    x[1] = x[1] + dy*dT;

    m_ayEgo = yawRate*m_vxEgo;

    return;
}

float* crp::apl::TestNode::globalToEgoTransform(float globalPose[3])
{
    // global to ego transform. First, translation with xEgo and yEgo, then rotation
    static float localPose[3];
    localPose[2] = globalPose[2] - x[2]; // orientation is simply rotated
    // translation:
    float xLocalBeforeRotation = globalPose[0] - x[0];
    float yLocalBeforeRotation = globalPose[1] - x[1];
    // rotation:
    localPose[0] = std::cos(x[2])*xLocalBeforeRotation+std::sin(x[2])*yLocalBeforeRotation;
    localPose[1] = -1.0*std::sin(x[2])*xLocalBeforeRotation+std::cos(x[2])*yLocalBeforeRotation;

    return localPose;
}

void crp::apl::TestNode::calculateRoute()
{
    // this method calculates the route and prepares it for the scenario mapping
    m_localPath_x.clear();
    m_localPath_y.clear();
    m_localPath_theta.clear();

    float dx = 1.0;
    float xGlobal[1001];
    float yGlobal[1001];
    float thetaGlobal[1001];
    float c0 = 0.0; float c1 = 0.2; float c2 = 0.005; float c3 = -0.00002;
    for (int i=0; i<1001; i++)
    {
        xGlobal[i] = i*dx;
        yGlobal[i] = c0+c1*xGlobal[i]+c2*std::pow(xGlobal[i],2)+c3*std::pow(xGlobal[i],3);
        thetaGlobal[i] = std::atan(c1+c2*2.0*xGlobal[i]+3.0*c3*pow(xGlobal[i],2));
    }
    // TODO: put global path to world message 

    // now turning it to local coordinates
    for (int i=0; i<1001; i++)
    {
        float globalPoint[3];
        globalPoint[0] = xGlobal[i]; globalPoint[1] = yGlobal[i]; globalPoint[2] = thetaGlobal[i];
        float* localPoint;
        localPoint = globalToEgoTransform(globalPoint);
        m_localPath_x.push_back(localPoint[0]);
        m_localPath_y.push_back(localPoint[1]);
        m_localPath_theta.push_back(localPoint[2]);
    }

    return;
}

float crp::apl::TestNode::getYawFromQuaternion(const geometry_msgs::msg::Quaternion & quaternion)
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

void crp::apl::TestNode::calculateSpeed()
{
    if(m_vehicleTooFarFromPath || m_noEnoughPointsLeft)
    {
        m_maximumSpeed = 0.0;
    }
    else
    {
        m_maximumSpeed = m_maximumSpeedInit;
    }
}

void crp::apl::TestNode::mapPath()
{
    float currentPathLength = 0;
    m_outPathMsg.points.clear();
    m_noEnoughPointsLeft = true;
    for (int i=0; i<m_localPath_x.size(); i++)
    {
        if (m_localPath_x.at(i)>=0)
        {
            // add points to path from simulated path
            tier4_planning_msgs::msg::PathPointWithLaneId currentPoint;
            currentPoint.point.pose.position.x = m_localPath_x.at(i);
            currentPoint.point.pose.position.y = m_localPath_y.at(i);
            tf2::Quaternion quat;
            quat.setRPY(0.0f, 0.0f, m_localPath_theta.at(i));
            currentPoint.point.pose.orientation = tf2::toMsg(quat);

            currentPoint.point.longitudinal_velocity_mps = m_maximumSpeed;

            float ds;
            if (i==0)
            {
                ds = std::pow(std::pow(m_localPath_x.at(i),2)+
                    std::pow(m_localPath_y.at(i),2),0.5);
            }
            else
            {
                ds = std::pow(std::pow(m_localPath_x.at(i)-m_localPath_x.at(i-1),2)+
                    std::pow(m_localPath_y.at(i)-m_localPath_y.at(i-1),2),0.5);
            }


    {
        m_maximumSpeed = 0.0;
    }
    else
    {
        m_maximumSpeed = m_maximumSpeedInit;
    }
}

void crp::apl::TestNode::mapPath()
{
    float currentPathLength = 0;
    m_outPathMsg.points.clear();
    m_outPathMsg.header.stamp = this->now();
    m_noEnoughPointsLeft = true;
    for (int i=0; i<m_localPath_x.size(); i++)
    {
        if (m_localPath_x.at(i)>=0)
        {
            // add points to path from simulated path
            tier4_planning_msgs::msg::PathPointWithLaneId currentPoint;
            currentPoint.point.pose.position.x = m_localPath_x.at(i);
            currentPoint.point.pose.position.y = m_localPath_y.at(i);
            tf2::Quaternion quat;
            quat.setRPY(0.0f, 0.0f, m_localPath_theta.at(i));
            currentPoint.point.pose.orientation = tf2::toMsg(quat);

            currentPoint.point.longitudinal_velocity_mps = m_maximumSpeed;

            float ds;
            if (i==0)
            {
                ds = std::pow(std::pow(m_localPath_x.at(i),2)+
                    std::pow(m_localPath_y.at(i),2),0.5);
            }
            else
            {
                ds = std::pow(std::pow(m_localPath_x.at(i)-m_localPath_x.at(i-1),2)+
                    std::pow(m_localPath_y.at(i)-m_localPath_y.at(i-1),2),0.5);
            }

            currentPathLength = currentPathLength+ds;

            m_outPathMsg.points.push_back(
                currentPoint
            );

            if(std::abs(m_outPathMsg.points.at(0).point.pose.position.y > 3.0))
            {
                m_vehicleTooFarFromPath = true;
            }
            
            if (currentPathLength > 100.0)
            {
                m_noEnoughPointsLeft = false;
                break;
            }
        }
    }
    printf("current path length %f\n", currentPathLength);

    return;
}

void crp::apl::TestNode::mapObstacles()
{
    // to be filled up
    return;
}

void crp::apl::TestNode::mapMovingObjects()
{
    // to be filled up
    return;
}

void crp::apl::TestNode::mapDrivableSurface()
{
    // to be filled up
    return;
}

void crp::apl::TestNode::mapEgo()
{
    m_egoStatus.tire_angle_front = m_roadWheelAngleTarget;

    m_egoStatus.header.stamp = this->now();
    m_egoStatus.tire_angle_front = m_roadWheelAngleTarget;

    m_kinematicState.header.stamp = this->now();
    m_kinematicState.pose_with_covariance.pose.position.x = x[0];
    m_kinematicState.pose_with_covariance.pose.position.y = x[1];
    tf2::Quaternion(tf2::Vector3(0, 0, 1), x[2]);
    m_kinematicState.pose_with_covariance.pose.orientation = 
        tf2::toMsg(tf2::Quaternion(tf2::Vector3(0, 0, 1), x[2])); 
  
    m_kinematicState.twist_with_covariance.twist.linear.x = m_vehicleSpeedTarget;
  
    m_kinematicState.twist_with_covariance.twist.linear.x = m_vxEgo;

    m_kinematicState.accel_with_covariance.accel.linear.x = m_axEgo;
    m_kinematicState.accel_with_covariance.accel.linear.x = m_ayEgo;

    return;
}

void crp::apl::TestNode::run()
{
    // params
    m_maximumSpeedInit = this->get_parameter("/test/maximumSpeedInit").as_double();
    p_previewDistance = this->get_parameter("/test/previewDistance").as_double();

    // update vehicle position
    vehicleModel();
    // calculate local path
    calculateRoute();

    calculateSpeed();

    // finally, map results to output messages
    mapPath();
    mapDrivableSurface();
    mapMovingObjects();
    mapObstacles();

    mapEgo();


    m_behavior.header.stamp = this->now();

    m_behavior.target_speed.data = m_maximumSpeedInit;

    // new publishing
    m_pub_movingObjects_->publish(m_movingObjectsMsg);
    m_pub_obstacles_->publish(m_movingObjectsMsg);
    m_pub_lanePath_->publish(m_outPathMsg);
    m_pub_drivableSurface_->publish(m_drivableSurfaceMsg);

    m_pub_kinematicState_->publish(m_kinematicState);
    m_pub_egoStatus_->publish(m_egoStatus);

    m_pub_behavior_->publish(m_behavior);
}


int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::apl::TestNode>());
    rclcpp::shutdown();
    return 0;
}