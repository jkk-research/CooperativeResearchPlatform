#ifndef CRP_APL_PLAN_LON_EMERGENCY_HANDLER_PLANLONEMERGENCY_HPP
#define CRP_APL_PLAN_LON_EMERGENCY_HANDLER_PLANLONEMERGENCY_HPP

#include "planner_base/handlerBase.hpp"

namespace crp
{
namespace apl
{

class PlanLonEmergencyHandler : public HandlerBase
{
public:
    PlanLonEmergencyHandler();

private:
    void plan(const PlannerInput & input, PlannerOutput & output) override;
};

} // namespace apl
} // namespace crp
#endif // CRP_APL_PLAN_LON_EMERGENCY_HANDLER_PLANLONEMERGENCY_HPP
