#include "testNode/testNode.hpp"


crp::apl::TestNode::TestNode() : Node("test_node")
{
    m_sub_ctrlCmd_   = this->create_subscription<autoware_control_msgs::msg::Control>(
        "/control/command/ctrl_cmd", 10, std::bind(&TestNode::controlCommandCallback, this, std::placeholders::_1));

    m_pub_world_    = this->create_publisher<crp_msgs::msg::World>("world", 1);
    m_pub_scenario_ = this->create_publisher<crp_msgs::msg::Scenario>("scenario", 1);
    m_pub_ego_      = this->create_publisher<crp_msgs::msg::Ego>("ego", 1);

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
    // single track kinematic model of the vehicle
    float yawRate = m_vehicleSpeedTarget*std::tan(m_roadWheelAngleTarget)/p_wheelBase; // ackermann formula: tan(delta) = L*kappa, yawRate = vx*kappa
    x[2] = x[2]+dT*yawRate; // global orientation increase
    float dx = m_vehicleSpeedTarget*std::cos(x[2])*dT;
    float dy = m_vehicleSpeedTarget*std::sin(x[2])*dT;
    x[0] = x[0] + dx*dT;
    x[1] = x[1] + dy*dT;
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

    // no turning it to local coordinates
    for (int i=0; i<1001; i++)
    {
        float globalPoint[3];
        globalPoint[0] = xGlobal[i]; globalPoint[1] = yGlobal[i]; globalPoint[2] = thetaGlobal[i];
        float* localPoint;
        localPoint = globalToEgoTransform(globalPoint);
        m_localPath[i][0] = localPoint[0];
        m_localPath[i][1] = localPoint[1];
        m_localPath[i][2] = localPoint[2];
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

void crp::apl::TestNode::mapScenario()
{
    // mapping the necessary signals to the map message
    m_scenarioMsg.paths.clear();
    crp_msgs::msg::PathWithTrafficRules path;
    tier4_planning_msgs::msg::PathPointWithLaneId pathPoint;
    for (int i=0; i<1001; i++)
    {
        pathPoint.point.pose.position.x = m_localPath[i][0];
        pathPoint.point.pose.position.y = m_localPath[i][1];
        tf2::Quaternion(tf2::Vector3(0, 0, 1), m_localPath[i][2]);
        pathPoint.point.pose.orientation = tf2::toMsg(tf2::Quaternion(tf2::Vector3(0, 0, 1), m_localPath[i][2]));

        pathPoint.point.longitudinal_velocity_mps = 20.0;

        path.path.points.push_back(pathPoint);
    }
    path.traffic_rules.maximum_speed = 20.0;
    // only one lane at the moment
    m_scenarioMsg.paths.push_back(path);
    
    return;
}

void crp::apl::TestNode::mapEgo()
{
    m_egoMsg.pose.pose.position.x = x[0];
    m_egoMsg.pose.pose.position.y = x[1];
    tf2::Quaternion(tf2::Vector3(0, 0, 1), x[2]);
    m_egoMsg.pose.pose.orientation = tf2::toMsg(tf2::Quaternion(tf2::Vector3(0, 0, 1), x[2]));

    m_egoMsg.twist.twist.linear.x = m_vehicleSpeedTarget;

    m_egoMsg.tire_angle_front = m_roadWheelAngleTarget;

    return;
}

void crp::apl::TestNode::run()
{
    // update vehicle position
    vehicleModel();
    // calculate local path
    calculateRoute();

    // finally, map results to output messages
    mapScenario();
    mapEgo();

    m_pub_world_->publish(m_worldMsg);
    m_pub_scenario_->publish(m_scenarioMsg);
    m_pub_ego_->publish(m_egoMsg);
}


int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::apl::TestNode>());
    rclcpp::shutdown();
    return 0;
}