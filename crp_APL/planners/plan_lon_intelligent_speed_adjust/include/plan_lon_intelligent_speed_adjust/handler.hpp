#ifndef CRP_APL_PLAN_LON_INTELLIGENT_SPEED_ADJUST_HANDLER_PLANLATLANEFOLLOW_HPP
#define CRP_APL_PLAN_LON_INTELLIGENT_SPEED_ADJUST_HANDLER_PLANLATLANEFOLLOW_HPP

#include "planner_base/handlerBase.hpp"

namespace crp
{
namespace apl
{

class PlanLonIntelligentSpeedAdjustHandler : public HandlerBase
{
public:
    PlanLonIntelligentSpeedAdjustHandler();

private:
    void plan(const PlannerInput & input, PlannerOutput & output) override;
};

} // namespace apl
} // namespace crp
#endif // CRP_APL_PLAN_LON_INTELLIGENT_SPEED_ADJUST_HANDLER_PLANLATLANEFOLLOW_HPP
