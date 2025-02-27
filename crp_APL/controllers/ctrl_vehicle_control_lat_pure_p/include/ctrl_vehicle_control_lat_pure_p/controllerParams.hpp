#ifndef CONTROLLER_INPUTS_HPP_
#define CONTROLLER_INPUTS_HPP_


#include <stdio.h>
#include <string>
#include <vector>


namespace crp
{
    namespace apl
    {
        struct ControlParams{
            double lookaheadTime{1.0f};
            double wheelbase{2.9f};
        };
    }
}

#endif