#ifndef CRP_APL_PLANNER_INTERFACES_INTERFACES_HPP
#define CRP_APL_PLANNER_INTERFACES_INTERFACES_HPP

#include <vector>
#include <cstdint>
#include <string>


namespace crp
{
namespace apl
{

struct Vector3D
{
    float x{0.0f}; // in m
    float y{0.0f}; // in m
    float z{0.0f}; // in m
};

typedef Vector3D Point3D;

struct Pose3D
{
    Point3D position; 
    float   orientation{0.0f}; // in rad
};

struct TrajectoryPoint
{
    Pose3D pose;
    float  velocity{0.0f}; // in m/s
};

typedef std::vector<TrajectoryPoint> PlannerOutputTrajectory;
typedef std::vector<std::vector<float>> OccupancyGrid;

struct PlannerInputTrafficRule
{
    // TODO: Define lane edge types
    uint8_t laneEdgeTypeLeft{0U};
    uint8_t laneEdgeTypeRight{0U};
    Pose3D  stoppingPose;
};

struct PlannerInputPathPoint
{
    Pose3D pose;
    float  curvature{0.0f}; // in 1/m
};

struct PlannerInputTrajectory
{
    // path extended with relevant velocity
    std::vector<PlannerInputPathPoint> pathPoints;
    uint32_t            laneID{0U};
    std::vector<float>  targetSpeed; // in m/s
};

struct PlannerInputKinematics3D
{
    // velocity in 3D
    float vX{0.0f}; // in m/s
    float vY{0.0f}; // in m/s
    float yawRate{0.0f}; // in rad/s
    // acceleration in 3D
    float aX{0.0f}; // in m/s^2
    float aY{0.0f}; // in m/s^2
    float yawAcceleration{0.0f}; // in rad/s^2
};

struct Shape
{
    enum ShapeType : uint8_t
    {
        BOUNDING_BOX = 0,
        CYLINDER = 1,
        POLYGON = 2
    };
    ShapeType            type;
    Vector3D             dimensions;
    std::vector<Point3D> polygon;
};

struct UserInputs
{
    uint8_t blinker{0u};
    float targetSpeed {0.0f};
};

struct BehaviorInputs
{
    uint8_t accelerationMode{0u};
    uint8_t decelerationMode{0u};
    uint8_t curveSpeedMode{0u};
    uint8_t corneringSpeedMode{0u};
};

struct PlannerInputObject
{
    uint32_t                  objectID{0U}; // 0 means no object
    float                     existenceProbability{0.0f};
    uint8_t                   classificationID{0U}; // classification, 0 means unknown
    float                     classificationProbability{0.0f};
    Pose3D                    initialPose;
    PlannerInputKinematics3D  objectKinematics;
    PlannerInputTrajectory    predictedPath;
    Shape                     shape;
};

struct PlannerInput
{
    // strategy
    std::string currentScenario;

    // target pose for relevant planners
    Pose3D targetPose;
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
    Pose3D egoPose;
    // ego velocity
    PlannerInputKinematics3D egoKinematics;

    // behavior inputs
    BehaviorInputs behaviorInputs;
};

struct PlannerOutput
{
    PlannerOutputTrajectory trajectory;
};

struct BehaviorPlannerInput
{
    UserInputs m_userInputs;
    BehaviorInputs m_behaviorInputs;
};

} // namespace apl
} // namespace crp
#endif // CRP_APL_PLANNER_INTERFACES_INTERFACES_HPP