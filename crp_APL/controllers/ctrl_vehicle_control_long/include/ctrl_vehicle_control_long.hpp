#ifndef CRP_APL_CTRL_VEHICLE_CONTROL_LONG_CTRLVEHICLECONTROLLONG_HPP
#define CRP_APL_CTRL_VEHICLE_CONTROL_LONG_CTRLVEHICLECONTROLLONG_HPP


#include "rclcpp/rclcpp.hpp"

#include "controller_base/ctrlLongWrapperBase.hpp"


namespace crp
{
    namespace apl
    {

        class CtrlVehicleControlLong : public CtrlLongWrapperBase
        {
            public:
                CtrlVehicleControlLong();

            private:
                void controlLoop(const ControlInput & input, LongControlOutput & output) override;
        };

    } // namespace apl
} // namespace crp
#endif // CRP_APL_CTRL_VEHICLE_CONTROL_LONG_CTRLVEHICLECONTROLLONG_HPP
