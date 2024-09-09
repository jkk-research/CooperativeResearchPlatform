#include "../../../inc/lib/utils.hpp"

namespace crp{
    namespace apl{
    
    double geometricOperators::transformQuatToEuler (const double quaternion [4]){

            /*// roll (x-axis rotation)
            double sinr_cosp = 2 * (quaternion[3] * quaternion[0] + quaternion[1] * quaternion[2]);
            double cosr_cosp = 1 - 2 * (quaternion[0] * quaternion[0] + quaternion[1] * quaternion[1]);
            angles.roll = std::atan2(sinr_cosp, cosr_cosp);

            // pitch (y-axis rotation)
            double sinp = std::sqrt(1 + 2 * (quaternion[3] * quaternion[1] - quaternion[0] * quaternion[2]));
            double cosp = std::sqrt(1 - 2 * (quaternion[3] * quaternion[1] - quaternion[0] * quaternion[2]));
            angles.pitch = 2 * std::atan2(sinp, cosp) - M_PI / 2;*/

            // yaw (z-axis rotation)
            double siny_cosp = 2 * (quaternion[3] * quaternion[2] + quaternion[0] * quaternion[1]);
            double cosy_cosp = 1 - 2 * (quaternion[1] * quaternion[1] + quaternion[2] * quaternion[2]);
            double angle = std::atan2(siny_cosp, cosy_cosp);

            return angle;
        }

        double* geometricOperators::ToQuaternion(double roll, double pitch, double yaw) // roll (x), pitch (y), yaw (z), angles are in radians
        {
            // Abbreviations for the various angular functions

            double cr = std::cos(roll * 0.5);
            double sr = std::sin(roll * 0.5);
            double cp = std::cos(pitch * 0.5);
            double sp = std::sin(pitch * 0.5);
            double cy = std::cos(yaw * 0.5);
            double sy = std::sin(yaw * 0.5);

            q[3]= cr * cp * cy + sr * sp * sy;
            q[0]= sr * cp * cy - cr * sp * sy;
            q[1] = cr * sp * cy + sr * cp * sy;
            q[2] = cr * cp * sy - sr * sp * cy;

            return q;
        }

        double* transforms::global2Ego (double point[3], const double egoPose[3]){
            // global to ego frame
            // transform matrix: T = [cos(theta) sin(theta); -sin(theta) cos(theta)]

            pointEgo[2] = point[2] - egoPose[2];
            pointEgo[0] = cos(egoPose[2])*(point[0]-egoPose[0]) + sin(egoPose[2])*(point[1]-egoPose[1]);
            pointEgo[1] = -sin(egoPose[2])*(point[0]-egoPose[0]) + cos(egoPose[2])*(point[1]-egoPose[1]);

            return pointEgo;
        }

        double* transforms::ego2Global (double point[3], const double egoPose[3]){
            // global to ego frame
            // transform matrix: T = [cos(theta) sin(theta); -sin(theta) cos(theta)]

            pointEgo[2] = point[2] + egoPose[2];
            pointEgo[0] = cos(egoPose[2])*point[0] - sin(egoPose[2])*point[1]+egoPose[0];
            pointEgo[1] = sin(egoPose[2])*point[0]+ cos(egoPose[2])*point[1]+egoPose[1];

            return pointEgo;
        }

        double filters::lowPassFilter(double unfiltered, double filtered_prev, double coefficient)
        {
            return (coefficient*unfiltered+(1.0f-coefficient)*filtered_prev);
        }
    }
}
