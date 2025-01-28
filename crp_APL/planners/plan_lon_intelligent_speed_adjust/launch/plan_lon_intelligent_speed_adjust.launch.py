from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    # NODES
    plan_lon_intelligent_speed_adjust = Node(
        package='plan_lon_intelligent_speed_adjust',
        executable='plan_lon_intelligent_speed_adjust',
        name='plan_lon_intelligent_speed_adjust',
        output='screen',
        parameters=[
            {"/plan_lon_intelligent_speed_adjust/useCurvatureBasedSpeedAdjust": 0.0},
        ]
    )

    return LaunchDescription([
        plan_lon_intelligent_speed_adjust
    ])