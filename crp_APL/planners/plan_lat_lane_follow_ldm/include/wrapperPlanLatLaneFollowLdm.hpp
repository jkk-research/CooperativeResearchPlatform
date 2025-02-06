#ifndef CRP_APL_PLAN_LAT_LANE_FOLLOW_LDM_HANDLER_PLANLATLANEFOLLOWLDM_HPP
#define CRP_APL_PLAN_LAT_LANE_FOLLOW_LDM_HANDLER_PLANLATLANEFOLLOWLDM_HPP

#include "planner_base/wrapperBase.hpp"

// functional code
#include "../src/functionCode/inc/linearDriverModel/linearDriverModel.hpp"
#include "../src/functionCode/inc/linearDriverModel/linearDriverModel_interfaces.hpp"
#include "../src/functionCode/inc/linearDriverModelUtilities/linearDriverModel_coordinateTransforms.hpp"

// utils
#include "geometryUtils/inc/polynomialRegression.hpp"

namespace crp
{
namespace apl
{

class PlanLatLaneFollowLdm : public WrapperBase
{
public:
    PlanLatLaneFollowLdm();

private:
    void plan(const PlannerInput & input, PlannerOutput & output) override;

    //function member
    LinearDriverModel       m_linearDriverModel;
    // planner interfaces
    LDMParamIn              m_ldmParams;
    Pose2D                  m_egoPose;
    ScenarioPolynomials     m_scenarioPolynomials;

    // support function to convert standardized planner input to subordinate types
    void generateEgoPose(const PlannerInput & input);
    void calculateNodePoints(const PlannerInput & input);

    PolynomialSubfunctions m_polynomialRegressor;
};

} // namespace apl
} // namespace crp
#endif // CRP_APL_PLAN_LAT_LANE_FOLLOW_LDM_HANDLER_PLANLATLANEFOLLOWLDM_HPP
