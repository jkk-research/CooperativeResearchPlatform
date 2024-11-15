#ifndef POLYNOMIAL_PATHCALCULATION_HPP_INCLUDED
#define POLYNOMIAL_PATHCALCULATION_HPP_INCLUDED

#include <string>
#include <vector>
#include <cmath>

namespace crp
{
namespace apl
{
class GeometricPathCalculation
{
public:
   // main function for curvature calculation
   std::vector<float> calculateCurvature(const std::vector<float> x,
        const std::vector<float> y);

private:
    float p_forgettingFactor{0.95f};
   
};
} // namespace apl
} // namespace crp

#endif  // POLYNOMIAL_PATHCALCULATION_HPP_INCLUDED