#ifndef CRP_APL_CTRL_VEHICLE_CONTROL_HPP
#define CRP_APL_CTRL_VEHICLE_CONTROL_HPP

#include <rclcpp/rclcpp.hpp>
#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <math.h>

#include "autoware_control_msgs/msg/control.hpp"
#include "geometry_msgs/msg/twist.hpp"

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
                
                void run();
                rclcpp::Subscription<autoware_control_msgs::msg::Lateral>::SharedPtr m_sub_controlLat_;
                rclcpp::Subscription<autoware_control_msgs::msg::Longitudinal>::SharedPtr m_sub_controlLong_;
                
                rclcpp::Publisher<autoware_control_msgs::msg::Control>::SharedPtr m_pub_control_;
                rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr m_pub_twist_;

                rclcpp::TimerBase::SharedPtr m_timer_;    

                geometry_msgs::msg::Twist m_twistMsg;
                autoware_control_msgs::msg::Control m_ctrlCmdMsg;

                float m_latAccelLim{0u};
                float m_jerkLim{0u};
                float m_wheelBase{0.0f};
        };

    } // namespace apl
} // namespace crp
#endif // CRP_APL_CTRL_VEHICLE_CONTROL_HPP
