from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration


def generate_launch_description():
    lanelet2_path_arg = DeclareLaunchArgument(
        'map_file_path',
        default_value="/home/matyko/Downloads/ZalaZone_Handling.osm",
        description='Path to the lanelet2 map file'
    )

    lanelet2_output_topic_arg = DeclareLaunchArgument(
        'map_output_topic',
        default_value="/map/global_static_map_from_file/lanelet2_map",
        description='Output topic for the lanelet2 map binary'
    )

    lanelet2_visualization_topic_arg = DeclareLaunchArgument(
        'map_visualization_topic',
        default_value="/map/global_static_map_from_file/lanelet2_map_visualization",
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

    lanelet_map_visualizer = Node(
        package='map_loader',
        executable='lanelet2_map_visualization',
        name='lanelet2_map_visualization',
        output='screen',
        remappings=[
            ('input/lanelet2_map', LaunchConfiguration('map_output_topic')),
            ('output/lanelet2_map_marker', LaunchConfiguration('map_visualization_topic'))
        ]
    )
    
    return LaunchDescription([
        lanelet2_path_arg,
        lanelet2_output_topic_arg,
        lanelet2_visualization_topic_arg,
        lanelet2_map_frame_id_arg,
        lanelet_map_handler,
        lanelet_map_visualizer
    ])