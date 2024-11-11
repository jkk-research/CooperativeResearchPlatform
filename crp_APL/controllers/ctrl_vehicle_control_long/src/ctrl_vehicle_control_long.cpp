#include "ctrl_vehicle_control_long.hpp"

using namespace std::chrono_literals;
using std::placeholders::_1;


crp::apl::CtrlVehicleControlLong::CtrlVehicleControlLong() : Node("CtrlVehicleControlLong")
{
    m_timer_ = this->create_wall_timer(std::chrono::milliseconds(33), std::bind(&CtrlVehicleControlLong::run, this));  
    m_pub_control_ = this->create_publisher<autoware_control_msgs::msg::Control>("/control/command/control_cmdLong", 30);

    m_sub_ego_ = this->create_subscription<crp_msgs::msg::Ego>("/ego", 10, std::bind(&CtrlVehicleControlLong::egoCallback, this, std::placeholders::_1));
    m_sub_trajectory_ = this->create_subscription<autoware_planning_msgs::msg::Trajectory>("/plan/trajectory", 10, std::bind(&CtrlVehicleControlLong::trajectoryCallback, this, std::placeholders::_1));

    this->declare_parameter("/ctrl/axMax", 2.0f);
    this->declare_parameter("/ctrl/axMin", -2.0f);
    this->declare_parameter("/ctrl/jxMax", 1.0f);
    this->declare_parameter("/ctrl/jxMin", -1
    this->declare_parameter("/ctrl/speedControlLookAheadTime", 0.6f);

    RCLCPP_INFO(this->get_logger(), "ctrl_vehicle_control_long has been started");
}

void crp::apl::CtrlVehicleControlLong::trajectoryCallback(const autoware_planning_msgs::msg::Trajectory::SharedPtr msg)
{
    // get the point which is in the look ahead distance
    double lookAheadDistance = m_egoSpeed*p_speedControlLookAheadTime; // in meters
    
    if (msg->points.size() == 0U)
    {
        m_ctrl_msg.longitudinal.velocity = 0.0f;
    }
    else{
        m_ctrl_msg.longitudinal.velocity = 
                    msg->points.at(msg->points.size()-1U).longitudinal_velocity_mps;
        //m_ctrl_msg.longitudinal.velocity = msg->points.at(0U).longitudinal_velocity_mps;
    }
    
    for (long unsigned int wp=0U; wp<msg->points.size(); wp++)
    {
        double distance = sqrt(std::pow(msg->points.at(wp).pose.position.x, 2) +
            std::pow(msg->points.at(wp).pose.position.y, 2));
        
        if(distance >= lookAheadDistance)
        {
            // filtering with acceleration
            if(msg->points.at(wp).longitudinal_velocity_mps >= (m_egoSpeed+p_axMax*dT))
            {
                m_ctrl_msg.longitudinal.velocity = m_egoSpeed+p_axMax*dT;  
            }
            else if(msg->points.at(wp).longitudinal_velocity_mps <= (m_egoSpeed+p_axMin*dT))
            {
                if (m_egoSpeed+p_axMin*dT < 2.0)
                {
                    m_ctrl_msg.longitudinal.velocity = 0.0;
                }
                else
                {
                    m_ctrl_msg.longitudinal.velocity = m_egoSpeed+p_axMin*dT;
                }
            }
            else
            {
                m_ctrl_msg.longitudinal.velocity = 
                    msg->points.at(wp).longitudinal_velocity_mps;
            }
            break;
        }
    }
}

void crp::apl::CtrlVehicleControlLong::egoCallback(const crp_msgs::msg::Ego::SharedPtr msg)
{
    m_egoSpeed = msg->twist.twist.linear.x;
}

void crp::apl::CtrlVehicleControlLong::run()
{
    // parameter read in
    p_axMax = this->get_parameter("/ctrl/axMax").as_double();
    p_axMin = this->get_parameter("/ctrl/axMin").as_double();
    p_jxMax = this->get_parameter("/ctrl/jxMax").as_double();
    p_jxMin = this->get_parameter("/ctrl/jxMin").as_double();
    p_speedControlLookAheadTime = this->get_parameter("/ctrl/speedControlLookAheadTime").as_double();

    m_pub_control_->publish(m_ctrl_msg);
}    

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::apl::CtrlVehicleControlLong>());
    rclcpp::shutdown();
    return 0;
}