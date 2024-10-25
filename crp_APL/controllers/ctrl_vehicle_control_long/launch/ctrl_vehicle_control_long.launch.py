from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    ld = LaunchDescription()

    ctrl_vehicle_control_long = Node(
        package="ctrl_vehicle_control_long",
        executable="ctrl_vehicle_control_long",
        parameters=[
            {"/ctrl/axMax": 2.0},
            {"/ctrl/axMin": -2.0},
            {"/ctrl/jxMax": 1.0},
            {"/ctrl/jxMin": -1.0},
            {"/ctrl/speedControlLookAheadTime": 0.6}
        ]
    )

    ld.add_action(ctrl_vehicle_control_long)

    return ld