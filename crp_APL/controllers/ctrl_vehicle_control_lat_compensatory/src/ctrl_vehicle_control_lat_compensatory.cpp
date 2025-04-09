#include <ctrl_vehicle_control_lat_compensatory/ctrl_vehicle_control_lat_compensatory.hpp>


crp::apl::CtrlVehicleControlLatCompensatory::CtrlVehicleControlLatCompensatory() : CtrlLatWrapperBase("ctrl_vehicle_control_lat_compensatory")
{
    this->declare_parameter("/ctrl/compensatory/ffLookAheadTime", 0.68f);
    this->declare_parameter("/ctrl/compensatory/steeringAngleLPFilter", 0.2f);
    this->declare_parameter("/ctrl/compensatory/fbLookAheadTime", 0.0f);
    this->declare_parameter("/ctrl/compensatory/fbPGain", 0.8f);
    this->declare_parameter("/ctrl/compensatory/fbDGain", 1.1f);
    this->declare_parameter("/ctrl/compensatory/fbIGain", 0.0f);
    this->declare_parameter("/ctrl/compensatory/fbIntegralLimit", 3.0f);
    this->declare_parameter("/ctrl/compensatory/trajectory_distance", 50.0f);

    this->declare_parameter("/ctrl/compensatory/sigma_thetaFP", 0.25f);
    this->declare_parameter("/ctrl/compensatory/maxThetaFP", 0.3f);
    this->declare_parameter("/ctrl/compensatory/targetAccelerationFF_lpFilterCoeff", 0.99f);
    this->declare_parameter("/ctrl/compensatory/targetAccelerationFB_lpFilterCoeff", 0.99f);
        
    RCLCPP_INFO(this->get_logger(), "ctrl_vehicle_control_lat_compensatory has been started");
}

void crp::apl::CtrlVehicleControlLatCompensatory::controlLoop(const ControlInput & input, LatControlOutput & output)
{
    // parameter assignments
    m_params.ffLookAheadTime       = this->get_parameter("/ctrl/compensatory/ffLookAheadTime").as_double();
    m_params.steeringAngleLPFilter = this->get_parameter("/ctrl/compensatory/steeringAngleLPFilter").as_double();
    m_params.fbLookAheadTime       = this->get_parameter("/ctrl/compensatory/fbLookAheadTime").as_double();
    m_params.fbPGain               = this->get_parameter("/ctrl/compensatory/fbPGain").as_double();
    m_params.fbDGain               = this->get_parameter("/ctrl/compensatory/fbDGain").as_double();
    m_params.fbIGain               = this->get_parameter("/ctrl/compensatory/fbIGain").as_double();
    m_params.fbIntegralLimit       = this->get_parameter("/ctrl/compensatory/fbIntegralLimit").as_double();
    m_params.trajectory_distance   = this->get_parameter("/ctrl/compensatory/trajectory_distance").as_double();

    m_params.sigma_thetaFP                      = this->get_parameter("/ctrl/compensatory/sigma_thetaFP").as_double();
    m_params.maxThetaFP                         = this->get_parameter("/ctrl/compensatory/maxThetaFP").as_double();
    m_params.targetAccelerationFF_lpFilterCoeff = this->get_parameter("/ctrl/compensatory/targetAccelerationFF_lpFilterCoeff").as_double();
    m_params.targetAccelerationFB_lpFilterCoeff = this->get_parameter("/ctrl/compensatory/targetAccelerationFB_lpFilterCoeff").as_double();

    m_compensatoryModel.run(input, output, m_params);
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::apl::CtrlVehicleControlLatCompensatory>());
    rclcpp::shutdown();
    return 0;
}