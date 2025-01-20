#ifndef CRP_APL_CTRL_VEHICLE_LAT_COMPENSATORY_CTRLVEHILECONTROLLATCOMPENSATORY_HPP
#define CRP_APL_CTRL_VEHICLE_LAT_COMPENSATORY_CTRLVEHILECONTROLLATCOMPENSATORY_HPP


#include "rclcpp/rclcpp.hpp"
#include "crp_msgs/msg/ego.hpp"

#include "autoware_control_msgs/msg/control.hpp"
#include "autoware_planning_msgs/msg/trajectory.hpp"

#include "ctrl_vehicle_control/controller_inputs.hpp"
#include "ctrl_vehicle_control/controller_outputs.hpp"
#include "lib/utils.hpp"
#include "compensatory_model/compensatory_model.hpp"

namespace crp
{
    namespace apl
    {
        class CtrlVehicleControlLatCompensatory : public rclcpp::Node
        {
            public:
                CtrlVehicleControlLatCompensatory();

            private:
                // VARIABLES
                crp::apl::ControlInput m_input;
                crp::apl::ControlOutput m_output;
                crp::apl::ControlParams m_params;
                crp::apl::GeometricOperators m_geometricOperator;
                crp::apl::CompensatoryModel m_compensatoryModel;

                void trajCallback(const autoware_planning_msgs::msg::Trajectory input_msg);
                void egoVehicleCallback(const crp_msgs::msg::Ego input_msg);

                void loop();

                rclcpp::TimerBase::SharedPtr m_timer_;
                rclcpp::Publisher<autoware_control_msgs::msg::Lateral>::SharedPtr m_pub_cmd;

                rclcpp::Subscription<autoware_planning_msgs::msg::Trajectory>::SharedPtr m_sub_traj_;
                rclcpp::Subscription<crp_msgs::msg::Ego>::SharedPtr m_sub_egoVehicle_;
                autoware_control_msgs::msg::Lateral m_ctrlCmdMsg;
        }; 
    } // namespace apl
}
#endif // CRP_APL_CTRL_VEHICLE_LAT_COMPENSATORY_CTRLVEHILECONTROLLATCOMPENSATORY_HPP