from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    ctrl_vehicle_control = Node(
        package="ctrl_vehicle_control",
        executable="control_handler"
    )

    return LaunchDescription([
       ctrl_vehicle_control
    ])
