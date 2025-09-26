#include "plan_behavior_planning/behaviorPlanner.hpp"


crp::apl::BehaviorPlanner::BehaviorPlanner() : Node("behavior_planner")
{
    m_sub_route_    = this->create_subscription<autoware_planning_msgs::msg::LaneletRoute>(
        "plan/route", 10, std::bind(&BehaviorPlanner::routeCallback, this, std::placeholders::_1));
    m_sub_scenario_ = this->create_subscription<crp_msgs::msg::Scenario>(
        "scenario", 10, std::bind(&BehaviorPlanner::scenarioCallback, this, std::placeholders::_1));
    m_sub_ego_      = this->create_subscription<crp_msgs::msg::Ego>(
        "ego", 10, std::bind(&BehaviorPlanner::egoCallback, this, std::placeholders::_1));

        m_sub_behavior_ = this->create_subscription<crp_msgs::msg::Behavior>(
        "/ui/behavior", 10,
        std::bind(&BehaviorPlanner::behaviorCallback, this, std::placeholders::_1));

    m_pdp_subscriber_ = this->create_subscription<pdp_if::msg::PdpPersonalizedParamsActive>(
        "/PdpPersonalizedParamsActive", 10, std::bind(&BehaviorPlanner::PdpPersonalizedParamsActiveCallback, this, std::placeholders::_1));


    m_pub_strategy_    = this->create_publisher<tier4_planning_msgs::msg::Scenario>("plan/strategy", 10);
    m_pub_targetSpace_ = this->create_publisher<crp_msgs::msg::TargetSpace>("plan/target_space", 10);
    m_pub_laneVisualization_ = this->create_publisher<visualization_msgs::msg::Marker>("plan/laneVisualization", 10);
    m_pub_targetSpeedVisualization_ = this->create_publisher<visualization_msgs::msg::Marker>("plan/targetSpeedVisualization", 10);

    m_timer_ = this->create_wall_timer(std::chrono::milliseconds(33), std::bind(&BehaviorPlanner::loop, this));

    RCLCPP_INFO(this->get_logger(), "behavior_planner has been started");
}


void crp::apl::BehaviorPlanner::routeCallback(const autoware_planning_msgs::msg::LaneletRoute::SharedPtr msg)
{
    // TODO
    (void)msg;
    return;
}

void crp::apl::BehaviorPlanner::PdpPersonalizedParamsActiveCallback(const pdp_if::msg::PdpPersonalizedParamsActive::SharedPtr msg)
{
    if (msg->pdpout_accmap_lat_mode == 1U)
    {
        m_corneringSpeed = 3.0f;
    }
    else if(msg->pdpout_accmap_lat_mode == 2U)
    {
        m_corneringSpeed = 4.0f;
    }
    else if(msg->pdpout_accmap_lat_mode == 3U)
    {
        m_corneringSpeed = 6.0f;
    }
    else
    {
        m_corneringSpeed = 3.0f;
    }
}

