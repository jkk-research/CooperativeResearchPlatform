#ifndef CRP_APL_PLAN_LAT_LANE_FOLLOW_HANDLER_PLANLATLANEFOLLOW_HPP
#define CRP_APL_PLAN_LAT_LANE_FOLLOW_HANDLER_PLANLATLANEFOLLOW_HPP

#include "planner_base/wrapperBase.hpp"

namespace crp
{
namespace apl
{

class PlanLatLaneFollowHandler : public WrapperBase
{
public:
    PlanLatLaneFollowHandler();

private:
    void plan(const PlannerInput & input, PlannerOutput & output) override;
};

} // namespace apl
} // namespace crp
#endif // CRP_APL_PLAN_LAT_LANE_FOLLOW_HANDLER_PLANLATLANEFOLLOW_HPP
