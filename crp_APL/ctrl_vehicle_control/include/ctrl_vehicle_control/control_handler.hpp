#ifndef CRP_APL_CTRL_CONTROL_HANDLER_CTRLVEHICLECONTROL_HPP
#define CRP_APL_CTRL_CONTROL_HANDLER_CTRLVEHICLECONTROL_HPP


#include <rclcpp/rclcpp.hpp>

#include "autoware_control_msgs/msg/control.hpp"
#include "autoware_planning_msgs/msg/trajectory.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "crp_msgs/msg/ego.hpp"
#include <visualization_msgs/msg/marker.hpp>

namespace crp
{
    namespace apl
    {

        class ControlHandler : public rclcpp::Node
        {
            public:
                ControlHandler();

            private:
                void controlLatCallback(const autoware_control_msgs::msg::Lateral::SharedPtr msg);
                void controlLongCallback(const autoware_control_msgs::msg::Longitudinal::SharedPtr msg);
                void egoCallback(const crp_msgs::msg::Ego::SharedPtr msg);
                void predictivePathCallback(const autoware_planning_msgs::msg::Trajectory::SharedPtr msg);
                
                void run();
                rclcpp::Subscription<crp_msgs::msg::Ego>::SharedPtr m_sub_ego_;
                rclcpp::Subscription<autoware_control_msgs::msg::Lateral>::SharedPtr m_sub_controlLat_;
                rclcpp::Subscription<autoware_control_msgs::msg::Longitudinal>::SharedPtr m_sub_controlLong_;

                rclcpp::Subscription<autoware_planning_msgs::msg::Trajectory>::SharedPtr m_sub_predictiveControl_;
                
                rclcpp::Publisher<autoware_control_msgs::msg::Control>::SharedPtr m_pub_control_;
                rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr m_pub_twist_;

                rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr m_pub_predictivePathVisualization_;

                rclcpp::TimerBase::SharedPtr m_timer_;    

                geometry_msgs::msg::Twist m_twistCmdMsg;
                autoware_control_msgs::msg::Control m_ctrlCmdMsg;

                float m_currentSteeringTireAngle{0.0f};
                float m_latAccelLim{0u};
                float m_jerkLim{0u};
                float m_wheelBase{0.0f};
        };

    } // namespace apl
} // namespace crp
#endif // CRP_APL_CTRL_CONTROL_HANDLER_CTRLVEHICLECONTROL_HPP
