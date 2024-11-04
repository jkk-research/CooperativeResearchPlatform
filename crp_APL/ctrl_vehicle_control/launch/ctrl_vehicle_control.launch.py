from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    ld = LaunchDescription()

    ctrl_vehicle_control = Node(
        package="ctrl_vehicle_control",
        executable="ctrl_vehicle_control"
    )

    ld.add_action(ctrl_vehicle_control)

    return ld