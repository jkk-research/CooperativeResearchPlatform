#include "wrapperPlanLatLaneFollowLdm.hpp"


crp::apl::PlanLatLaneFollowHandler::PlanLatLaneFollowHandler() : WrapperBase("plan_lat_lane_follow")
{
    this->declare_parameter<std::vector<double>>(
        "/plan_lat_lane_follow_ldm/nodePointDistances", std::vector<double>{25.0f, 50.0f, 75.0f});
    this->declare_parameter(
        "/plan_lat_lane_follow_ldm/trajectoryResolution", 1.0f);
    this->declare_parameter<std::vector<double>>(
        "/plan_lat_lane_follow_ldm/pLeft", std::vector<double>{
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f});
    this->declare_parameter<std::vector<double>>(
        "/plan_lat_lane_follow_ldm/pRight", std::vector<double>{
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f});
    this->declare_parameter(
        "/plan_lat_lane_follow_ldm/pStraight", 0.0f);     
}


void crp::apl::PlanLatLaneFollowHandler::plan(const PlannerInput & input, PlannerOutput & output)
{
    output.trajectory.clear(); // initialize the output at empty vector in the beginning of each cycle
    TrajectoryPoint trajectoryPoint;
    for (int n=0; n<input.path.pathPoints.size(); n++)
        {
            trajectoryPoint.pose.position.x = input.path.pathPoints.at(n).pose.position.x;
            trajectoryPoint.pose.position.y = input.path.pathPoints.at(n).pose.position.y;
            trajectoryPoint.pose.orientation = 0.0;
            trajectoryPoint.velocity = input.path.targetSpeed.at(n);
            output.trajectory.push_back(trajectoryPoint);
        }
    
    // remap parameters
    /*std::vector<double> nodePointDistances;
    this->get_parameter<std::vector<double>>(
        "/plan_lat_lane_follow_ldm/nodePointDistances", nodePointDistances);
    for (int np=0; np<3; np++)
    {
        m_ldmParams.P_nodePointDistances[np] = nodePointDistances.at(np);   
    }
    std::vector<double> pLeft;
    this->get_parameter<std::vector<double>>(
        "/plan_lat_lane_follow_ldm/pLeft", pLeft);
    m_ldmParams.P[0U] = pLeft.at(0U); 
    m_ldmParams.P[1U] = pLeft.at(1U); 
    m_ldmParams.P[2U] = pLeft.at(2U); 
    m_ldmParams.P[7U] = pLeft.at(3U); 
    m_ldmParams.P[8U] = pLeft.at(4U); 
    m_ldmParams.P[9U] = pLeft.at(5U); 
    m_ldmParams.P[14U] = pLeft.at(6U); 
    m_ldmParams.P[15U] = pLeft.at(7U); 
    m_ldmParams.P[16U] = pLeft.at(8U); 

    std::vector<double> pRight;
    this->get_parameter<std::vector<double>>(
        "/plan_lat_lane_follow_ldm/pRight", pRight);
    m_ldmParams.P[3U] = pRight.at(0U); 
    m_ldmParams.P[4U] = pRight.at(1U); 
    m_ldmParams.P[5U] = pRight.at(2U); 
    m_ldmParams.P[10U] = pRight.at(3U); 
    m_ldmParams.P[11U] = pRight.at(4U); 
    m_ldmParams.P[12U] = pRight.at(5U); 
    m_ldmParams.P[17U] = pRight.at(6U); 
    m_ldmParams.P[18U] = pRight.at(7U); 
    m_ldmParams.P[19U] = pRight.at(8U); 

    this->get_parameter<double>(
        "/plan_lat_lane_follow_ldm/pStraight", m_ldmParams.P[6U]);
    this->get_parameter<double>(
        "/plan_lat_lane_follow_ldm/pStraight", m_ldmParams.P[13U]);
    this->get_parameter<double>(
        "/plan_lat_lane_follow_ldm/pStraight", m_ldmParams.P[20U]);    

    if (inputPlausibilityCheck(input))
    {
        // fit polynomial
        calculateNodePoints(input);

        // get ego pose from input
        generateEgoPose(input);

        // run path planner including LDM
        TrajectoryOutput trajectoryOutput = m_linearDriverModel.runCoeffsLite(
            m_scenarioPolynomials,
            m_egoPose,
            m_ldmParams
        );

        // map to node output
        float x = 0.0f; // starting from local zero coordinate
        float y = 0.0f;
        float theta = 0.0f;
        uint8_t relevantSegment = 0U;
        TrajectoryPoint trajectoryPoint;

        while(x<input.path.pathPoints.at(input.path.pathPoints.size()-1).pose.position.x)
        {
            // loop through all points
            // finding the right segment first
            for (uint8_t np=0; np<4;np++)
            {
                if (x<trajectoryOutput.nodePts.nodePointsCoordinates[np].x)
                {
                    relevantSegment = np-1U;
                    break;
                }
            }
            y = trajectoryOutput.segmentCoeffs.segmentCoeffs[relevantSegment].c0 +
                trajectoryOutput.segmentCoeffs.segmentCoeffs[relevantSegment].c1*x+
                trajectoryOutput.segmentCoeffs.segmentCoeffs[relevantSegment].c2*std::pow(x,2)+
                trajectoryOutput.segmentCoeffs.segmentCoeffs[relevantSegment].c3*std::pow(x,3);
            theta = std::atan(trajectoryOutput.segmentCoeffs.segmentCoeffs[relevantSegment].c1+
                2.0f*trajectoryOutput.segmentCoeffs.segmentCoeffs[relevantSegment].c2*x+
                3.0f*trajectoryOutput.segmentCoeffs.segmentCoeffs[relevantSegment].c3*std::pow(x,2));
            
            trajectoryPoint.pose.position.x = x;
            trajectoryPoint.pose.position.y = y;
            trajectoryPoint.pose.orientation = theta;
            trajectoryPoint.velocity = input.maximumSpeed;

            output.trajectory.push_back(trajectoryPoint);

            x = x + static_cast<float>(
                this->get_parameter("/plan_lat_lane_follow_ldm/trajectoryResolution").as_double());
        }
    }*/
}

