#include "../ctrl_vehicle_control/controller_inputs.hpp"
#include <math.h>

#ifndef POLYNOMIALCALCULATOR_HPP_
#define POLYNOMIALCALCULATOR_HPP_

namespace crp
{
    namespace apl
    {
        class polynomialCalculator{
            public:
                double* calculateThirdOrderPolynomial (std::vector<double> x, std::vector<double> y);

                double M [4][4] {0.0f};
                double detM;
                double b [4] {0.0f};

            private:
                void calculateBvector(std::vector<double> x, std::vector<double> y);
                void calculateMmatrix(std::vector<double> x);
                double calculateDeterminant(double Mx[4][4]);
                double calculateSubDeterminant(double Mx[4][4], int i);
                void calculateModifiedMMatrix(int i);
                
                double a[4];
                double v [9];
                double M_[4][4] {0.0f};

        };
    }
}

#endif