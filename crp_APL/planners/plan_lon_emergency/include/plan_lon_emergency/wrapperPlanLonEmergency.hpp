#ifndef CRP_APL_WRAPPER_PLAN_LON_EMERGENCY_PLANLONEMERGENCY_HPP
#define CRP_APL_WRAPPER_PLAN_LON_EMERGENCY_PLANLONEMERGENCY_HPP

#include "planner_base/wrapperBase.hpp"

namespace crp
{
namespace apl
{

class WrapperPlanLonEmergency : public WrapperBase
{
public:
    WrapperPlanLonEmergency();

private:
    void plan(const PlannerInput & input, PlannerOutput & output) override;
};

} // namespace apl
} // namespace crp
#endif // CRP_APL_WRAPPER_PLAN_LON_EMERGENCY_PLANLONEMERGENCY_HPP
