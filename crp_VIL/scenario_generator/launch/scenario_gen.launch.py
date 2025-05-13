from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node

def generate_launch_description():
    local_path_length_arg = DeclareLaunchArgument(
        'local_path_length',
        default_value='70.0',
        description='Length of the scenario in meters')
    
    scenario_gen = Node(
        package='scenario_generator',
        executable='scenario_gen',
        name='scenario_gen',
        output='screen',
        parameters=[{
            'local_path_length': LaunchConfiguration('local_path_length')
        }]
    )

    return LaunchDescription([
        local_path_length_arg,
        scenario_gen
    ])