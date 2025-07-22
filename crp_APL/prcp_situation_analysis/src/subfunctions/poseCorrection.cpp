#include "../../include/prcp_situation_analysis/subfunctions/poseCorrection.hpp"

namespace crp
{
    namespace apl
    {
        void PoseCorrection::run(const double orientation[4], const double position[2], const double velocity)
        {
            // estimated angle based on position change
            // if velocity is high enough, start estimation of orientation based on position change
            double dx = position[0]-m_position_prev[0];
            // vehicle is almost south-north oriented, therefore estimation is not stable
            double dy = position[1]-m_position_prev[1];
            if (velocity > 5.0f && std::abs(dx)>0.0f && std::abs(dy)>0.0f)
            {
                m_estimatedOrientationRaw = std::atan2(dy, dx);
            }
            double siny_cosp = 2 * (orientation[3] * orientation[2] + orientation[0] * orientation[1]);
            double cosy_cosp = 1 - 2 * (orientation[1] * orientation[1] + orientation[2] * orientation[2]);
            m_orientationInAngle = std::atan2(siny_cosp, cosy_cosp);

            // error calculation and filter
            m_orientationError = 0.01*(m_estimatedOrientationRaw + m_orientationInAngle) + 0.99*m_orientationError;

            // error compensation
            m_estimatedOrientation = m_orientationInAngle+m_orientationError;

            // save previous values
            m_position_prev[0] = position[0];
            m_position_prev[1] = position[1];
        }

        double PoseCorrection::getEstimatedOrientation()
        {
            return m_estimatedOrientation;
        }

        double PoseCorrection::getMeasuredOrientation()
        {
            return m_orientationInAngle;
        }
    }
}