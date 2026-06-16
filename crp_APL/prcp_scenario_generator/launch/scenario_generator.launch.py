from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    prcp_scenario_generator = Node(
        namespace='',
        package='prcp_scenario_generator',
        executable='prcp_scenario_generator',
        name='prcp_scenario_generator',
        output='screen'
    )
    
    return LaunchDescription([
        prcp_scenario_generator
    ])