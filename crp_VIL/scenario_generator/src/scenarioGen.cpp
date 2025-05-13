#include <scenario_generator/scenarioGen.hpp>


ScenarioGenerator::ScenarioGenerator() : Node("scenario_gen")
{
    m_pub_scenario_ = this->create_publisher<crp_msgs::msg::Scenario>("/scenario", 10);
    
    this->declare_parameter<double>("local_path_length", 70.0);
    double pathLength;
    this->get_parameter("local_path_length", pathLength);

    crp_msgs::msg::PathWithTrafficRules path;
    for (uint16_t i = 0; i <= ceil(pathLength); ++i)
    {
        tier4_planning_msgs::msg::PathPointWithLaneId pt;
        pt.point.pose.position.x = i;
        pt.point.pose.position.y = 0;
        pt.point.longitudinal_velocity_mps = 30.0;
        path.path.points.push_back(pt);
    }
    m_scenario.paths.push_back(path);

    m_publishTimer_ = this->create_wall_timer(
        std::chrono::milliseconds(20),
        std::bind(&ScenarioGenerator::publishScenarioCallback, this)
    );
}

void ScenarioGenerator::publishScenarioCallback()
{
    m_scenario.header.stamp = this->now();
    m_pub_scenario_->publish(m_scenario);
}

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ScenarioGenerator>());
    rclcpp::shutdown();
    return 0;
}

// m_scenario.header.stamp = this->now();