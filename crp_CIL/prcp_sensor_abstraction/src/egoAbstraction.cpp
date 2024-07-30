#include "prcp_sensor_abstraction/egoAbstraction.hpp"


crp::cil::EgoAbstraction::EgoAbstraction() : Node("ego_abstraction")
{
    m_pub_kinematic_state_ = this->create_publisher<nav_msgs::msg::Odometry>("cai/ego/kinematic_state", 10);
}


int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::cil::EgoAbstraction>());
    rclcpp::shutdown();
    return 0;
}
