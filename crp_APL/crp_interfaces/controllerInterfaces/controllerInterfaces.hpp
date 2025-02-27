#ifndef CRP_APL_CONTROLLER_INTERFACES_INTERFACES_HPP
#define CRP_APL_CONTROLLER_INTERFACES_INTERFACES_HPP

#include <vector>

namespace crp
{
namespace apl
{
    struct ControlInput{
        std::vector<double> pathX;
        std::vector<double> pathY;
        std::vector<double> pathTheta;
        std::vector<double> pathVelocity;
        double egoPoseGlobal [3]{0.0f};
        double vxEgo{0.0f};
        double egoSteeringAngle{0.0f};
    };

    struct LatControlOutput{
        double accelerationTarget{0.0f};
        double steeringAngleTarget{0.0f};
    };

    struct LongControlOutput{
        double velocityTarget{0.0f};
    };

} // namespace apl
} // namespace crp

#endif // CRP_APL_CONTROLLER_INTERFACES_INTERFACES_HPP
