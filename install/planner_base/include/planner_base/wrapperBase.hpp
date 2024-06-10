#ifndef CRP_APL_WRAPPER_BASE_PLANBASE_HPP
#define CRP_APL_WRAPPER_BASE_PLANBASE_HPP


#include <rclcpp/rclcpp.hpp>
#include <crp_msgs/msg/target_space.hpp>
#include <crp_msgs/msg/ego.hpp>
#include <tier4_planning_msgs/msg/scenario.hpp>
#include <autoware_planning_msgs/msg/trajectory.hpp>


namespace crp
{
namespace apl
{

class WrapperBase : public rclcpp::Node
{
public:
    WrapperBase(const std::string & node_name, const rclcpp::NodeOptions & options = rclcpp::NodeOptions());

protected:
    // PlannerInput substructs
    struct PlannerInputPose2D
    {
        float x{0.0f}; // meters
        float y{0.0f}; // meters
        float theta{0.0f}; // road orientation in radian
        float kappa {0.0f}; // road curvature in 1/m
    };

    struct PlannerInputPath
    {
        // directly taken from path with lane id definition
        std::vector<PlannerInputPose2D> pathPoint;
    };

    struct PlannerInputTrafficRule
    {
        uint8_t laneEdgeTypes[2]{0U}; // 2 lane edges of the relevant lane
        PlannerInputPose2D stoppingPose;
    };

    struct PlannerInputTrajectory
    {
        // path extended with relevant velocity
        std::vector<PlannerInputPose2D> pathPoint;
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

    struct PlannerInputObject
    {
        uint32_t                    objectID{0U}; // 0 means no object
        float                       existenceProbability{0.0f};
        uint8_t                     classificationID{0U}; // classification, 0 means unknown
        float                       classificationProbability{0.0f};
        PlannerInputPose2D          initialPose;
        PlannerInputKinematics2D    objectKinematics;
        PlannerInputTrajectory      predictedPath;
        PlannerInputTrajectory      path;
        // TODO object geometry struct to be added
    };

    struct PlannerInput
    {
        // The following signals are directly taken from the 'target space' definition
 
        // moving relevant objects
        std::vector<PlannerInputObject> relevantObjects;
        // static objects as obstacles
        std::vector<PlannerInputObject> relevantObstacles;
        // pure path
        PlannerInputTrajectory path;
        // traffic rule(s)
        PlannerInputTrafficRule trafficRules;
        // cost map of the local area
        std::vector<std::vector<float>> freeSpace; 
        // maximum allowed speed, in m/s
        float maximumSpeed{0.0f}; 
    };

    struct PlannerOutput
    {
        // TODO: standar c++ definition of the output trajectory message
    };

    virtual void plan(const PlannerInput & input, PlannerOutput & output) = 0;

private:
    void strategyCallback(const tier4_planning_msgs::msg::Scenario::SharedPtr msg);
    void targetSpaceCallback(const crp_msgs::msg::TargetSpace::SharedPtr msg);
    void egoCallback(const crp_msgs::msg::Ego::SharedPtr msg);
    
    void outputCPP2ROS(const PlannerOutput & output, autoware_planning_msgs::msg::Trajectory & msg);
    void publishTrajectory(const PlannerOutput & trajectory);
    void run();

    rclcpp::Subscription<tier4_planning_msgs::msg::Scenario>::SharedPtr m_sub_strategy_;
    rclcpp::Subscription<crp_msgs::msg::TargetSpace>::SharedPtr         m_sub_target_space_;
    rclcpp::Subscription<crp_msgs::msg::Ego>::SharedPtr                 m_sub_ego_;

    rclcpp::Publisher<autoware_planning_msgs::msg::Trajectory>::SharedPtr m_pub_trajectory_;

    PlannerInput  m_input;
    PlannerOutput m_output;
};

} // namespace apl
} // namespace crp
#endif // CRP_APL_WRAPPER_BASE_PLANBASE_HPP
