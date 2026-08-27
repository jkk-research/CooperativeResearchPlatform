from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    
    scenario_abstraction = Node(
        package='prcp_sensor_abstraction',
        executable='scenario_abstraction',
        name='scenario_abstraction',
        output='screen'
    )
    
    ego_abstraction = Node(
        package='prcp_sensor_abstraction',
        executable='ego_abstraction',
        name='ego_abstraction',
        output='screen'
    )

    return LaunchDescription([
        scenario_abstraction,
        ego_abstraction
    ])