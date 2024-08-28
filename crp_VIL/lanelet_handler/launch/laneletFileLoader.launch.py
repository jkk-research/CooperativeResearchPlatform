from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration


def generate_launch_description():
    lanelet2_path_arg = DeclareLaunchArgument(
        'map_file_path',
        default_value="",
        description='Path to the lanelet2 map file'
    )

    # NODES
    lanelet_map_handler = Node(
        package='lanelet_handler',
        executable='lanelet_file_loader',
        name='lanelet_file_loader',
        output='screen',
        parameters=[
            {'map_file_path':  LaunchConfiguration('map_file_path')},
        ]
    )
    
    return LaunchDescription([
        lanelet2_path_arg,
        lanelet_map_handler
    ])