// method calculates the subsegments of the incoming trajectory based on a LDMParamIn type parameters struct
void crp::apl::PlanLatLaneFollowHandler::calculateNodePoints(
    const PlannerInput & input)
{
    float distance{0.0f};
    float averageCurvatureBetweenNodepoints{0.0f};
    std::vector<float> segmentPointsX;
    std::vector<float> segmentPointsY;
    uint16_t startIdx{0U};
    m_scenarioPolynomials.coeffs.clear();
    m_scenarioPolynomials.kappaNominal.clear();

    for (uint8_t np=0U; 
        np<sizeof(m_ldmParams.P_nodePointDistances)/sizeof(*m_ldmParams.P_nodePointDistances); np++)
        {
            segmentPointsX.clear();
            segmentPointsY.clear();
            averageCurvatureBetweenNodepoints = 0.0f;
            for (uint16_t p=startIdx; p<input.path.pathPoints.size(); p++)
            {
                distance = std::sqrt(std::pow(input.path.pathPoints.at(p).pose.position.x,2)+
                    std::pow(input.path.pathPoints.at(p).pose.position.y,2));
                if (distance > m_ldmParams.P_nodePointDistances[np])
                {
                    startIdx = p;
                    break;
                }
                segmentPointsX.push_back(input.path.pathPoints.at(p).pose.position.x);
                segmentPointsY.push_back(input.path.pathPoints.at(p).pose.position.y);
                averageCurvatureBetweenNodepoints = ((p-startIdx)*averageCurvatureBetweenNodepoints+
                    input.path.pathPoints.at(p).curvature)/(p-startIdx+1);
            }
            if (segmentPointsX.size() > 0U)
            {
                PolynomialCoeffs coeffs;
                std::vector<float> c = 
                    m_polynomialRegressor.fitThirdOrderPolynomial(segmentPointsX,segmentPointsY);
                coeffs.c0 = c.at(0U);
                coeffs.c1 = c.at(1U);
                coeffs.c2 = c.at(2U);
                coeffs.c3 = c.at(3U);

                m_scenarioPolynomials.coeffs.push_back(coeffs);
                m_scenarioPolynomials.kappaNominal.push_back(averageCurvatureBetweenNodepoints);
            }
            else
            {
                PolynomialCoeffs coeffs;
                coeffs.c0 = 0.0f;
                coeffs.c1 = 0.0f;
                coeffs.c2 = 0.0f;
                coeffs.c3 = 0.0f;
                m_scenarioPolynomials.coeffs.push_back(coeffs);
                m_scenarioPolynomials.kappaNominal.push_back(0.0f);
            }
        }
}

void crp::apl::PlanLatLaneFollowHandler::generateEgoPose(
    const PlannerInput & input)
{
    m_egoPose.Pose2DCoordinates.x = input.egoPose.position.x;
    m_egoPose.Pose2DCoordinates.y = input.egoPose.position.y;
    m_egoPose.Pose2DTheta = input.egoPose.orientation;
}


int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::apl::PlanLatLaneFollowHandler>());
    rclcpp::shutdown();
    return 0;
}
