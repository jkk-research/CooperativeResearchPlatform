from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    ld = LaunchDescription()

    ctrl_vehicle_control_lat_pure_p = Node(
        package="ctrl_vehicle_control_lat_pure_p",
        executable="ctrl_vehicle_control_lat_pure_p",
        parameters=[
            {"/ctrl/lookahead_time": 1.0},
            {"/ctrl/wheelbase": 2.7},
        ]
    )

    ld.add_action(ctrl_vehicle_control_lat_pure_p)

    return ld