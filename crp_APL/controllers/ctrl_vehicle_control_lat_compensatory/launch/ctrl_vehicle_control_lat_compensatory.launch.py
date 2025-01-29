from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    ctrl_vehicle_control_lat_compensatory = Node(
        package="ctrl_vehicle_control_lat_compensatory",
        executable="ctrl_vehicle_control_lat_compensatory",
        parameters=[
            {"/ctrl/ffLookAheadTime": 1.5},
            {"/ctrl/steeringAngleLPFilter": 0.2},
            {"/ctrl/fbLookAheadTime": 0.2},
            {"/ctrl/fbPGain": 0.2},
            {"/ctrl/fbDGain": 1.0},
            {"/ctrl/fbIGain": 0.1},
            {"/ctrl/fbIntegralLimit": 3.0},
            {"/ctrl/trajectory_distance": 50.0},
            {"/ctrl/sigma_thetaFP": 0.05},
            {"/ctrl/maxThetaFP": 1.0},
            {"/ctrl/targetAccelerationFF_lpFilterCoeff": 0.8},
            {"/ctrl/targetAccelerationFB_lpFilterCoeff": 0.5},
        ]
    )

    return LaunchDescription([
        ctrl_vehicle_control_lat_compensatory
    ])