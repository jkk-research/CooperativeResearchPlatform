#ifndef CRP_APL_CTRL_VEHICLE_CONTROL_LONG_CTRLVEHICLECONTROLLONG_HPP
#define CRP_APL_CTRL_VEHICLE_CONTROL_LONG_CTRLVEHICLECONTROLLONG_HPP


#include "rclcpp/rclcpp.hpp"

#include "crp_msgs/msg/ego.hpp"
#include "crp_msgs/msg/behavior_limits.hpp"
#include "autoware_control_msgs/msg/control.hpp"
#include "autoware_planning_msgs/msg/trajectory.hpp"


namespace crp
{
    namespace apl
    {

        class CtrlVehicleControlLong : public rclcpp::Node
        {
            public:
                CtrlVehicleControlLong();

            private:
                void trajectoryCallback(const autoware_planning_msgs::msg::Trajectory::SharedPtr msg);
                void egoCallback(const crp_msgs::msg::Ego::SharedPtr msg);
                void behaviorLimitsCallback(const crp_msgs::msg::BehaviorLimits::SharedPtr msg);
                
                void run();

                rclcpp::TimerBase::SharedPtr m_timer_;
                rclcpp::Publisher<autoware_control_msgs::msg::Longitudinal>::SharedPtr m_pub_control_;

                rclcpp::Subscription<autoware_planning_msgs::msg::Trajectory>::SharedPtr m_sub_trajectory_;
                rclcpp::Subscription<crp_msgs::msg::Ego>::SharedPtr m_sub_ego_;
                rclcpp::Subscription<crp_msgs::msg::BehaviorLimits>::SharedPtr m_sub_behaviorLimits_;
                
                autoware_control_msgs::msg::Longitudinal m_ctrl_msg;

                // member parameters
                double p_axMax{0.0f};
                double p_axMin{0.0f};
                double p_jxMax{0.0f};
                double p_jxMin{0.0f};
                double p_speedControlLookAheadTime{0.0f};

                double dT = 0.033; // in seconds

                // member variables
                double m_egoSpeed{0.0f}; // in m/s
        };

    } // namespace apl
} // namespace crp
#endif // CRP_APL_CTRL_VEHICLE_CONTROL_LONG_CTRLVEHICLECONTROLLONG_HPP
