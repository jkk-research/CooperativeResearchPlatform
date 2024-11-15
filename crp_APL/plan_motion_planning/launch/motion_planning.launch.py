from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    motion_planning = Node(
        package="plan_motion_planning",
        executable="motion_handler",
        name="motion_handler",
        output="screen"
    )

    return LaunchDescription([
        motion_planning
    ])

    return ld