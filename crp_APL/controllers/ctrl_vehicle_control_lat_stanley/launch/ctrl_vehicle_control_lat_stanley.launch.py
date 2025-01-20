from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    ctrl_vehicle_control_lat_stanley = Node(
        package="ctrl_vehicle_control_lat_stanley",
        executable="ctrl_vehicle_control_lat_stanley",
        parameters=[
            {"/ctrl/k_gain": 0.5},
            {"/ctrl/wheelbase": 2.7},
        ]
    )

    return LaunchDescription([
        ctrl_vehicle_control_lat_stanley
    ])
