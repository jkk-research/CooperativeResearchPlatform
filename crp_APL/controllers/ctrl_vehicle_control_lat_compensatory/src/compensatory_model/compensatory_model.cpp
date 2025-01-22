#include "compensatory_model/compensatory_model.hpp"

namespace crp
{
    namespace apl
    {
        void CompensatoryModel::run(ControlInput &input, ControlOutput &output, const ControlParams &params)
        {
            // this method is the main method of the compensatory model. It calculates the feedforward and feedback

            double point[3];
            m_localPath_x.clear();
            m_localPath_y.clear();
            for (unsigned long int i = 0; i < input.path_x.size(); i++)
            {
                point[0] = input.path_x.at(i);
                point[1] = input.path_y.at(i);
                point[2] = 0.0f;
                double *localPathPoint = point;

                m_localPath_x.push_back(localPathPoint[0]);
                m_localPath_y.push_back(localPathPoint[1]);
                printf("local points x=%f y=%f\n", point[0], point[1]);
            }

            // cut snippet
            cutRelevantLocalSnippet();

            if (input.path_x.size() >= 4)
            {

                printf("Trajectory length is %d\n", input.path_x.size());
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
                calculateSteeringAngle(input, params);

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
            m_k_superPosition = 0.5;
        }

        void CompensatoryModel::calculateFeedforward(const ControlInput &input, const ControlParams &params)
        {
            // this method calculates the feedforward target steering angle.
            // First, a look ahead point is calculated. This is based on the preview time defined
            // as a parameter. Then, this preview time is used to calculate the feedforward
            // acceleration.

            calculateLookAheadPose(input, params);

            double targetAccelerationUnfiltered = 2*m_lookAheadPose[1] / (std::pow(params.fbLookAheadTime,2));

            // strong filtering
            m_targetAccelerationFF = 0.99f * m_targetAccelerationFF +
                                0.01f * targetAccelerationUnfiltered;

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
            m_lookAheadDistanceFb = std::max(params.fbMinLookAheadDistance, params.fbLookAheadTime * input.vxEgo);
            // from coefficients
            double thetaFP = atan(m_coefficients[1] + 
                2.0f*m_coefficients[2]*m_lookAheadDistanceFb + 
                3.0f*m_coefficients[3]*std::pow(m_lookAheadDistanceFb,2));

            double thetaFPNormalized = thetaFP / params.maxThetaFP;
            thetaFPNormalized = std::min(1.0, std::max(-1.0, thetaFPNormalized));

            // now, calculate the gain schedule based on theta FP
            double k = 1-exp(-std::pow(thetaFPNormalized,2)/(2.0f*std::pow(params.sigma_thetaFP,2)));


            m_posErr = k*(m_coefficients[0]+m_coefficients[1]*m_lookAheadDistanceFb+
                        m_coefficients[2]*std::pow(m_lookAheadDistanceFb,2) +
                        m_coefficients[3]*std::pow(m_lookAheadDistanceFb,3));

            m_posIntegralError = m_posIntegralError + m_posErr * params.dT;
            m_posIntegralError = std::min(std::max(-params.fbIntegralLimit, m_posIntegralError),
                                          params.fbIntegralLimit);

            m_posDerivativeError = (m_posErr - m_posErrPrev) / params.dT;
            m_posDerivativeError = m_posDerivativeFilter.lowPassFilter(m_posDerivativeError, m_posDerivativeError_prev, 0.999f);

            double targetFbAccelerationUnfiltered = params.fbPGain * m_posErr +
                                     params.fbDGain * m_posDerivativeError; +
                                     params.fbIGain * m_posIntegralError;

            
            // strong filtering
            m_targetAccelerationFB = 0.99f * m_targetAccelerationFB +
                                0.01f * targetFbAccelerationUnfiltered;

            m_targetAccelerationFB = std::min(std::max(-params.maxAcceleration, m_targetAccelerationFB), params.maxAcceleration);

            m_posErrPrev = m_posErr;
        }

        void CompensatoryModel::cutRelevantLocalSnippet()
        {
            // this method cuts a relevant length of the total trajectory, based on the complete
            // path transformed to the ego coordinate frame
            // step 1: searching for nearest point in trajectory
            unsigned long int startIdx = -1;
            unsigned long int stopIdx = -1;
            m_trajInvalid = false;
            double maxDistance = 50; // meters
            m_localPathCut_x.clear();
            m_localPathCut_y.clear();

            for (unsigned long int i = 0; i < m_localPath_x.size(); i++)
            {
                if (m_localPath_x.at(i) > 0)
                {
                    if (i == 0)
                        startIdx = i;
                    else
                        startIdx = i - 1;

                    break;
                }
            }
            if (startIdx == -1)
            {
                // no valid point was found
                m_trajInvalid = true;
            }
            else
            {
                // find stop index
                for (unsigned long int i = startIdx; i < m_localPath_x.size(); i++)
                {
                    double pointDistance = std::sqrt(std::pow(m_localPath_x.at(i), 2) + std::pow(m_localPath_y.at(i), 2));
                    if (pointDistance > maxDistance)
                    {
                        stopIdx = i;
                        break;
                    }
                }
            }
            if (stopIdx == -1)
            {
                m_trajInvalid = true;
            }
            else
            {
                // valid snippet is found, calculate real path
                for (unsigned long int i = startIdx; i < stopIdx; i++)
                {
                    m_localPathCut_x.push_back(m_localPath_x.at(i));
                    m_localPathCut_y.push_back(m_localPath_y.at(i));
                    if (i < startIdx + 10)
                    {
                        printf("path after cut %f %f\n", m_localPath_x.at(i), m_localPath_y.at(i));
                    }
                }
            }
        }

        void CompensatoryModel::calculateLookAheadPose(const ControlInput &input, const ControlParams &params)
        {
            // calculate the look ahead pose based on a predicted position of the vehicle.
            // look ahead time is taken from parameters.

            // feedback look ahead pose
            float lookAheadDistance = params.fbLookAheadTime * input.vxEgo;
            float lookAheadOrientationChange =
                params.fbLookAheadTime * tan(input.egoSteeringAngle) / params.vehAxleDistance * input.vxEgo;

            printf("predicted orientation change is %f\n", lookAheadOrientationChange * 180 / 3.14159);

            // in the ego frame x-y the displacements are
            float predictedVehiclePoset1[3];
            predictedVehiclePoset1[0] = lookAheadDistance * cos(lookAheadOrientationChange);
            predictedVehiclePoset1[1] = lookAheadDistance * sin(lookAheadOrientationChange);
            predictedVehiclePoset1[2] = lookAheadOrientationChange;

            // calculate the look ahead point in the t0 ego frame
            float lookAheadPointt0[3];
            lookAheadPointt0[0] = predictedVehiclePoset1[0];
            lookAheadPointt0[1] =
                m_coefficients[0] +
                m_coefficients[1] * predictedVehiclePoset1[0] +
                m_coefficients[2] * std::pow(predictedVehiclePoset1[0], 2) +
                m_coefficients[3] * std::pow(predictedVehiclePoset1[0], 3);
            lookAheadPointt0[2] =
                atan(m_coefficients[1] +
                     2.0f * m_coefficients[2] * predictedVehiclePoset1[0] +
                     3.0f * m_coefficients[3] * std::pow(predictedVehiclePoset1[0], 2));

            // now making the correction for the predicted new pose and calculate the prospective error
            // (this is a global to local like transformation)
            // translation first, then rotation
            m_lookAheadPose[0] = (lookAheadPointt0[0] - predictedVehiclePoset1[0]) * cos(predictedVehiclePoset1[2]) +
                                 (lookAheadPointt0[1] - predictedVehiclePoset1[1]) * sin(predictedVehiclePoset1[2]);

            m_lookAheadPose[1] = -1.0f * (lookAheadPointt0[0] - predictedVehiclePoset1[0]) * sin(predictedVehiclePoset1[2]) +
                                 (lookAheadPointt0[1] - predictedVehiclePoset1[1]) * cos(predictedVehiclePoset1[2]);

            // finally, angle adjustment
            m_lookAheadPose[2] = lookAheadPointt0[2] - predictedVehiclePoset1[2];

            printf("look ahead pose is %f ,%f, %f\n", m_lookAheadPose[0], m_lookAheadPose[1], m_lookAheadPose[2]);
        }

        void CompensatoryModel::calculateSteeringAngle(const ControlInput &input, const ControlParams &params)
        {
            // function to map acceleration target to steering angle. Right now, no proper low level 
            // acceleration function we have, only an Ackermann mapping 
            double vxLim = std::max(params.vxMin, input.vxEgo);

            // calculate steering angle from the acceleration based on target curvature
            double targetCurvature = m_targetAcceleration / std::pow(vxLim, 2);
            m_targetSteeringAngle = std::atan(targetCurvature * params.vehAxleDistance);

        }
    }
}