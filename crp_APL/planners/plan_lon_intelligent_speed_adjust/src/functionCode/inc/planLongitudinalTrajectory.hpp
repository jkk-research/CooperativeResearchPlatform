#ifndef CRP_APL_PLANLONGITUDINALTRAJECTORY_HPP
#define CRP_APL_PLANLONGITUDINALTRAJECTORY_HPP

#include <cmath>
#include <stdio.h>

#include "plannerInterfaces/plannerInterfaces.hpp"
#include <pdp_if/msg/pdp_personalized_params_active.hpp>

namespace crp
{
    namespace apl
    {
        class PlanLongitudinalTrajectory{
            public:
                void run (const PlannerInput& input, PlannerOutput& output);
            
            private:
                void curveSpeedControl(const PlannerInput& input);

                float m_vxCurveSpeed{0.0};

                float m_R_filtered = 0.0f;
                float m_R_filtered_prev = 0.0f;
                
        };
    }
}

#endif