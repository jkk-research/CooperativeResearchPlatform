#include "compensatory_model/compensatory_model.hpp"

namespace crp
{
    namespace apl
    {
        void CompensatoryModel::run (ControlInput& input, ControlOutput& output, const ControlParams& params)
        {
            // this method is the main method of the compensatory model. It calculates the feedforward and feedback

            double point[3];
            m_localPath_x.clear(); m_localPath_y.clear();
            for (unsigned long int i=0; i<input.path_x.size(); i++)
            {
                point[0] = input.path_x.at(i);
                point[1] = input.path_y.at(i);
                point[2] = 0.0f;
                double* localPathPoint = point;
                
                m_localPath_x.push_back(localPathPoint[0]);
                m_localPath_y.push_back(localPathPoint[1]);
                printf("local points x=%f y=%f\n", point[0], point[1]);
            }

            // cut snippet
            cutRelevantLocalSnippet();

            if (input.path_x.size() < 4)
            {
            
                printf("Trajectory is invalid\n");
                printf("The following KPIs have been calculated\n");
                printf("\t c0 RMS = %f\n\tc0 mean = %f\n\t cycle numbers %l\n", m_KPI_c0RMS, m_KPI_c0Mean, N);
            }
            else
            {
                
                printf("Trajectory length is %d\n", input.path_x.size());
                m_coefficients = m_polynomialCalculator.calculateThirdOrderPolynomial(m_localPathCut_x, m_localPathCut_y);
                
                calculateLookAheadPose(input, params);
                
                // feedforward control
                calculateFeedforward(input, params);

                // feedback control
                calculateFeedback(input, params);

                // output calculation
                calculateSteeringAngle(input, params);

                // summing the target values to produce the target
                output.accelerationTarget = m_targetAccelerationFF + m_targetAccelerationFB;
                output.steeringAngleTarget = m_targetSteeringAngleFF + m_targetSteeringAngleFB;
                output.steeringAngleTarget = m_steeringAngleFilter.lowPassFilter(output.steeringAngleTarget, m_targetSteeringAngle_prev, params.steeringAngleLPFilter);

                // output.steeringAngleTarget = steeringInverseDynamics(output.steeringAngleTarget, params);

                // KPI calculation
                if (N<=65535)
                {
                    N++;
                }
                m_KPI_c0Mean = ((N-1)*m_KPI_c0Mean+m_coefficients[0])/N;
                m_KPI_c0MS = ((N-1)*m_KPI_c0MS+std::pow(m_coefficients[0],2))/N;
                m_KPI_c0RMS = std::sqrt(m_KPI_c0MS);
                m_KPI_c0Max = std::min(m_KPI_c0Max, m_coefficients[0]);
                
                m_targetSteeringAngle_prev = output.steeringAngleTarget;

                m_actualSteeringAngleLog[1] = m_actualSteeringAngleLog[0];
                m_actualSteeringAngleLog[0] = input.egoSteeringAngle;

                printf("The following KPIs have been calculated\n");
                printf("\t c0 RMS = %f\n\tc0 mean = %f\n\t c0 max = %f\n\t cycle numbers %d\n", m_KPI_c0RMS, m_KPI_c0Mean, m_KPI_c0Max, N);
            }
        }

        void CompensatoryModel::calculateFeedforward(const ControlInput& input, const ControlParams& params)
        {
            // this method calculates the feedforward target steering angle.
            // First, the curvature is calculated in the preview distance.
            // Secondly, the curvature is transformed to a steering angle, with a velocity dependent 
            // gain. This gain is planned to be used to imitate slip compensation.
            // Third, the resulting feedforward steering angle is filtered using a simple low pass 
            // filter.

            m_lookAheadDistance = std::max(params.ffMinLookAheadDistance, params.ffLookAheadTime*input.vxEgo);
            // second derivative of the 3rd order polynomial, filtered
            printf("relevant coefficients are %f, %f\n", m_coefficients[2], m_coefficients[3]);
            m_targetCurvature = 0.99f*m_targetCurvature + 
                                0.01f*(2.0f*m_coefficients[2]+6.0f*m_coefficients[3]*m_lookAheadDistance);

            if(params.debugKPIs)
            {printf("data before limitation: c0=%f, lad=%f, kappa=%f\n", m_coefficients[0], m_lookAheadDistance, m_targetCurvature);}

            // finally, lateral feedforward acceleration is calculated - this is done to be in harmony 
            // with the feedback path later (out of scope of this method)
            m_targetAccelerationFF = params.ffGainOffsetGround*std::min(std::max(-params.maxFFAcceleration, m_targetCurvature * std::pow(input.vxEgo,2)), params.maxFFAcceleration);

            if(params.debugKPIs)
                {printf("ayTarFF %f\n", m_targetAccelerationFF);}
        }

