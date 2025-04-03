#ifndef CRP_APL_CTRL_VEHICLE_CONTROL_LAT_PURE_P_CTRLVEHICLECONTROLLATPUREP_HPP
#define CRP_APL_CTRL_VEHICLE_CONTROL_LAT_PURE_P_CTRLVEHICLECONTROLLATPUREP_HPP


#include "rclcpp/rclcpp.hpp"

#include "ctrl_vehicle_control_lat_pure_p/controllerParams.hpp"

#include "controllerInterfaces/controllerInterfaces.hpp"
#include "controller_base/ctrlLatWrapperBase.hpp"

namespace crp
{
    namespace apl
    {
        class CtrlVehicleControlLatPureP : public CtrlLatWrapperBase
        {
            public:
                CtrlVehicleControlLatPureP();

            private:
                // VARIABLES
                crp::apl::ControlParams m_params;

                void pure_p_control(const ControlInput & input, LatControlOutput & output);
                void controlLoop(const ControlInput & input, LatControlOutput & output) override;
        }; 
    } // namespace apl
}
#endif // CRP_APL_CTRL_VEHICLE_CONTROL_LAT_PURE_P_CTRLVEHICLECONTROLLATPUREP_HPP