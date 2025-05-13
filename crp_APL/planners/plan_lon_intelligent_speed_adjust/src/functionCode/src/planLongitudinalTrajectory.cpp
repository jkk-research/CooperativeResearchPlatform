#include "../inc/planLongitudinalTrajectory.hpp"

namespace crp
{
    namespace apl
    {
        void PlanLongitudinalTrajectory::run (const PlannerInput& input, PlannerOutput& output)
        {
            // This function is responsible to read the speed in the input trajectory and 
            // calculate an appropriate speed profile for the planning horizon.
            // There are two different speeds: 
            // 1. A generic maximum speed in the traffic rules (m_input.maximumSpeed) and
            // 2. one in the target space local path, given for each point separately (m_input.path.targetSpeed).
            // 
            // The minimum of the two values are taken in the first place
            
            // loop through all the path points from the input and write the speed data to the output
            output.trajectory.clear();

            curveSpeedControl();
            
            for (unsigned long int i=0; i<input.path.pathPoints.size(); i++)
            {
                Pose3D pathPointPose = input.path.pathPoints.at(i).pose;

                TrajectoryPoint outputTrajectoryPoint;

                // assign the same point as the input
                outputTrajectoryPoint.pose = pathPointPose;

                // assign the minimum speed to the output
                outputTrajectoryPoint.velocity = std::min(input.maximumSpeed, m_vxCurveSpeed);

                output.trajectory.push_back(outputTrajectoryPoint);
            }
        }

        void PlanLongitudinalTrajectory::curveSpeedControl (const PlannerInput& input)
        {
            double lateralAccelerationMax = 0.0f;
            // this method calculates a maximum speed when driving in a curve, considering the lateral maximum acceleration
            if (input.behaviorInputs.curveSpeedMode == 1U)
            {
                lateralAccelerationMax = 1.5f;
            }
            else if (input.behaviorInputs.curveSpeedMode == 2U)
            {
                lateralAccelerationMax = 2.5f;
            }
            else if (input.behaviorInputs.curveSpeedMode == 2U)
            {
                lateralAccelerationMax = 3.5f;
            }
            else{
                lateralAccelerationMax = 1.5f;
            }

            // predict curve radius
            if (std::abs(input.egoKinematics.yawRate) > 0.05f)
            {
                double R = std::abs(input.egoKinematics.vX / input.egoKinematics.yawRate);
                m_vxCurveSpeed = std::sqrt(lateralAccelerationMax*R);
            }
            else{
                // no limitation
                m_vxCurveSpeed = 100.0f;
            }
        }
    }
}