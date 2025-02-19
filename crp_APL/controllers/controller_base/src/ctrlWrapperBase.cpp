#include "controller_base/ctrlWrapperBase.hpp"


crp::apl::CtrlWrapperBase::CtrlWrapperBase(const std::string & node_name, const rclcpp::NodeOptions & options)
    : rclcpp::Node(node_name, options)
{
    m_sub_trajectory_ = this->create_subscription<autoware_planning_msgs::msg::Trajectory>("/plan/trajectory", 10, std::bind(&CtrlWrapperBase::trajectoryCallback, this, std::placeholders::_1));
    m_sub_ego_        = this->create_subscription<crp_msgs::msg::Ego>("/ego", 10, std::bind(&CtrlWrapperBase::egoCallback, this, std::placeholders::_1));

    m_timer_ = this->create_wall_timer(std::chrono::milliseconds(33), std::bind(&CtrlWrapperBase::run, this));
}

void crp::apl::CtrlWrapperBase::trajectoryCallback(const autoware_planning_msgs::msg::Trajectory input_msg)
{
    m_input.pathX.clear();
    m_input.pathY.clear();
    m_input.pathTheta.clear();

    // this callback maps the input trajectory to the internal interface
    for (long unsigned int i=0; i<input_msg.points.size(); i++)
    {      
        m_input.pathX.push_back(input_msg.points.at(i).pose.position.x);
        m_input.pathY.push_back(input_msg.points.at(i).pose.position.y);
        m_input.pathTheta.push_back(m_dataConversion.quaternionToEuler(
            input_msg.points.at(i).pose.orientation.x,
            input_msg.points.at(i).pose.orientation.y,
            input_msg.points.at(i).pose.orientation.z,
            input_msg.points.at(i).pose.orientation.w
        ).at(2));
    }

    if (input_msg.points.size() > 0)
        m_input.targetSpeed = input_msg.points.at(0).longitudinal_velocity_mps;
    else
        m_input.targetSpeed = 0.0f;
}

void crp::apl::CtrlWrapperBase::egoCallback(const crp_msgs::msg::Ego input_msg)
{
    m_input.vxEgo = input_msg.twist.twist.linear.x;
    m_input.egoSteeringAngle = input_msg.tire_angle_front;

    m_input.egoPoseGlobal[0] = input_msg.pose.pose.position.x;
    m_input.egoPoseGlobal[1] = input_msg.pose.pose.position.y;
    double theta = m_dataConversion.quaternionToEuler(
        input_msg.pose.pose.orientation.x,
        input_msg.pose.pose.orientation.y,
        input_msg.pose.pose.orientation.z,
        input_msg.pose.pose.orientation.w
    ).at(2);
    m_input.egoPoseGlobal[2] = theta;
}
