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

    lanelet2_output_topic_arg = DeclareLaunchArgument(
        'map_output_topic',
        default_value="/map/global_static_map_from_file/lanelet2_map",
        description='Output topic for the lanelet2 map binary'
    )

    lanelet2_map_frame_id_arg = DeclareLaunchArgument(
        'map_frame_id',
        default_value="map",
        description='Frame id of the lanelet2 map'
    )

    # NODES
    lanelet_map_handler = Node(
        package='lanelet_handler',
        executable='lanelet_file_loader',
        name='lanelet_file_loader',
        output='screen',
        parameters=[{
            'map_file_path':    LaunchConfiguration('map_file_path'),
            'map_output_topic': LaunchConfiguration('map_output_topic'),
            'map_frame_id':     LaunchConfiguration('map_frame_id')
        }]
    )
    
    return LaunchDescription([
        lanelet2_path_arg,
        lanelet2_output_topic_arg,
        lanelet2_map_frame_id_arg,
        lanelet_map_handler
    ])