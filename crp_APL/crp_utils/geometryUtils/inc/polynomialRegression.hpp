#ifndef POLYNOMIAL_REGRESSION_HPP_INCLUDED
#define POLYNOMIAL_REGRESSION_HPP_INCLUDED

#include <string>
#include <vector>
#include <cmath>

namespace crp
{
namespace apl
{
class PolynomialSubfunctions
{
public:
   // main function for gauss elimination
   void gaussElimination(float (&gaussMatrix)[4][5], float(&coefficients)[4]);
   // main function for 3rd order polynomial fitting
   std::vector<float> fitThirdOrderPolynomial(
        const std::vector<float> x,
        const std::vector<float> y);

private:
   // helper methods for gauss elimination
   uint8_t forwardElimination(float (&gaussMatrix)[4][5]);
   void backSubstitute(float (&gaussMatrix)[4][5]);
   // helper variable for gauss elimination
   float m_gaussResult[4]{0.0f};
   // helper methods for polynomial fitting
   void calculateBvector(const std::vector<float> x,
        const std::vector<float> y);
   void calculateMmatrix(const std::vector<float> x);
   double calculateDeterminant(double Mx[4][4]);
   double calculateSubDeterminant(double Mx[4][4], uint8_t);
   void calculateModifiedMMatrix(uint8_t);

   float  m_a[4];
   double m_v[9];
   double m_M_[4][4]{{0.0f}};
   double m_M[4][4]{{0.0f}};
   double m_detM;
   double m_b[4]{0.0f};
};
} // namespace apl
} // namespace crp

#endif  // POLYNOMIAL_REGRESSION_HPP_INCLUDED