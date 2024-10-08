from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    ld = LaunchDescription()

    behavior_planner = Node(
        package="plan_behavior_planning",
        executable="behavior_planner",
        parameters=[
            {"/behavior_planner/current_scenario": "laneFollowWithSpeedAdjust"}
        ]
    )

    ld.add_action(behavior_planner)

    return ld