#include <stdio.h>
#include <string>
#include <vector>

#ifndef CONTROLLER_OUTPUTS_HPP_
#define CONTROLLER_OUTPUTS_HPP_

namespace crp
{
    namespace apl
    {
        struct controlOutput{
            double accelerationTarget{0.0f};
            double steeringAngleTarget{0.0f};
        };
    }
}

#endif