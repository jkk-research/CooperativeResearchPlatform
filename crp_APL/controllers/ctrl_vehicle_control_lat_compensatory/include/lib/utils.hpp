#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <cmath>
#include <stdio.h>

namespace crp
{
    namespace apl{
        
        class GeometricOperators{
            public:
                double transformQuatToEuler (const double quaternion [4]);
                double* ToQuaternion(double roll, double pitch, double yaw);
            private:
                double q[4]{0.0f};
        };

        class Transforms{
            public:
                double* global2Ego (double point[3], const double egoPose[3]);
                double* ego2Global (double point[3], const double egoPose[3]);

            private:
                double pointEgo[3]{0.0f};
        };

        class Filters{
            public:
                double lowPassFilter(double unfiltered, double filtered_prev, double coefficient);
        };
    }
}

#endif