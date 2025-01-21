from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    ld = LaunchDescription()

    ctrl_vehicle_control_lat_stanley = Node(
        package="ctrl_vehicle_control_lat_stanley",
        executable="ctrl_vehicle_control_lat_stanley",
        parameters=[
            {"/ctrl/k_gain": 1.5},
            {"/ctrl/wheelbase": 2.7},
        ]
    )

    ld.add_action(ctrl_vehicle_control_lat_stanley)

    return ld