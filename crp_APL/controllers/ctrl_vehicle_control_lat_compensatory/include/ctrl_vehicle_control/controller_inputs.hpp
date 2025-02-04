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
            double targetSpeed{0.0f};
            double egoPoseGlobal [3]{0.0f};
            double vxEgo{0.0f};
            double egoSteeringAngle{0.0f};
        };

        struct ControlParams{
            double ffLookAheadTime{1.5f};
            double vehAxleDistance{2.9f};
            double maxAcceleration{3.0f};
            double steeringAngleLPFilter{0.7f};
            double fbLookAheadTime{0.0f};
            double fbPGain{0.5f};
            double fbDGain{0.1f};
            double fbIGain{0.01f};
            double fbIntegralLimit{3.0f};
            double dT{0.0333f};
            double vxMin{3.0f};
            double trajectory_distance{50.0f};
            double sigma_thetaFP{0.25f};
            double maxThetaFP{0.3f};
            double targetAccelerationFF_lpFilterCoeff{0.99f};
            double targetAccelerationFB_lpFilterCoeff{0.99f};
        };
    }
}

#endif