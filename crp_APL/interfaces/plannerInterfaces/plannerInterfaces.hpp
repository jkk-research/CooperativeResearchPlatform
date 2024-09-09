#ifndef CRP_APL_PLANNER_INTERFACES_INTERFACES_HPP
#define CRP_APL_PLANNER_INTERFACES_INTERFACES_HPP

#include <vector>
#include <cstdint>


namespace crp
{
namespace apl
{

struct Vector2D
{
    float x{0.0f};
    float y{0.0f};
};

struct Vector3D
{
    float x{0.0f};
    float y{0.0f};
    float z{0.0f};
};

typedef Vector2D Point2D;
typedef Vector3D Point3D;

struct Pose2D
{
    Point2D position;
    float theta{0.0f};
};

struct TrajectoryPoint
{
    Pose2D pose;
    float velocity{0.0f};
};

typedef std::vector<TrajectoryPoint> PlannerOutputTrajectory;
typedef std::vector<std::vector<float>> OccupancyGrid;

struct PlannerInputPose2D
{
    Pose2D pose;
    float kappa {0.0f}; // road curvature in 1/m
};

struct PlannerInputTrafficRule
{
    uint8_t laneEdgeTypeLeft{0U};
    uint8_t laneEdgeTypeRight{0U};
    PlannerInputPose2D stoppingPose;
};

struct PlannerInputTrajectory
{
    // path extended with relevant velocity
    std::vector<PlannerInputPose2D> pathPoints;
    uint32_t laneID{0U};
    std::vector<float> targetSpeed;
};

struct PlannerInputKinematics2D
{
    // velocity in 2D
    float vX{0.0f};
    float vY{0.0f};
    float yawRate{0.0f};
    // acceleration in 2D
    float aX{0.0f};
    float aY{0.0f};
    float yawAcceleration{0.0f};
};

struct Shape
{
    enum ShapeType : uint8_t
    {
        BOUNDING_BOX = 0,
        CYLINDER = 1,
        POLYGON = 2
    };
    ShapeType type;
    std::vector<Point3D> polygon;
    Vector3D dimensions;
};

struct PlannerInputObject
{
    uint32_t                  objectID{0U}; // 0 means no object
    float                     existenceProbability{0.0f};
    uint8_t                   classificationID{0U}; // classification, 0 means unknown
    float                     classificationProbability{0.0f};
    PlannerInputPose2D        initialPose;
    PlannerInputKinematics2D  objectKinematics;
    PlannerInputTrajectory    predictedPath;
    Shape                     shape;
};

struct PlannerInput
{
    // strategy
    std::string currentScenario;

    // target pose for relevant planners
    PlannerInputPose2D targetPose;
    // moving relevant objects
    std::vector<PlannerInputObject> relevantObjects;
    // static objects as obstacles
    std::vector<PlannerInputObject> relevantObstacles;
    // pure path
    PlannerInputTrajectory path;
    // traffic rule(s)
    PlannerInputTrafficRule trafficRules;
    // cost map of the local area
    OccupancyGrid freeSpace;
    // maximum allowed speed, in m/s
    float maximumSpeed{0.0f};

    // ego pose
    Pose2D egoPose;
    // ego velocity
    PlannerInputKinematics2D egoKinematics;
};

struct PlannerOutput
{
    PlannerOutputTrajectory trajectory;
};

} // namespace apl
} // namespace crp
#endif // CRP_APL_PLANNER_INTERFACES_INTERFACES_HPP