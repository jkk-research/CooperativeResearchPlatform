from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration

def generate_launch_description():
    behavior_planner = Node(
        package="plan_behavior_planning",
        executable="behavior_planner",
        name="behavior_planner",
        output="screen",
    )

    return LaunchDescription([
        behavior_planner
    ])
