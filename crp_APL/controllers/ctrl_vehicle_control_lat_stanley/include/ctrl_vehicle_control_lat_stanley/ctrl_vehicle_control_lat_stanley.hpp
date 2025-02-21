#ifndef CRP_APL_CTRL_VEHICLE_LAT_STANLEY_CTRLVEHICLECONTROLLATSTANLEY_HPP
#define CRP_APL_CTRL_VEHICLE_LAT_STANLEY_CTRLVEHICLECONTROLLATSTANLEY_HPP


#include "rclcpp/rclcpp.hpp"

#include "controllerInterfaces/controllerInterfaces.hpp"
#include "ctrl_vehicle_control_lat_stanley/controllerParams.hpp"

#include "controller_base/ctrlLatWrapperBase.hpp"

namespace crp
{
    namespace apl
    {
        class CtrlVehicleControlLatStanley : public CtrlLatWrapperBase
        {
            public:
                CtrlVehicleControlLatStanley();

            private:
                // VARIABLES
                crp::apl::ControlParams m_params;

                void error_calculation(const ControlInput & input, double &lateral_error, double &heading_error);
                void stanleyControl(const ControlInput & input, LatControlOutput & output);
                void controlLoop(const ControlInput & input, LatControlOutput & output) override;
        }; 
    } // namespace apl
}
#endif // CRP_APL_CTRL_VEHICLE_LAT_STANLEY_CTRLVEHICLECONTROLLATSTANLEY_HPP