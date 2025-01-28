from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    ctrl_vehicle_control_lat_compensatory = Node(
        package="ctrl_vehicle_control_lat_compensatory",
        executable="ctrl_vehicle_control_lat_compensatory",
        parameters=[
            {"/ctrl/ffLookAheadTime": 0.68},
            {"/ctrl/steeringAngleLPFilter": 0.2},
            {"/ctrl/fbLookAheadTime": 0.6},
            {"/ctrl/fbPGain": 0.5},
            {"/ctrl/fbDGain": 0.1},
            {"/ctrl/fbIGain": 0.0},
            {"/ctrl/fbIntegralLimit": 3.0},
            {"/ctrl/trajectory_distance": 50.0},
            {"/ctrl/sigma_thetaFP": 0.25},
            {"/ctrl/maxThetaFP": 0.3},
            {"/ctrl/targetAccelerationFF_lpFilterCoeff": 0.99},
            {"/ctrl/targetAccelerationFB_lpFilterCoeff": 0.99},
        ]
    )

    return LaunchDescription([
        ctrl_vehicle_control_lat_compensatory
    ])