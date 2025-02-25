#include "ctrl_vehicle_control_long.hpp"


crp::apl::CtrlVehicleControlLong::CtrlVehicleControlLong() : Node("ctrl_vehicle_control_long")
{
    m_timer_ = this->create_wall_timer(std::chrono::milliseconds(33), std::bind(&CtrlVehicleControlLong::run, this));  
    m_pub_control_ = this->create_publisher<autoware_control_msgs::msg::Longitudinal>("/control/command/control_cmdLong", 30);

    m_sub_ego_ = this->create_subscription<crp_msgs::msg::Ego>("/ego", 10, std::bind(&CtrlVehicleControlLong::egoCallback, this, std::placeholders::_1));
    m_sub_trajectory_ = this->create_subscription<autoware_planning_msgs::msg::Trajectory>("/plan/trajectory", 10, std::bind(&CtrlVehicleControlLong::trajectoryCallback, this, std::placeholders::_1));
    m_sub_behaviorLimits_ = this->create_subscription<crp_msgs::msg::BehaviorLimits>("/plan/behavior_limits", 10, std::bind(&CtrlVehicleControlLong::behaviorLimitsCallback, this, std::placeholders::_1));

    this->declare_parameter("/ctrl/long/speedControlLookAheadTime", 0.6f);

    RCLCPP_INFO(this->get_logger(), "ctrl_vehicle_control_long has been started");
}

void crp::apl::CtrlVehicleControlLong::trajectoryCallback(const autoware_planning_msgs::msg::Trajectory::SharedPtr msg)
{
    // get the point which is in the look ahead distance
    double lookAheadDistance = m_egoSpeed*p_speedControlLookAheadTime; // in meters
    
    if (msg->points.size() == 0U)
    {
        m_ctrl_msg.velocity = 0.0f;
    }
    else{
        m_ctrl_msg.velocity = 
                    msg->points.at(msg->points.size()-1U).longitudinal_velocity_mps;
    }
    
    for (long unsigned int wp=0U; wp<msg->points.size(); wp++)
    {
        double distance = sqrt(std::pow(msg->points.at(wp).pose.position.x, 2) +
            std::pow(msg->points.at(wp).pose.position.y, 2));
        
        if(distance >= lookAheadDistance)
        {
            m_ctrl_msg.velocity = msg->points.at(wp).longitudinal_velocity_mps;
            break;
        }
    }
}

void crp::apl::CtrlVehicleControlLong::egoCallback(const crp_msgs::msg::Ego::SharedPtr msg)
{
    m_egoSpeed = msg->twist.twist.linear.x;
}

void crp::apl::CtrlVehicleControlLong::behaviorLimitsCallback(const crp_msgs::msg::BehaviorLimits::SharedPtr msg)
{
    p_axMin = msg->ax_min.data;
    p_axMax = msg->ax_max.data;
    p_jxMin = msg->jx_min.data;
    p_jxMax = msg->jx_max.data;
}

void crp::apl::CtrlVehicleControlLong::run()
{
    // parameter read in
    p_speedControlLookAheadTime = this->get_parameter("/ctrl/long/speedControlLookAheadTime").as_double();

    m_pub_control_->publish(m_ctrl_msg);
}    

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::apl::CtrlVehicleControlLong>());
    rclcpp::shutdown();
    return 0;
}