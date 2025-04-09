#ifndef CRP_APL_CTRL_VEHICLE_LAT_COMPENSATORY_CTRLVEHILECONTROLLATCOMPENSATORY_HPP
#define CRP_APL_CTRL_VEHICLE_LAT_COMPENSATORY_CTRLVEHILECONTROLLATCOMPENSATORY_HPP


#include "rclcpp/rclcpp.hpp"
#include "lib/utils.hpp"

#include "ctrl_vehicle_control/controllerParams.hpp"
#include "compensatory_model/compensatory_model.hpp"

#include "controllerInterfaces/controllerInterfaces.hpp"
#include "controller_base/ctrlLatWrapperBase.hpp"

namespace crp
{
namespace apl
{
    class CtrlVehicleControlLatCompensatory : public CtrlLatWrapperBase
    {
        public:
            CtrlVehicleControlLatCompensatory();

        private:
            crp::apl::ControlParams      m_params;
            crp::apl::GeometricOperators m_geometricOperator;
            crp::apl::CompensatoryModel  m_compensatoryModel;

            void controlLoop(const ControlInput & input, LatControlOutput & output) override;
    };
} // namespace apl
} // namespace crp
#endif // CRP_APL_CTRL_VEHICLE_LAT_COMPENSATORY_CTRLVEHILECONTROLLATCOMPENSATORY_HPP