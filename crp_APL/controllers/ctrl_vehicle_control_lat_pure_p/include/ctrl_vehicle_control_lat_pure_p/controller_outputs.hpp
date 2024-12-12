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
            double m_accelerationTarget{0.0f};
            double m_steeringAngleTarget{0.0f};
        };
    }
}

#endif