#ifndef CRP_APL_COMPENSATORY_MODEL_CTRLVEHICLECONTROL_HPP
#define CRP_APL_COMPENSATORY_MODEL_CTRLVEHICLECONTROL_HPP

#include <cmath>
#include <stdio.h>

#include "ctrl_vehicle_control/controller_inputs.hpp"
#include "ctrl_vehicle_control/controller_outputs.hpp"
#include "lib/polynomialCalculator.hpp"
#include "lib/utils.hpp"
#include "lib/polyfitBoost.hpp"


namespace crp
{
    namespace apl
    {
        class CompensatoryModel{
            public:
                void run (ControlInput& input, ControlOutput& output, const ControlParams& params);
            
            private:
                PolynomialCalculator m_polynomialCalculator;
                Transforms m_transforms;
                Filters m_steeringAngleFilter;
                Filters m_posDerivativeFilter;


                double* m_coefficients;
                std::vector<double> m_localPath_x;
                std::vector<double> m_localPath_y;
                std::vector<double> m_localPathCut_x;
                std::vector<double> m_localPathCut_y;
                bool m_trajInvalid{false};

                // FEEDFORWARD
                double m_lookAheadDistance{0.0f};
                double m_targetCurvature{0.0f};
                double m_targetAccelerationFF{0.0f};
                double m_targetSteeringAngleFF{0.0f};

                // FEEDBACK
                double m_targetAccelerationFB{0.0f};
                double m_targetSteeringAngleFB{0.0f};
                double m_lookAheadDistanceFb{0.0f};
                double m_posErr{0.0f};
                double m_posIntegralError{0.0f};
                double m_posDerivativeError{0.0f};
                double m_posDerivativeError_prev{0.0f};
                double m_posErrPrev{0.0f};
                double m_orientationErr{0.0f};

                double m_targetSteeringAngle_prev{0.0f};
                double m_actualSteeringAngleLog[2]{0.0f, 0.0f};

                // FILTERS
                double m_steeringTarget_prev{0.0f};
                double m_derivativeError_prev{0.0f};

                // KPIs - for debug
                double m_KPI_c0RMS{0.0f};
                double m_KPI_c0Mean{0.0f};
                double m_KPI_c0MS{0.0f};
                double m_KPI_c0Max{4e8};
                unsigned long int N{0};

                // METHODS
                void calculateFeedforward(const ControlInput& input, const ControlParams& params);
                void calculateFeedback(const ControlInput& input, const ControlParams& params);

                void calculateSteeringAngle(const ControlInput& input, const ControlParams& params);

                void cutRelevantLocalSnippet();

                double steeringInverseDynamics(const double& steeringAngle, const ControlParams& params);
        };
    }
}

#endif