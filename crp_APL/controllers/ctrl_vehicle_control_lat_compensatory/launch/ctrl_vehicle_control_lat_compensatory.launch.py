from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    ld = LaunchDescription()

    ctrl_vehicle_control_lat_compensatory = Node(
        package="ctrl_vehicle_control_lat_compensatory",
        executable="ctrl_vehicle_control_lat_compensatory",
        parameters=[
            {"/ctrl/ffGainOffsetGround": 1.0},
            {"/ctrl/ffGainSlope": 0.0},
            {"/ctrl/ffLookAheadTime": 0.68},
            {"/ctrl/steeringAngleLPFilter": 0.2},
            {"/ctrl/fbLookAheadTime": 0.6},
            {"/ctrl/fbPGain": 0.5},
            {"/ctrl/fbDGain": 0.1},
            {"/ctrl/fbIGain": 0.0},
            {"/ctrl/fbMinLookAheadDistance": 0.0},
            {"/ctrl/fbIntegralLimit": 3.0},
            {"/ctrl/fbThetaGain": 0.0},
        ]
    )

    ld.add_action(ctrl_vehicle_control_lat_compensatory)

    return ld