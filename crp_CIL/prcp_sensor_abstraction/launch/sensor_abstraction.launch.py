from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    scenario_abstraction = Node(
        package='prcp_sensor_abstraction',
        executable='scenario_abstraction',
        name='scenario_abstraction',
        output='screen',
        parameters=[
            {'local_path_length': 70.0}
        ]
    )

    return LaunchDescription([
        scenario_abstraction
    ])
