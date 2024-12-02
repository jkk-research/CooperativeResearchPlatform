from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration

def generate_launch_description():
    ld = LaunchDescription()

    behavior_current_scenario_arg = DeclareLaunchArgument(
        'behavior_current_scenario',
        default_value='laneFollowWithSpeedAdjust',
        description='Current scenario ("off", "laneFollowWithDefaultSpeed", "laneFollowWithSpeedAdjust", "laneFollow", "speedAdjust")'
    )

    behavior_planner = Node(
        package="plan_behavior_planning",
        executable="behavior_planner",
        parameters=[
            {"/behavior_planner/current_scenario": LaunchConfiguration("behavior_current_scenario")}
        ]
    )

    ld.add_action(behavior_current_scenario_arg)
    ld.add_action(behavior_planner)

    return ld