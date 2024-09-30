#ifndef CONTROLLER_OUTPUTS_HPP_
#define CONTROLLER_OUTPUTS_HPP_

#include <stdio.h>
#include <string>
#include <vector>


namespace crp
{
    namespace apl
    {
        struct ControlOutput{
            double accelerationTarget{0.0f};
            double steeringAngleTarget{0.0f};
        };
    }
}

#endif