        void CompensatoryModel::calculateFeedback(const ControlInput& input, const ControlParams& params)
        {
            // this method calculates the feedback target steering angle.
            // First, control error is defined. A separate FB look ahead distance is calculated for this.
            // orientation and position error are calculated.
            // Secondly, the integral and derival parts are calculated.
            // Third, the orientation error is calculated.
            // Fourth, the errors are weighted and summed with the corresponding control coefficients.
            // Finally, lateral acceleration target is mapped to steering angle target.

            m_lookAheadDistanceFb = std::max(params.fbMinLookAheadDistance, params.fbLookAheadTime*input.vxEgo);

            m_posErr = m_lookAheadPose[1];

            m_orientationErr = m_lookAheadPose[2];

            m_posIntegralError = m_posIntegralError+m_posErr*params.dT;
            m_posIntegralError = std::min(std::max(-params.fbIntegralLimit, m_posIntegralError), 
                params.fbIntegralLimit);

            m_posDerivativeError = (m_posErr-m_posErrPrev)/params.dT;
            m_posDerivativeError = m_posDerivativeFilter.lowPassFilter(m_posDerivativeError, m_posDerivativeError_prev, 0.9f);

            m_targetAccelerationFB = params.fbPGain*m_posErr*std::abs(6*m_posErr) +
                params.fbDGain*m_posDerivativeError;

            m_targetAccelerationFB = m_targetAccelerationFB +
                params.fbIGain*m_posIntegralError;
            
            m_targetAccelerationFB = m_targetAccelerationFB + 
                params.fbThetaGain * m_orientationErr;

            m_targetAccelerationFB = std::min(std::max(-params.maxAcceleration, m_targetAccelerationFB), params.maxAcceleration);
            
            if(params.debugKPIs){printf("errors %f, %f, %f, %f\n", m_posErr, m_posIntegralError, m_posDerivativeError, m_orientationErr); printf("ayTarFB %f\n", m_targetAccelerationFB);}

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
            m_localPathCut_x.clear(); m_localPathCut_y.clear();

            for (unsigned long int i = 0; i<m_localPath_x.size(); i++)
            {
                if (m_localPath_x.at(i) > 0)
                {
                    if (i==0)
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
                for (unsigned long int i=startIdx; i<m_localPath_x.size(); i++)
                {
                    double pointDistance = std::sqrt(std::pow(m_localPath_x.at(i),2)+std::pow(m_localPath_y.at(i),2));
                    if (pointDistance>maxDistance)
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
            else{
                // valid snippet is found, calculate real path
                for (unsigned long int i=startIdx; i<stopIdx; i++)
                {
                    m_localPathCut_x.push_back(m_localPath_x.at(i));
                    m_localPathCut_y.push_back(m_localPath_y.at(i));
                    if (i<startIdx+10)
                    {
                        printf("path after cut %f %f\n", m_localPath_x.at(i), m_localPath_y.at(i));
                    }
                }
            }
   }

    void CompensatoryModel::calculateLookAheadPose(const ControlInput& input, const ControlParams& params)
    {
        // calculate the look ahead pose based on a predicted position of the vehicle.
        // look ahead time is taken from parameters.

        // feedback look ahead pose
        float lookAheadDistance = params.fbLookAheadTime*input.vxEgo;
        float lookAheadOrientationChange = 
            params.fbLookAheadTime*tan(input.egoSteeringAngle)/params.vehAxleDistance*input.vxEgo;

        printf("predicted orientation change is %f\n", lookAheadOrientationChange*180/3.14159);
        
        // in the ego frame x-y the displacements are
        float predictedVehiclePoset1[3];
        predictedVehiclePoset1[0] = lookAheadDistance*cos(lookAheadOrientationChange);
        predictedVehiclePoset1[1] = lookAheadDistance*sin(lookAheadOrientationChange);
        predictedVehiclePoset1[2] = lookAheadOrientationChange;

        // calculate the look ahead point in the t0 ego frame
        float lookAheadPointt0[3];
        lookAheadPointt0[0] = predictedVehiclePoset1[0];
        lookAheadPointt0[1] = 
                m_coefficients[0]+
                m_coefficients[1]*predictedVehiclePoset1[0] +
                m_coefficients[2]*std::pow(predictedVehiclePoset1[0],2)+
                m_coefficients[3]*std::pow(predictedVehiclePoset1[0],3);
        lookAheadPointt0[2] = 
                atan(m_coefficients[1] +
                2.0f*m_coefficients[2]*predictedVehiclePoset1[0]+
                3.0f*m_coefficients[3]*std::pow(predictedVehiclePoset1[0],2));

        // now making the correction for the predicted new pose and calculate the prospective error
        // (this is a global to local like transformation)
        // translation first, then rotation
        m_lookAheadPose[0] = (lookAheadPointt0[0] - predictedVehiclePoset1[0])*cos(predictedVehiclePoset1[2]) +
                                (lookAheadPointt0[1] - predictedVehiclePoset1[1])*sin(predictedVehiclePoset1[2]);
        
        m_lookAheadPose[1] = -1.0f*(lookAheadPointt0[0] - predictedVehiclePoset1[0])*sin(predictedVehiclePoset1[2]) +
                                (lookAheadPointt0[1] - predictedVehiclePoset1[1])*cos(predictedVehiclePoset1[2]);

        // finally, angle adjustment
        m_lookAheadPose[2] = lookAheadPointt0[2] - predictedVehiclePoset1[2];

        printf("look ahead pose is %f ,%f, %f\n", m_lookAheadPose[0], m_lookAheadPose[1], m_lookAheadPose[2]);

    }

        void CompensatoryModel::calculateSteeringAngle(const ControlInput& input, const ControlParams& params)
        {
            double vxLim = std::max(params.vxMin, input.vxEgo);

            // calculate steering angle from the acceleration based on target curvature
            double targetCurvatureFF = m_targetAccelerationFF / std::pow(vxLim,2);
            m_targetSteeringAngleFF = std::atan(targetCurvatureFF*params.vehAxleDistance);
            
            double targetCurvatureFb = m_targetAccelerationFB / std::pow(vxLim,2);
            m_targetSteeringAngleFB = std::atan(targetCurvatureFb*params.vehAxleDistance);

            if(params.debugKPIs){printf("steering target ff: %f, fb: %f\n", m_targetSteeringAngleFF, m_targetSteeringAngleFB);}
        }


        double CompensatoryModel::steeringInverseDynamics(const double& steeringAngle, const ControlParams& params)
        {
            // u[k] = y[k-2]*(T^2/dT^2) + y[k-1]*(-2*xi*T/dT - 2*T^2/dT^2) + y[k]*(T^2/dT^2 + 2*xi*T/dT + 1)
            return m_actualSteeringAngleLog[1]*(pow(params.invSteerTimeConstant, 2)/(pow(params.dT, 2))) + 
                m_actualSteeringAngleLog[0]*((-2 * params.invSteerDamping * params.invSteerTimeConstant / params.dT)-(2 * pow(params.invSteerTimeConstant, 2) / pow(params.dT, 2))) +
                steeringAngle*(pow(params.invSteerTimeConstant, 2) / pow(params.dT, 2) + 2 * params.invSteerDamping * params.invSteerTimeConstant / params.dT + 1);
        }
    }
}