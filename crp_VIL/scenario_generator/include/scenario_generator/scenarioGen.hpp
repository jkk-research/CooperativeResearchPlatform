#ifndef CRP_VIL_SCENARIO_GEN_SCENARIOGENERATOR_HPP
#define CRP_VIL_SCENARIO_GEN_SCENARIOGENERATOR_HPP

#include <rclcpp/rclcpp.hpp>
#include <crp_msgs/msg/scenario.hpp>


class ScenarioGenerator : public rclcpp::Node
{
public:
    ScenarioGenerator();

private:
    void publishScenarioCallback();

    rclcpp::Publisher<crp_msgs::msg::Scenario>::SharedPtr m_pub_scenario_;

    crp_msgs::msg::Scenario m_scenario;
    rclcpp::TimerBase::SharedPtr m_publishTimer_;
};


#endif //CRP_VIL_SCENARIO_GEN_SCENARIOGENERATOR_HPP