from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration

def generate_launch_description():
    ld = LaunchDescription()

    behavior_planner = Node(
        package="plan_behavior_planning",
        executable="behavior_planner"
    )

    ld.add_action(behavior_planner)

    return ld