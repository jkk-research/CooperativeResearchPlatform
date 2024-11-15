#include "../inc/geometricPathCalculation.hpp"

std::vector<float> crp::apl::GeometricPathCalculation::calculateCurvature(const std::vector<float> x,
        const std::vector<float> y)
{
   std::vector<float> curvature;
   std::vector<float> orientation;

   if (x.size()>0)
   {
      curvature.push_back(0.0f);
      orientation.push_back(0.0f);
   }
   for (int i=1; i<x.size(); i++)
   {
      // conduct first derivation
      float dx = x.at(i)-x.at(i-1);
      float dy = y.at(i)-y.at(i-1);
      orientation.push_back(p_forgettingFactor*orientation.at(i-1)+(1.0f-p_forgettingFactor)*dy/dx);
      float dorientation = orientation.at(i)-orientation.at(i-1);
      curvature.push_back(p_forgettingFactor*curvature.at(i-1)+(1.0f-p_forgettingFactor)*dorientation/dx);
      //printf("dx for %d is %f\t orientation is %f\t curvature is %f\n", i, dx, orientation.at(i), curvature.at(i));
   }

   return curvature;
}