#include "compensatory_model/compensatory_model.hpp"

namespace crp
{
    namespace apl
    {
        void CompensatoryModel::run(const ControlInput &input, LatControlOutput &output, const ControlParams &params)
        {
            // this method is the main method of the compensatory model. It calculates the feedforward and feedback

            double point[3];
            m_localpathX.clear();
            m_localpathY.clear();
            for (unsigned long int i = 0; i < input.pathX.size(); i++)
            {
                point[0] = input.pathX.at(i);
                point[1] = input.pathY.at(i);
                point[2] = 0.0f;
                double *localPathPoint = point;

                m_localpathX.push_back(localPathPoint[0]);
                m_localpathY.push_back(localPathPoint[1]);
                printf("local points x=%f y=%f\n", point[0], point[1]);
            }

            // cut snippet
            cutRelevantLocalSnippet(params);

            if (input.pathX.size() >= 4)
            {

                printf("Trajectory length is %ld\n", input.pathX.size());
                m_coefficients = m_polynomialCalculator.calculateThirdOrderPolynomial(m_localPathCut_x, m_localPathCut_y);

                // feedforward based on preview point - output: target feedforward acceleration
                calculateFeedforward(input, params);

                // feedback control
                calculateFeedback(input, params);

                // superposition of the feedforward and feedback lateral acceleration
                superPoseFeedforwardAndFeedback();

                // summing the target values to produce the target
                output.accelerationTarget = (1.0f-m_k_superPosition)*m_targetAccelerationFF + 
                    m_k_superPosition*m_targetAccelerationFB;

                // output calculation
                calculateSteeringAngle(input, output, params);

                printf("target steering output %f\n", output.steeringAngleTarget);

                // finally, filter the output steering angle, too
                output.steeringAngleTarget = m_steeringAngleFilter.lowPassFilter(output.steeringAngleTarget, m_targetSteeringAngle_prev, params.steeringAngleLPFilter);

                m_targetSteeringAngle_prev = output.steeringAngleTarget;
            }
        }

        void CompensatoryModel::superPoseFeedforwardAndFeedback()
        {
            // the superposition is the calculation of the superposition factor between the 
            // feedforward and the feedback targets. The factor lies between 0 and 1.
            // k = 0: only feedforward,
            // k = 1: only feedback
            if (std::abs(m_coefficients[2]) < 1.25e-4)
            {
                m_k_superPosition = 1.0;
            }
            else{
                m_k_superPosition = 0.5;
            }
        }

        void CompensatoryModel::calculateFeedforward(const ControlInput &input, const ControlParams &params)
        {
            // this method calculates the feedforward target steering angle.
            // First, a look ahead point is calculated. This is based on the preview time defined
            // as a parameter. Then, this preview time is used to calculate the feedforward
            // acceleration.

            calculateLookAheadPose(input, params);

            double targetAccelerationUnfiltered = 2.0f*m_lookAheadPose[1] / (std::pow(params.ffLookAheadTime,2));

            // strong filtering
            m_targetAccelerationFF = params.targetAccelerationFF_lpFilterCoeff * m_targetAccelerationFF +
                                (1.0f-params.targetAccelerationFF_lpFilterCoeff) * targetAccelerationUnfiltered;

            m_targetAccelerationFF = std::min(std::max(-params.maxAcceleration, m_targetAccelerationFF), params.maxAcceleration);

        }

        void CompensatoryModel::calculateFeedback(const ControlInput &input, const ControlParams &params)
        {
            // This method calculates the feedback based acceleration target.
            // Elements:
            // 1. An adaptive proportional gain based on the focus point angle
            // 2. Highly filtered derivative gain,
            // 3. Integral

            // calculate focus point angle
            m_lookAheadDistanceFb = params.fbLookAheadTime * input.vxEgo;

            m_posErr = (m_coefficients[0]+m_coefficients[1]*m_lookAheadDistanceFb+
                        m_coefficients[2]*std::pow(m_lookAheadDistanceFb,2) +
                        m_coefficients[3]*std::pow(m_lookAheadDistanceFb,3));

            double errorNormalized = m_posErr / params.maxThetaFP;
            errorNormalized = std::min(1.0, std::max(-1.0, errorNormalized));

            // now, calculate the gain schedule based on theta FP
            double k = 1-exp(-std::pow(errorNormalized,2)/(2.0f*std::pow(params.sigma_thetaFP,2)));
            k = std::min(std::max(k, -1.0), 1.0);

            m_posIntegralError = m_posIntegralError + m_posErr * params.dT;
            m_posIntegralError = std::min(std::max(-params.fbIntegralLimit, m_posIntegralError),
                                          params.fbIntegralLimit);     

            m_posDerivativeError = (m_posErr - m_posErrPrev) / params.dT;
            m_posDerivativeError = m_posDerivativeFilter.lowPassFilter(m_posDerivativeError, m_posDerivativeError_prev, 0.99f);

            m_posErr = k*m_posErr;  

            double targetFbAccelerationUnfiltered = params.fbPGain * m_posErr +
                                    params.fbDGain * m_posDerivativeError +
                                    params.fbIGain * m_posIntegralError;

            
            // strong filtering
            m_targetAccelerationFB = params.targetAccelerationFB_lpFilterCoeff * m_targetAccelerationFB +
                                (1.0f - params.targetAccelerationFB_lpFilterCoeff) * targetFbAccelerationUnfiltered;

            m_targetAccelerationFB = std::min(std::max(-params.maxAcceleration, m_targetAccelerationFB), params.maxAcceleration);

            m_posErrPrev = m_posErr;
        }

