#include <stdio.h>
#include <string>
#include <vector>

#ifndef CONTROLLER_INPUTS_HPP_
#define CONTROLLER_INPUTS_HPP_

namespace crp
{
    namespace apl
    {
        struct controlInput{
            std::vector<double> path_x;
            std::vector<double> path_y;
            std::vector<double> path_theta;
            double target_speed{0.0f};
            double egoPoseGlobal [3]{0.0f};
            double vxEgo{0.0f};
            double egoSteeringAngle{0.0f};
        };

        struct controlParams{
            double ffGainOffsetGround{1.0f};
            double ffGainSlope{0.1f};
            double ffLookAheadTime{1.5f};
            double ffMinLookAheadDistance{0.1f};
            double vehAxleDistance{2.9f};
            double maxCurvature{0.1};
            double steeringAngleLPFilter{0.7f};
            double fbLookAheadTime{0.0f};
            double fbPGain{0.5f};
            double fbDGain{0.1f};
            double fbIGain{0.01f};
            double fbThetaGain{0.0f};
            double fbMinLookAheadDistance{0.0f};
            double fbIntegralLimit{3.0f};
            double invSteerTimeConstant{0.0f}; // todo
            double invSteerDamping{0.0f}; // todo
            double dT{0.0333f};
            double vxMin{3.0f};
            double trajectory_distance{50.0f};
            bool debugKPIs{true};
        };
    }
}

#endif