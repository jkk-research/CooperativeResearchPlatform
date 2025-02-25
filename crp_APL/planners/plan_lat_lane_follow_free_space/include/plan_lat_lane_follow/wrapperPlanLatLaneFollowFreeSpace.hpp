#ifndef CRP_APL_WRAPPER_PLAN_LAT_LANE_FOLLOW_FREESPACE_HPP
#define CRP_APL_WRAPPER_PLAN_LAT_LANE_FOLLOW_FREESPACE_HPP

#include "planner_base/wrapperBase.hpp"
#include "../../src/functionCode/inc/planFreeSpacePath.hpp"

namespace crp
{
namespace apl
{

class WrapperPlanLatLaneFollowFreeSpace : public WrapperBase
{
public:
    WrapperPlanLatLaneFollowFreeSpace();

private:
    void plan(const PlannerInput & input, PlannerOutput & output) override;

    PlanFreeSpacePath m_planFreeSpacePath;
};

} // namespace apl
} // namespace crp
#endif // CRP_APL_WRAPPER_PLAN_LAT_LANE_FOLLOW_FREESPACE_HPP
