#include "lib/polynomialCalculator.hpp"

namespace crp{
    namespace apl{
        double PolynomialCalculator::calculateThirdOrderPolynomial (
        std::vector<double> x, std::vector<double> y){
            calculateMmatrix(x);
            detM = calculateDeterminant(M);
            calculateBvector(x,y);

            for (int i=0; i<4; i++){
                calculateModifiedMMatrix(i);
                a[i] = calculateDeterminant(M_) / detM;
            }

            return a;
        }

        void PolynomialCalculator::calculateBvector(
        std::vector<double> x, std::vector<double> y){
            double sum = 0.0f;
            for (int i=0; i<4; i++){
                sum = 0;
                for(unsigned long int j=0; j<x.size(); j++){
                    sum = sum + pow(x.at(j),i)*
                        y.at(j);
                }
                b[i] = sum;
            }

        }

        void PolynomialCalculator::calculateMmatrix(
        std::vector<double> x){
            double sum = 0.0f;
            for(int i=0; i<9; i++){
                sum = 0;
                for (unsigned long int j=0; j<x.size(); j++){
                    sum = sum + pow(x.at(j),i);
                }
                v[i] = sum;
            }
            for (int i=0; i<4; i++){
                for (int j=0; j<4; j++){
                    M[i][j] = v[i+j];
                }
            }
        }

        void PolynomialCalculator::calculateModifiedMMatrix(int k){
            for (int i=0; i<4; i++){
                for (int j=0; j<4; j++){
                    if (j==k){
                        M_[i][j] = b[i];
                    }
                    else{
                        M_[i][j] = M[i][j];
                    }                 
                }
            }
        }

        double PolynomialCalculator::calculateDeterminant(double Mx[4][4]){
            double D = 0;
            for (int i=0; i<4; i++){
                double dM = calculateSubDeterminant(Mx, i);
                if (i==0 || i==2){
                    D = D + Mx[0][i]*dM;
                }
                else{
                    D = D - Mx[0][i]*dM;
                }
            }
            return D;

        }
        double PolynomialCalculator::calculateSubDeterminant(double Mx[4][4], int i){
            double m[3][3];
            int columns[3];
            switch (i){
                case 0:
                    columns[0] = 1;
                    columns[1] = 2;
                    columns[2] = 3;
                    break;
                case 1:
                    columns[0] = 0;
                    columns[1] = 2;
                    columns[2] = 3;
                    break;
                case 2:
                    columns[0] = 0;
                    columns[1] = 1;
                    columns[2] = 3;
                    break;
                case 3:
                    columns[0] = 0;
                    columns[1] = 1;
                    columns[2] = 2;
                    break;            
            }
            for (int j=0; j<3; j++){
                for (int k=0; k<3; k++){
                    m[j][k] = Mx[j+1][columns[k]];
                }
            }

            return m[0][0] * (m[1][1]*m[2][2]-m[1][2]*m[2][1]) -
                    m[0][1] * (m[1][0]*m[2][2]-m[1][2]*m[2][0]) + 
                    m[0][2] * (m[1][0]*m[2][1]-m[1][1]*m[2][0]);
        }

    }
}