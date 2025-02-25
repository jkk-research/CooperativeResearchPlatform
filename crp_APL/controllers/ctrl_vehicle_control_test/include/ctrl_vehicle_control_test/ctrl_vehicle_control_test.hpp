#ifndef CRP_APL_CTRL_VEHICLE_LAT_TEST_HPP
#define CRP_APL_CTRL_VEHICLE_LAT_TEST_HPP


#include "rclcpp/rclcpp.hpp"
#include "crp_msgs/msg/ego.hpp"

#include "autoware_control_msgs/msg/control.hpp"
#include "autoware_planning_msgs/msg/trajectory.hpp"

namespace crp
{
    namespace apl
    {
        class CtrlVehicleControlTest : public rclcpp::Node
        {
            public:
                CtrlVehicleControlTest();

            private:
                float m_target_speed{0.0f};
                float m_egoSpeed{0.0f};
                float m_steeringAngle{0.0f};

                float p_stepAmplitude{0.0f};
                float p_sinusAmplitude{0.0f};
                float p_sinusFrequency{0.0f};

                float m_steeringAngleTarget{0.0f};

                float t{0.0f};
                float dT{0.01};

                void trajCallback(const autoware_planning_msgs::msg::Trajectory input_msg);
                void egoVehicleCallback(const crp_msgs::msg::Ego input_msg);
                void calculateSteeringAngle();

                void loop();

                rclcpp::TimerBase::SharedPtr m_timer_;
                rclcpp::Publisher<autoware_control_msgs::msg::Lateral>::SharedPtr m_pub_cmdLat;
                rclcpp::Publisher<autoware_control_msgs::msg::Longitudinal>::SharedPtr m_pub_cmdLong;

                rclcpp::Subscription<autoware_planning_msgs::msg::Trajectory>::SharedPtr m_sub_traj_;
                rclcpp::Subscription<crp_msgs::msg::Ego>::SharedPtr m_sub_egoVehicle_;
                autoware_control_msgs::msg::Lateral m_ctrlCmdLatMsg;
                autoware_control_msgs::msg::Longitudinal m_ctrlCmdLongMsg;
        }; 
    } // namespace apl
}
#endif // CRP_APL_CTRL_VEHICLE_LAT_TEST_HPP