#ifndef CRP_APL_SCENARIO_FUSION_POSECORRECTION_HPP
#define CRP_APL_SCENARIO_FUSION_POSECORRECTION_HPP

namespace crp
{
    namespace apl
    {
        class PoseCorrection{
            public:
                double run (const double orientation[4]);
            
            private:
                double m_orientationInAngle{1.0f};
        };
    }
}

#endif // CRP_APL_SCENARIO_FUSION_POSECORRECTION_HPP