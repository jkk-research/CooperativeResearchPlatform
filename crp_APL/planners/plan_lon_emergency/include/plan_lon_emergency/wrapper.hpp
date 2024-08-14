#ifndef CRP_APL_PLAN_LON_EMERGENCY_WRAPPER_PLANLONEMERGENCY_HPP
#define CRP_APL_PLAN_LON_EMERGENCY_WRAPPER_PLANLONEMERGENCY_HPP

#include "planner_base/wrapperBase.hpp"

namespace crp
{
namespace apl
{

class PlanLonEmergencyWrapper : public WrapperBase
{
public:
    PlanLonEmergencyWrapper();

private:
    void plan(const PlannerInput & input, PlannerOutput & output) override;
};

} // namespace apl
} // namespace crp
#endif // CRP_APL_PLAN_LON_EMERGENCY_WRAPPER_PLANLONEMERGENCY_HPP
