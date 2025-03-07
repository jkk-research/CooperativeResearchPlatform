#ifndef CRP_CIL_ABSTRACTION_UTILS_PRCPSENSORABSTRACTION_HPP
#define CRP_CIL_ABSTRACTION_UTILS_PRCPSENSORABSTRACTION_HPP

#include <rclcpp/rclcpp.hpp>

#include <tier4_planning_msgs/msg/path_point_with_lane_id.hpp>
#include <tier4_planning_msgs/msg/path_with_lane_id.hpp>
#include <geometry_msgs/msg/pose_with_covariance_stamped.hpp>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>
#include <lanelet2_core/geometry/Lanelet.h>
#include <crp_msgs/msg/path_with_traffic_rules.hpp>

#include <vector>

namespace crp
{
namespace cil
{

class AbstractionUtils
{
public:
    float    getYawFromQuaternion(const geometry_msgs::msg::Quaternion & quaternion);
    float    distanceBetweenPoints(const lanelet::BasicPoint2d a, const lanelet::ConstPoint2d b);
    float    distanceBetweenPoints(const tier4_planning_msgs::msg::PathPointWithLaneId a, const tier4_planning_msgs::msg::PathPointWithLaneId b);
    uint16_t getGPSNNPointIdx(const lanelet::BasicPoint2d & currentPos, const lanelet::ConstLanelet & lane);
    
    float    calcYawAngle(
        const tier4_planning_msgs::msg::PathPointWithLaneId & prevPoint,
        const tier4_planning_msgs::msg::PathPointWithLaneId & pathPoint);

    void     filterMovingAverage(std::vector<float> & data, uint8_t windowSize);
    void     calcPathOrientation(
        crp_msgs::msg::PathWithTrafficRules & path);

    tier4_planning_msgs::msg::PathPointWithLaneId transformToLocal(
        const tier4_planning_msgs::msg::PathPointWithLaneId & pathPoint,
        const geometry_msgs::msg::PoseWithCovarianceStamped & ego);
    tier4_planning_msgs::msg::PathPointWithLaneId laneletPtToPathPoint(
        const lanelet::ConstPoint2d & pt,float speedLimit);
};

} // namespace cil
} // namespace crp
#endif // CRP_CIL_ABSTRACTION_UTILS_PRCPSENSORABSTRACTION_HPP