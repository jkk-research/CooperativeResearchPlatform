#include "../inc/dataConversion.hpp"


std::vector<double> crp::apl::DataConversion::quaternionToEuler (const double x, const double y, const double z, const double w)
{
    std::vector<double> angles(3);

    // roll (x-axis rotation)
    double sinr_cosp = 2 * (w * x + y * z);
    double cosr_cosp = 1 - 2 * (x * x + y * y);
    angles[0] = std::atan2(sinr_cosp, cosr_cosp);

    // pitch (y-axis rotation)
    double sinp = std::sqrt(1 + 2 * (w * y - x * z));
    double cosp = std::sqrt(1 - 2 * (w * y - x * z));
    angles[1] = 2 * std::atan2(sinp, cosp) - M_PI / 2;

    // yaw (z-axis rotation)
    double siny_cosp = 2 * (w * z + x * y);
    double cosy_cosp = 1 - 2 * (y * y + z * z);
    angles[2] = std::atan2(siny_cosp, cosy_cosp);

    return angles;
}

std::vector<double> crp::apl::DataConversion::quaternionToEuler(const double quaternion[4])
{
    return quaternionToEuler(quaternion[0], quaternion[1], quaternion[2], quaternion[3]);
}

std::vector<double> crp::apl::DataConversion::eulerToQuaternion(const double roll, const double pitch, const double yaw)
{
    std::vector<double> quat(4);

    double cr = std::cos(roll * 0.5);
    double sr = std::sin(roll * 0.5);
    double cp = std::cos(pitch * 0.5);
    double sp = std::sin(pitch * 0.5);
    double cy = std::cos(yaw * 0.5);
    double sy = std::sin(yaw * 0.5);

    quat[3]= cr * cp * cy + sr * sp * sy;  // w
    quat[0]= sr * cp * cy - cr * sp * sy;  // x
    quat[1] = cr * sp * cy + sr * cp * sy; // y
    quat[2] = cr * cp * sy - sr * sp * cy; // z

    return quat;
}

std::vector<double> crp::apl::DataConversion::eulerToQuaternion(const double angles[3])
{
    return eulerToQuaternion(angles[0], angles[1], angles[2]);
}
