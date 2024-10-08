from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    ld = LaunchDescription()

    ctrl_vehicle_control = Node(
        package="ctrl_vehicle_control",
        executable="ctrl_vehicle_control",
        parameters=[
            {"/ctrl/ffGainOffsetGround": 1.0},
            {"/ctrl/ffGainSlope": 0.1},
            {"/ctrl/ffLookAheadTime": 1.5},
            {"/ctrl/steeringAngleLPFilter": 0.7},
            {"/ctrl/fbLookAheadTime": 0.5},
            {"/ctrl/fbPGain": 0.01},
            {"/ctrl/fbDGain": 0.0},
            {"/ctrl/fbIGain": 0.0},
            {"/ctrl/fbMinLookAheadDistance": 0.0},
            {"/ctrl/fbIntegralLimit": 3.0}
        ]
    )

    ld.add_action(ctrl_vehicle_control)

    return ld