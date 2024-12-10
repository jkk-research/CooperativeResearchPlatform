#ifndef CRP_APL_CTRL_VEHICLE_LAT_COMPENSATORY_HPP
#define CRP_APL_CTRL_VEHICLE_LAT_COMPENSATORY_HPP

#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <math.h>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/pose.hpp"
#include "std_msgs/msg/float32_multi_array.hpp"
#include "std_msgs/msg/float32.hpp"
#include "crp_msgs/msg/ego.hpp"

#include "autoware_control_msgs/msg/control.hpp"
#include "autoware_planning_msgs/msg/trajectory.hpp"
#include "autoware_vehicle_msgs/msg/steering_report.hpp"

#include "ctrl_vehicle_control_lat_stanley/controller_inputs.hpp"
#include "ctrl_vehicle_control_lat_stanley/controller_outputs.hpp"

namespace crp
{
    namespace apl
    {
        class CtrlVehicleControlLat : public rclcpp::Node
        {
            public:
                CtrlVehicleControlLat();

            private:
                // VARIABLES
                crp::apl::ControlInput m_input;
                crp::apl::ControlOutput m_output;
                crp::apl::ControlParams m_params;

                void trajCallback(const autoware_planning_msgs::msg::Trajectory input_msg);
                void egoVehicleCallback(const crp_msgs::msg::Ego input_msg);

                void error_calculation(double &lateral_error, double &heading_error);
                void stanleyControl();
                void loop();

                rclcpp::TimerBase::SharedPtr timer_;
                rclcpp::Publisher<autoware_control_msgs::msg::Lateral>::SharedPtr m_pub_cmd;

                rclcpp::Subscription<autoware_planning_msgs::msg::Trajectory>::SharedPtr m_traj_sub_;
                rclcpp::Subscription<crp_msgs::msg::Ego>::SharedPtr m_egoVehicle_sub_;
                autoware_control_msgs::msg::Lateral m_ctrlCmdMsg;
        }; 
    } // namespace apl
}
#endif // CRP_APL_CTRL_VEHICLE_LAT_COMPENSATORY_HPP