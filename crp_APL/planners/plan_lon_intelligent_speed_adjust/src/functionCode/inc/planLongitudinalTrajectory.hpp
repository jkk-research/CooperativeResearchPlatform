#ifndef CRP_APL_PLANLONGITUDINALTRAJECTORY_HPP
#define CRP_APL_PLANLONGITUDINALTRAJECTORY_HPP

#include <cmath>
#include <stdio.h>

#include "plannerInterfaces/plannerInterfaces.hpp"

namespace crp
{
    namespace apl
    {
        class PlanLongitudinalTrajectory{
            public:
                void run (const PlannerInput& input, PlannerOutput& output);
            
            private:
                
        };
    }
}

#endif