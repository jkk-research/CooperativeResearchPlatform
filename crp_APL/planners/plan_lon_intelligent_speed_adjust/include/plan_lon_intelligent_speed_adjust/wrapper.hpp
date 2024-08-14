#ifndef CRP_APL_PLAN_LON_INTELLIGENT_SPEED_ADJUST_WRAPPER_PLANLATLANEFOLLOW_HPP
#define CRP_APL_PLAN_LON_INTELLIGENT_SPEED_ADJUST_WRAPPER_PLANLATLANEFOLLOW_HPP

#include "planner_base/wrapperBase.hpp"

namespace crp
{
namespace apl
{

class PlanLonIntelligentSpeedAdjustWrapper : public WrapperBase
{
public:
    PlanLonIntelligentSpeedAdjustWrapper();

private:
    void plan(const PlannerInput & input, PlannerOutput & output) override;
};

} // namespace apl
} // namespace crp
#endif // CRP_APL_PLAN_LON_INTELLIGENT_SPEED_ADJUST_WRAPPER_PLANLATLANEFOLLOW_HPP
