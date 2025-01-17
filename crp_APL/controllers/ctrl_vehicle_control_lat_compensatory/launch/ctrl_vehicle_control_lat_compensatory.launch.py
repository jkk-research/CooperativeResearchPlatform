from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    ctrl_vehicle_control_lat_compensatory = Node(
        package="ctrl_vehicle_control_lat_compensatory",
        executable="ctrl_vehicle_control_lat_compensatory",
        parameters=[
            {"/ctrl/ffGainOffsetGround": 0.66},
            {"/ctrl/ffGainSlope": 0.0},
            {"/ctrl/ffLookAheadTime": 0.68},
            {"/ctrl/steeringAngleLPFilter": 0.2},
            {"/ctrl/fbLookAheadTime": 0.6},
            {"/ctrl/fbPGain": 0.8},
            {"/ctrl/fbDGain": 1.1},
            {"/ctrl/fbIGain": 0.0},
            {"/ctrl/fbMinLookAheadDistance": 0.0},
            {"/ctrl/fbIntegralLimit": 3.0}
        ]
    )

    return LaunchDescription([
        ctrl_vehicle_control_lat_compensatory
    ])