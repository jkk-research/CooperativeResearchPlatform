#include "mpc_25_wrapper/mpc_25_wrapper.hpp"


crp::cil::Mpc25Wrapper::Mpc25Wrapper() : Node("mpc_25_wrapper")
{
    m_sub_mpcCameraInput_ = this->create_subscription<crp_if::msg::MpcCameraInput>("/mpc25/mpc_camera_input", 10, std::bind(&Mpc25Wrapper::mpcCameraInputCallback, this, std::placeholders::_1));

    m_pub_localLanesFromCamera_ = this->create_publisher<tier4_planning_msgs::msg::PathWithLaneId>("/sensing/camera/local_lanes", 10);

    RCLCPP_INFO(this->get_logger(), "mpc_25_wrapper has been started");
}

void crp::cil::Mpc25Wrapper::mpcCameraInputCallback(const crp_if::msg::MpcCameraInput::SharedPtr msg)
{
    tier4_planning_msgs::msg::PathWithLaneId localLanesFromCamera;
    localLanesFromCamera.header = msg->zzz_header;
    localLanesFromCamera.header.frame_id = "base_link";

    tier4_planning_msgs::msg::PathPointWithLaneId localPathPoint;

    // road_line_1 process
    float c0 = 0.5f*(msg->road_line_1.dy + msg->road_line_2.dy);

    float c1
        = (msg->road_line_1.line_lookahead * msg->road_line_1.gierw_diff + msg->road_line_2.line_lookahead * msg->road_line_2.gierw_diff) / 
        (msg->road_line_1.line_lookahead + msg->road_line_2.line_lookahead);

    float c2 = (msg->road_line_1.hor_curvature / 65536.0 * msg->road_line_1.line_lookahead  + 
        msg->road_line_2.hor_curvature / 65536.0 * msg->road_line_2.line_lookahead)
                     / (msg->road_line_1.line_lookahead + msg->road_line_2.line_lookahead);

    int N = (int)(msg->road_line_1.line_lookahead / p_pathStepSize);
    for (int i=0; i<N; i++)
    {
        // loop through all the points and evaluate the coefficients
        float x_i = i*p_pathStepSize;
        float y_i = c0+c1*x_i+c2*pow(x_i,2);
        autoware_planning_msgs::msg::PathPoint point;
        point.pose.position.x = x_i;
        point.pose.position.y = y_i;
        localPathPoint.point = point;
        localLanesFromCamera.points.push_back(localPathPoint);
    }
    
    m_pub_localLanesFromCamera_->publish(localLanesFromCamera);

}

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::cil::Mpc25Wrapper>());
    rclcpp::shutdown();
    return 0;
}