#include "prcp_sensor_abstraction/abstractionUtils.hpp"


float crp::cil::AbstractionUtils::distanceBetweenPoints(const lanelet::BasicPoint2d a, const lanelet::ConstPoint2d b)
{
    return sqrt(pow(b.x()-a.x(), 2)+pow(b.y()-a.y(), 2));
}


float crp::cil::AbstractionUtils::distanceBetweenPoints(const tier4_planning_msgs::msg::PathPointWithLaneId a, const tier4_planning_msgs::msg::PathPointWithLaneId b)
{
    return sqrt(pow(b.point.pose.position.x-a.point.pose.position.x, 2)+pow(b.point.pose.position.y-a.point.pose.position.y, 2));
}


uint16_t crp::cil::AbstractionUtils::getGPSNNPointIdx(const lanelet::BasicPoint2d & currentPos, const lanelet::ConstLanelet & lane)
{
    lanelet::ConstLineString2d centerline = lane.centerline2d();
    double minDist = distanceBetweenPoints(currentPos, centerline.front());
    uint16_t minDistIdx = 0;
    for (uint16_t i = 1; i < centerline.size(); i++)
    {
        double currentDist = distanceBetweenPoints(currentPos, centerline[i]);
        if (currentDist < minDist)
        {
            minDist = currentDist;
            minDistIdx = i;
        }
    }
    return minDistIdx;
}


float crp::cil::AbstractionUtils::calcYawAngle(
    const tier4_planning_msgs::msg::PathPointWithLaneId & prevPoint,
    const tier4_planning_msgs::msg::PathPointWithLaneId & pathPoint)
{
    // calculate yaw angle with derivative
    return atan2(
        pathPoint.point.pose.position.y - prevPoint.point.pose.position.y,
        pathPoint.point.pose.position.x - prevPoint.point.pose.position.x
    );
}


void crp::cil::AbstractionUtils::filterMovingAverage(std::vector<float> & data, uint8_t windowSize)
{
    std::vector<float> filteredData;
    uint8_t currentHalfWindow = 1;
    uint16_t dataSize = data.size();
    for (uint16_t i = 0; i < dataSize; i++)
    {
        currentHalfWindow = (uint16_t)std::min((windowSize-1)/2, std::min((int)i, (dataSize-(i+1))));
        float sum = 0;
        for (uint16_t j = i-currentHalfWindow; j <= i+currentHalfWindow; j++)
        {
            sum += data[j];
        }
        filteredData.push_back(sum/(currentHalfWindow*2+1));
    }
    data = filteredData;
}


void crp::cil::AbstractionUtils::calcPathOrientation(
    crp_msgs::msg::PathWithTrafficRules & path)
{
    std::vector<float> orientations;
    tier4_planning_msgs::msg::PathPointWithLaneId prevPoint = path.path.points[0];
    orientations.push_back(calcYawAngle(prevPoint, path.path.points[1]));
    for (uint16_t i = 1; i < path.path.points.size(); i++)
        orientations.push_back(calcYawAngle(path.path.points[i-1], path.path.points[i]));
    
    filterMovingAverage(orientations, 5);
    
    for (uint16_t i = 0; i < path.path.points.size(); i++)
    {
        tf2::Quaternion quat;
        quat.setRPY(0.0f, 0.0f, orientations[i]);
        path.path.points[i].point.pose.orientation = tf2::toMsg(quat);
    }
}


float crp::cil::AbstractionUtils::getYawFromQuaternion(const geometry_msgs::msg::Quaternion & quaternion)
{
    tf2::Quaternion q(
        quaternion.x,
        quaternion.y,
        quaternion.z,
        quaternion.w);
    tf2::Matrix3x3 m(q);
    double roll, pitch, yaw;
    m.getRPY(roll, pitch, yaw);

    return yaw;
}


tier4_planning_msgs::msg::PathPointWithLaneId crp::cil::AbstractionUtils::transformToLocal(
    const tier4_planning_msgs::msg::PathPointWithLaneId & pathPoint,
    const geometry_msgs::msg::PoseWithCovarianceStamped & ego)
{
    float egoYaw = getYawFromQuaternion(ego.pose.pose.orientation);

    float R[2]{
        cos(egoYaw),
        sin(egoYaw)};
    float translatedX{0.0};
    float translatedY{0.0};

    translatedX = pathPoint.point.pose.position.x - ego.pose.pose.position.x;
    translatedY = pathPoint.point.pose.position.y - ego.pose.pose.position.y;

    tier4_planning_msgs::msg::PathPointWithLaneId outPoint;
    outPoint.point.pose.position.x = R[0] * translatedX + R[1] * translatedY;
    outPoint.point.pose.position.y = -R[1] * translatedX + R[0] * translatedY;
    outPoint.point.longitudinal_velocity_mps = pathPoint.point.longitudinal_velocity_mps;

    return outPoint;
}


tier4_planning_msgs::msg::PathPointWithLaneId crp::cil::AbstractionUtils::laneletPtToPathPoint(
    const lanelet::ConstPoint2d & pt, float speedLimit)
{
    tier4_planning_msgs::msg::PathPointWithLaneId pathPoint;
    pathPoint.point.pose.position.x = pt.x();
    pathPoint.point.pose.position.y = pt.y();
    pathPoint.point.longitudinal_velocity_mps = speedLimit;
    return pathPoint;
}
