#ifndef CONTROLLER_INPUTS_HPP_
#define CONTROLLER_INPUTS_HPP_


#include <stdio.h>
#include <string>
#include <vector>


namespace crp
{
    namespace apl
    {
        struct ControlInput{
            std::vector<double> path_x;
            std::vector<double> path_y;
            std::vector<double> path_theta;
            double target_speed{0.0f};
            double egoPoseGlobal [3]{0.0f};
            double vxEgo{0.0f};
            double egoSteeringAngle{0.0f};
        };

        struct ControlParams{
            double k_gain{0.5f};
            double wheelbase{2.9f};
        };
    }
}

#endif