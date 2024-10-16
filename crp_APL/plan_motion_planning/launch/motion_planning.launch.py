from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    behavior_planner = Node(
        package="plan_motion_planning",
        executable="motion_handler",
        name="motion_handler",
        output="screen"
    )

    return LaunchDescription([
        behavior_planner
    ])

    return ld