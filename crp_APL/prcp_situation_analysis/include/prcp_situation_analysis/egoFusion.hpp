#ifndef CRP_APL_EGO_FUSION_PRCPSITUATIONANALYSIS_HPP
#define CRP_APL_EGO_FUSION_PRCPSITUATIONANALYSIS_HPP

#include <rclcpp/rclcpp.hpp>
#include <crp_msgs/msg/ego.hpp>
#include <crp_msgs/msg/ego_status.hpp>
#include <autoware_localization_msgs/msg/kinematic_state.hpp>
#include <sensor_msgs/msg/nav_sat_fix.hpp>
#include "subfunctions/poseCorrection.hpp"

namespace crp
{
namespace apl
{

class EgoFusion : public rclcpp::Node
{
public:
    EgoFusion();

private:
    void egoKinematicStateCallback(const autoware_localization_msgs::msg::KinematicState::SharedPtr msg);
    void gnssFixCallback(const sensor_msgs::msg::NavSatFix::SharedPtr msg);
    void egoStatusCallback(const crp_msgs::msg::EgoStatus::SharedPtr msg);
    void publishCallback();

    rclcpp::Subscription<autoware_localization_msgs::msg::KinematicState>::SharedPtr m_sub_egoKinematicState_;
    rclcpp::Subscription<sensor_msgs::msg::NavSatFix>::SharedPtr                     m_sub_gnssFix_;
    rclcpp::Subscription<crp_msgs::msg::EgoStatus>::SharedPtr                        m_sub_egoStatus_;

    rclcpp::Publisher<crp_msgs::msg::Ego>::SharedPtr m_pub_ego_;

    rclcpp::TimerBase::SharedPtr m_publishTimer_;

    crp_msgs::msg::Ego m_ego;
    
    PoseCorrection m_poseCorrector;
};

} // namespace apl
} // namespace crp
#endif // CRP_APL_EGO_FUSION_PRCPSITUATIONANALYSIS_HPP