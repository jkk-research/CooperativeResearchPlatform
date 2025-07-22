#ifndef CRP_APL_SCENARIO_FUSION_POSECORRECTION_HPP
#define CRP_APL_SCENARIO_FUSION_POSECORRECTION_HPP

#include <cmath>

namespace crp
{
    namespace apl
    {
        class PoseCorrection{
            public:
                void run (const double orientation[4], const double position[2], const double velocity);
                double getEstimatedOrientation();
                double getMeasuredOrientation();

            private:
                double m_orientationInAngle{0.0f};
                double m_position_prev[2]{0.0f};
                double m_estimatedOrientationRaw{0.0f};
                double m_orientationError{0.0f};
                double m_estimatedOrientation{0.0f};
        };
    }
}

#endif // CRP_APL_SCENARIO_FUSION_POSECORRECTION_HPP