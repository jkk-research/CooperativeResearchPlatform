#ifndef CRP_APL_DATA_CONVERSION_GENERALUTILS_HPP
#define CRP_APL_DATA_CONVERSION_GENERALUTILS_HPP

#include <vector>
#include <cmath>

namespace crp
{
namespace apl
{

class DataConversion
{
public:
    /// @brief Transform quaternion to euler angles
    /// @param x Quaternion x value
    /// @param y Quaternion y value
    /// @param z Quaternion z value
    /// @param w Quaternion w value
    /// @return Vector of euler angles (roll, pitch, yaw)
    std::vector<double> quaternionToEuler(const double x, const double y, const double z, const double w);
    
    /// @brief Transform quaternion to euler angles
    /// @param quaternion Quaternion values array (x, y, z, w)
    /// @return Vector of euler angles (roll, pitch, yaw)
    std::vector<double> quaternionToEuler(const double quaternion[4]);
    
    /// @brief Transform euler angles to quaternion
    /// @param roll  Roll angle (x)
    /// @param pitch Pitch angle (y)
    /// @param yaw   Yaw angle (z)
    /// @return Vector of quaternion values (x, y, z, w)
    std::vector<double> eulerToQuaternion(const double roll, const double pitch, const double yaw);
    
    /// @brief Transform euler angles to quaternion
    /// @param angles Euler angles (roll, pitch, yaw)
    /// @return Vector of quaternion values (x, y, z, w)
    std::vector<double> eulerToQuaternion(const double angles[3]);
};

} // namespace apl
} // namespace crp

#endif // CRP_APL_DATA_CONVERSION_GENERALUTILS_HPP