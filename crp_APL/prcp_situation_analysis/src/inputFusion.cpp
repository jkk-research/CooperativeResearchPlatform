#include "prcp_situation_analysis/inputFusion.hpp"


crp::apl::InputFusion::InputFusion() : Node("input_fusion")
{
    // world
    m_sub_global_static_map_ = this->create_subscription<autoware_map_msgs::msg::LaneletMapBin>(
        "cai/global_static_map/lanelet2_map", 1, std::bind(&InputFusion::globalStaticMapCallback, this, std::placeholders::_1));
    m_sub_global_moving_objects_ = this->create_subscription<autoware_perception_msgs::msg::PredictedObjects>(
        "cai/global_moving_objects", 1, std::bind(&InputFusion::globalMovingObjectsCallback, this, std::placeholders::_1));
    // scenario
    m_sub_local_moving_objects_ = this->create_subscription<autoware_perception_msgs::msg::PredictedObjects>(
        "cai/local_moving_objects", 1, std::bind(&InputFusion::localMovingObjectsCallback, this, std::placeholders::_1));
    m_sub_local_obstacles_ = this->create_subscription<autoware_perception_msgs::msg::PredictedObjects>(
        "cai/local_obstacles", 1, std::bind(&InputFusion::localObstaclesCallback, this, std::placeholders::_1));

     m_sub_local_lane_path_ = this->create_subscription<tier4_planning_msgs::msg::PathWithLaneId>(
        "cai/local_lane/path", 1, std::bind(&InputFusion::localLanePathCallback, this, std::placeholders::_1));
   
    m_sub_local_drivable_surface_ = this->create_subscription<nav_msgs::msg::OccupancyGrid>(
        "cai/local_drivable_surface", 1, std::bind(&InputFusion::localDrivableSurfaceCallback, this, std::placeholders::_1));
    // ego
    m_sub_ego_kinematic_state_ = this->create_subscription<autoware_localization_msgs::msg::KinematicState>(
        "cai/ego/kinematic_state", 1, std::bind(&InputFusion::egoKinematicStateCallback, this, std::placeholders::_1));

    
    m_pub_world_    = this->create_publisher<crp_msgs::msg::World>("world", 1);
    m_pub_scenario_ = this->create_publisher<crp_msgs::msg::Scenario>("scenario", 1);
    m_pub_ego_      = this->create_publisher<crp_msgs::msg::Ego>("ego", 1);
}

void crp::apl::InputFusion::localLanePathCallback(const tier4_planning_msgs::msg::PathWithLaneId::SharedPtr msg)
{
    // TODO
    return;
}


void crp::apl::InputFusion::globalStaticMapCallback(const autoware_map_msgs::msg::LaneletMapBin::SharedPtr msg)
{
    // TODO
    return;
}


void crp::apl::InputFusion::globalMovingObjectsCallback(const autoware_perception_msgs::msg::PredictedObjects::SharedPtr msg)
{
    // TODO
    return;
}


void crp::apl::InputFusion::localMovingObjectsCallback(const autoware_perception_msgs::msg::PredictedObjects::SharedPtr msg)
{
    // TODO
    return;
}


void crp::apl::InputFusion::localObstaclesCallback(const autoware_perception_msgs::msg::PredictedObjects::SharedPtr msg)
{
    // TODO
    return;
}





void crp::apl::InputFusion::localDrivableSurfaceCallback(const nav_msgs::msg::OccupancyGrid::SharedPtr msg)
{
    // TODO
    return;
}


void crp::apl::InputFusion::egoKinematicStateCallback(const autoware_localization_msgs::msg::KinematicState::SharedPtr msg)
{
    // TODO
    return;
}


int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::apl::InputFusion>());
    rclcpp::shutdown();
    return 0;
}
