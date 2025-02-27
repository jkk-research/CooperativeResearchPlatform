#ifndef CRP_APL_WRAPPER_PLAN_LON_INTELLIGENT_SPEED_ADJUST_PLANLONINTELLIGENTSPEEDADJUST_HPP
#define CRP_APL_WRAPPER_PLAN_LON_INTELLIGENT_SPEED_ADJUST_PLANLONINTELLIGENTSPEEDADJUST_HPP

#include "planner_base/planWrapperBase.hpp"
#include "../../src/functionCode/inc/planLongitudinalTrajectory.hpp"

namespace crp
{
namespace apl
{

class WrapperPlanLonIntelligentSpeedAdjust : public PlanWrapperBase
{
public:
    WrapperPlanLonIntelligentSpeedAdjust();

private:
    void planLoop(const PlannerInput & input, PlannerOutput & output) override;

    // function main class
    PlanLongitudinalTrajectory m_planLongitudinalTrajectory;
};

} // namespace apl
} // namespace crp
#endif // CRP_APL_WRAPPER_PLAN_LON_INTELLIGENT_SPEED_ADJUST_PLANLONINTELLIGENTSPEEDADJUST_HPP