        void CompensatoryModel::cutRelevantLocalSnippet(const ControlParams &params)
        {
            // this method cuts a relevant length of the total trajectory, based on the complete
            // path transformed to the ego coordinate frame
            // step 1: searching for nearest point in trajectory
            unsigned long int startIdx = std::numeric_limits<unsigned long int>::max();
            unsigned long int stopIdx  = std::numeric_limits<unsigned long int>::max();
            m_trajInvalid = false;
            double maxDistance = params.trajectory_distance; // meters
            m_localPathCut_x.clear();
            m_localPathCut_y.clear();

            for (unsigned long int i = 0; i < m_localpathX.size(); i++)
            {
                if (m_localpathX.at(i) > 0)
                {
                    if (i == 0)
                        startIdx = i;
                    else
                        startIdx = i - 1;

                    break;
                }
            }
            if (startIdx == std::numeric_limits<unsigned long int>::max())
            {
                // no valid point was found
                m_trajInvalid = true;
            }
            else
            {
                // find stop index
                for (unsigned long int i = startIdx; i < m_localpathX.size(); i++)
                {
                    double pointDistance = std::sqrt(std::pow(m_localpathX.at(i), 2) + std::pow(m_localpathY.at(i), 2));
                    if (pointDistance > maxDistance)
                    {
                        stopIdx = i;
                        break;
                    }
                }
            }
            if (stopIdx == std::numeric_limits<unsigned long int>::max())
            {
                m_trajInvalid = true;
            }
            else
            {
                // valid snippet is found, calculate real path
                for (unsigned long int i = startIdx; i < stopIdx; i++)
                {
                    m_localPathCut_x.push_back(m_localpathX.at(i));
                    m_localPathCut_y.push_back(m_localpathY.at(i));
                    if (i < startIdx + 10)
                    {
                        printf("path after cut %f %f\n", m_localpathX.at(i), m_localpathY.at(i));
                    }
                }
            }
        }

        void CompensatoryModel::calculateLookAheadPose(const ControlInput &input, const ControlParams &params)
        {
            // calculate the look ahead pose based on a predicted position of the vehicle.
            // look ahead time is taken from parameters.

            // feedback look ahead pose
            m_lookAheadPose[0] = params.ffLookAheadTime * input.vxEgo;

            m_lookAheadPose[1] =
                m_coefficients[0] +
                m_coefficients[1] * m_lookAheadPose[0] +
                m_coefficients[2] * std::pow(m_lookAheadPose[0], 2) +
                m_coefficients[3] * std::pow(m_lookAheadPose[0], 3);
            m_lookAheadPose[2] =
                atan(m_coefficients[1] +
                     2.0f * m_coefficients[2] * m_lookAheadPose[0] +
                     3.0f * m_coefficients[3] * std::pow(m_lookAheadPose[0], 2));

            printf("look ahead pose is %f ,%f, %f\n", m_lookAheadPose[0], m_lookAheadPose[1], m_lookAheadPose[2]);
        }

        void CompensatoryModel::calculateSteeringAngle(const ControlInput &input, LatControlOutput& output, const ControlParams &params)
        {
            // function to map acceleration target to steering angle. Right now, no proper low level 
            // acceleration function we have, only an Ackermann mapping 
            double vxLim = std::max(params.vxMin, input.vxEgo);

            // calculate steering angle from the acceleration based on target curvature
            double targetCurvature = output.accelerationTarget / std::pow(vxLim, 2);
            output.steeringAngleTarget = std::atan(targetCurvature * params.vehAxleDistance);
        }
    }
}