#include "compensatory_model/compensatory_model.hpp"

namespace crp
{
    namespace apl
    {
    
        void compensatoryModel::run (const controlInput& input, controlOutput& output, const controlParams& params){
                // this method calculates the PID controller target steering angle

                if (m_trajInvalid)
                {
                    printf("Trajectory is invalid\n");
                    printf("The following KPIs have been calculated\n");
                    printf("\t c0 RMS = %f\n\tc0 mean = %f\n\t cycle numbers %l\n", m_KPI_c0RMS, m_KPI_c0Mean, N);
                }
                else{
                    m_coefficients = m_polynomialCalculator.calculateThirdOrderPolynomial(input.path_x, input.path_y);
                    printf("coefficients are %f %f %f %f\n", m_coefficients[0], m_coefficients[1], m_coefficients[2], m_coefficients[3]);
                    // feedforward control
                    calculateFeedforward(input, params);

                    // feedback control
                    calculateFeedback(input, params);

                    // output calculation
                    calculateSteeringAngle(input, params);

                    // summing the target values to produce the target
                    output.accelerationTarget = m_targetAccelerationFF+m_targetAccelerationFB;
                    output.steeringAngleTarget = m_targetSteeringAngleFF+m_targetSteeringAngleFB;
                    output.steeringAngleTarget = m_steeringAngleFilter.lowPassFilter(output.steeringAngleTarget, m_targetSteeringAngle_prev, params.steeringAngleLPFilter);

                    output.steeringAngleTarget = steeringInverseDynamics(output.steeringAngleTarget, params);

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

        void compensatoryModel::calculateFeedforward(const controlInput& input, const controlParams& params)
        {
            // this method calculates the feedforward target steering angle.
            // First, the curvature is calculated in the preview distance.
            // Secondly, the curvature is transformed to a steering angle, with a velocity dependent 
            // gain. This gain is planned to be used to imitate slip compensation.
            // Third, the resulting feedforward steering angle is filtered using a simple low pass 
            // filter.
            
            m_lookAheadDistance = std::max(params.ffMinLookAheadDistance, params.ffLookAheadTime*input.vxEgo);
            // second derivative of the 3rd order polynomial
            m_targetCurvature = 2.0f*m_coefficients[2]+6.0f*m_coefficients[3]*m_lookAheadDistance;

            if(params.debugKPIs)
            {printf("data before limitation: c0=%f, lad=%f, kappa=%f\n", m_coefficients[0], m_lookAheadDistance, m_targetCurvature);}
            m_targetCurvature = std::max(std::min(m_targetCurvature, params.maxCurvature), -params.maxCurvature);
            if(params.debugKPIs){printf("kappa after limitation %f\n", m_targetCurvature);}
            // the target curvature is compensated with a velocity dependent scaling factor
            m_targetCurvature = m_targetCurvature*(params.ffGainOffsetGround+input.vxEgo*params.ffGainSlope);

            if(params.debugKPIs){printf("kappa after velocity compensation %f\n", m_targetCurvature);}
            // finally, lateral feedforward acceleration is calculated - this is done to be in harmony 
            // with the feedback path later (out of scope of this method)
            m_targetAccelerationFF = m_targetCurvature*std::pow(input.vxEgo,2);
            if(params.debugKPIs){printf("ayTarFF %f\n", m_targetAccelerationFF);}
        }

        void compensatoryModel::calculateFeedback(const controlInput& input, const controlParams& params)
        {
            // this method calculates the feedback target steering angle.
            // First, control error is defined. A separate FB look ahead distance is calculated for this.
            // orientation and position error are calculated.
            // Secondly, the integral and derival parts are calculated.
            // Third, the orientation error is calculated.
            // Fourth, the errors are weighted and summed with the corresponding control coefficients.
            // Finally, lateral acceleration target is mapped to steering angle target.

            m_lookAheadDistanceFb = std::max(params.fbMinLookAheadDistance, params.fbLookAheadTime*input.vxEgo);

            m_posErr = 
                m_coefficients[0]+
                m_coefficients[1]*m_lookAheadDistanceFb +
                m_coefficients[2]*std::pow(m_lookAheadDistanceFb,2)+
                m_coefficients[3]*std::pow(m_lookAheadDistanceFb,3);

            m_orientationErr = atan(
                m_coefficients[1]+
                2.0f*m_coefficients[2]*m_lookAheadDistanceFb+
                3.0f*m_coefficients[3]*std::pow(m_lookAheadDistanceFb,2));

            m_posIntegralError = m_posIntegralError+m_posErr*params.dT;

            m_posDerivativeError = (m_posErr-m_posErrPrev)/params.dT;
            m_posDerivativeError = m_posDerivativeFilter.lowPassFilter(m_posDerivativeError, m_posDerivativeError_prev, 0.9f);

            m_targetAccelerationFB = params.fbPGain*m_posErr +
                params.fbDGain*m_posDerivativeError;

            if (m_posIntegralError < params.fbIntegralLimit)
            {
                m_targetAccelerationFB = m_targetAccelerationFB +
                params.fbIGain*m_posIntegralError;
            }
            
            m_targetAccelerationFB = m_targetAccelerationFB + params.fbThetaGain*m_orientationErr;
            
            if(params.debugKPIs){printf("errors %f, %f, %f\n", m_posErr, m_posIntegralError, m_posDerivativeError); printf("ayTarFB %f\n", m_targetAccelerationFB);}

            m_posErrPrev = m_posErr;
        }

        void compensatoryModel::calculateSteeringAngle(const controlInput& input, const controlParams& params)
        {
            // calculate steering angle from the acceleration based on target curvature
            m_targetSteeringAngleFF = std::atan(m_targetCurvature*params.vehAxleDistance);

            double vxLim = std::max(params.vxMin, input.vxEgo);
            double targetCurvatureFb = m_targetAccelerationFB / std::pow(vxLim,2);
            m_targetSteeringAngleFB = std::atan(targetCurvatureFb*params.vehAxleDistance);

            if(params.debugKPIs){printf("steering target ff: %f, fb: %f\n", m_targetSteeringAngleFF, m_targetSteeringAngleFB);}
        }


        double compensatoryModel::steeringInverseDynamics(const double& steeringAngle, const controlParams& params)
        {
            // u[k] = y[k-2]*(T^2/dT^2) + y[k-1]*(-2*xi*T/dT - 2*T^2/dT^2) + y[k]*(T^2/dT^2 + 2*xi*T/dT + 1)
            return m_actualSteeringAngleLog[1]*(pow(params.invSteerTimeConstant, 2)/(pow(params.dT, 2))) + 
                m_actualSteeringAngleLog[0]*((-2 * params.invSteerDamping * params.invSteerTimeConstant / params.dT)-(2 * pow(params.invSteerTimeConstant, 2) / pow(params.dT, 2))) +
                steeringAngle*(pow(params.invSteerTimeConstant, 2) / pow(params.dT, 2) + 2 * params.invSteerDamping * params.invSteerTimeConstant / params.dT + 1);
        }
    }
}