void crp::apl::BehaviorPlanner::scenarioCallback(const crp_msgs::msg::Scenario::SharedPtr msg)
{
    crp_msgs::msg::TargetSpace targetSpaceMsg;
    crp_msgs::msg::PathWithTrafficRules fixedPathWithTrafficRules;

    if (msg->paths.size() > 0){
        targetSpaceMsg.path = msg->paths[0];
        // compensate the orientation error
        for (int i=0; i<targetSpaceMsg.path.path.points.size(); i++)
        {
            tier4_planning_msgs::msg::PathPointWithLaneId point;
            point.point.pose.position.x = targetSpaceMsg.path.path.points.at(i).point.pose.position.x*std::cos(m_orientationError) - targetSpaceMsg.path.path.points.at(i).point.pose.position.y*std::sin(m_orientationError);
            point.point.pose.position.y = targetSpaceMsg.path.path.points.at(i).point.pose.position.x*std::sin(m_orientationError) + targetSpaceMsg.path.path.points.at(i).point.pose.position.y*std::cos(m_orientationError);  
            fixedPathWithTrafficRules.path.points.push_back(point);
        }
    }

    targetSpaceMsg.path = fixedPathWithTrafficRules;

    if (m_behaviorPlannerInput.m_userInputs.blinker > 0)
    {
        targetSpaceMsg.path.traffic_rules.maximum_speed = std::min(m_corneringSpeed, 
            m_behaviorPlannerInput.m_userInputs.targetSpeed);
    }
    else{
        targetSpaceMsg.path.traffic_rules.maximum_speed = 
            m_behaviorPlannerInput.m_userInputs.targetSpeed;
    }

    targetSpaceMsg.free_space = msg->free_space;
    
    m_pub_targetSpace_->publish(targetSpaceMsg);

    // visualization
    visualization_msgs::msg::Marker lane;
    lane.header.frame_id = "base_link";
    lane.header.stamp = this->get_clock()->now();
    lane.ns = "lanes";
    lane.id = 1;
    lane.type = visualization_msgs::msg::Marker::SPHERE_LIST;
    lane.action = visualization_msgs::msg::Marker::ADD;
    lane.scale.x = 0.2; // line width
    lane.scale.y = 0.2;
    lane.scale.z = 0.2;
    lane.color.r = 1.0f;
    lane.color.g = 1.0f;
    lane.color.b = 0.0f;
    lane.color.a = 1.0f;

    // speed in each waypoint
    std::vector<visualization_msgs::msg::Marker> labels;

    if (msg->paths.size() > 0){
        // compensate the orientation error
        for (int i=0; i<fixedPathWithTrafficRules.path.points.size(); i++)
        {
            geometry_msgs::msg::Point p;
            p.x = fixedPathWithTrafficRules.path.points.at(i).point.pose.position.x;
            p.y = fixedPathWithTrafficRules.path.points.at(i).point.pose.position.y;  
            lane.points.push_back(p);

            // speed points
            visualization_msgs::msg::Marker label;
            label.header.frame_id = "base_link";
            label.ns = "labels";
            label.id = i + 1;
            label.type = visualization_msgs::msg::Marker::TEXT_VIEW_FACING;
            label.action = visualization_msgs::msg::Marker::ADD;
            label.scale.z = 0.2;  // text height
            label.color.r = 0.0;
            label.color.g = 0.0;
            label.color.b = 0.0;
            label.color.a = 1.0;
            label.pose.position = p;
            label.pose.position.z = 1.0;
            std::stringstream ss;
            ss << std::fixed << std::setprecision(2) << fixedPathWithTrafficRules.path.points.at(i).point.longitudinal_velocity_mps*3.6;
            label.text = ss.str() + " km/h";
            labels.push_back(label);
        }
        m_pub_laneVisualization_->publish(lane);

        for (auto &label : labels) {
            label.header.stamp = this->get_clock()->now();
            m_pub_targetSpeedVisualization_->publish(label);
        }
    }    
}


void crp::apl::BehaviorPlanner::egoCallback(const crp_msgs::msg::Ego::SharedPtr msg)
{
    m_behaviorPlannerInput.m_userInputs.blinker = msg->blinker.data;
    
    // TODO: this should be relocated into CIL!!!
    m_orientationError = -(msg->estimated_orientation - msg->orientation);

    return;
}


void crp::apl::BehaviorPlanner::behaviorCallback(const crp_msgs::msg::Behavior::SharedPtr msg)
{
    RCLCPP_INFO(this->get_logger(), "Behavior: %d", msg->target_speed.data);
    m_behaviorPlannerInput.m_userInputs.targetSpeed = msg->target_speed.data;
}


void crp::apl::BehaviorPlanner::loop()
{
    // possible Scenrios: "off", "laneFollowWithDefaultSpeed", "", "laneFollow", "speedAdjust"
    std::string currentScenarioMsg = "laneFollowWithSpeedAdjust";

    tier4_planning_msgs::msg::Scenario scenario_msg;
    scenario_msg.current_scenario = currentScenarioMsg;

    m_pub_strategy_->publish(scenario_msg);
    
    return;
}


int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::apl::BehaviorPlanner>());
    rclcpp::shutdown();
    return 0;
}