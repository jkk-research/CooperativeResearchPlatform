from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch.launch_description_sources import PythonLaunchDescriptionSource
from ament_index_python.packages import get_package_share_directory
from os.path import join

def generate_launch_description():
    # ARGS

    # centerpoint radar curve detector (docker)
    centerpoint_image_arg = DeclareLaunchArgument(
        'centerpoint/image',
        default_value='ghcr.io/jkk-research/lidar_centerpoint_detector:latest',
        description='Docker image of the centerpoint detector.')
    centerpoint_model_arg = DeclareLaunchArgument(
        'centerpoint/model',
        default_value='v3_distnms',
        description='Bundled model preset: [v3_distnms, v4_hungarian, v3_curvy]. v3_curvy does not need the delta_t field.')
    centerpoint_radar_input_topic_arg = DeclareLaunchArgument(
        'centerpoint/radar_input_topic',
        default_value='/sensing/radar/fc/aggregated_points',
        description='Aggregated, doppler compensated PointCloud2 topic (needs x, y, z, rcs, dv_comp and delta_t fields).')
    centerpoint_model_frame_arg = DeclareLaunchArgument(
        'centerpoint/model_frame',
        default_value='lexus3/base_link',
        description='Coordinate frame used during training.')
    centerpoint_tracking_enabled_arg = DeclareLaunchArgument(
        'centerpoint/tracking_enabled',
        default_value='true',
        description='Enable the static curve tracker.')
    centerpoint_tracking_frame_arg = DeclareLaunchArgument(
        'centerpoint/tracking_frame',
        default_value='map',
        description='Fixed frame used for tracking.')

    # LEXUS

    lexus = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('crp_launcher'),
                'launch',
                'lexus.launch.py')
        )
    )

    # NODES

    centerpoint_detector = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('lidar_centerpoint_detector'),
                'docker_run.launch.py')
        ),
        launch_arguments={
            'image'            : LaunchConfiguration('centerpoint/image'),
            'model'            : LaunchConfiguration('centerpoint/model'),
            'radar_input_topic': LaunchConfiguration('centerpoint/radar_input_topic'),
            'model_frame'      : LaunchConfiguration('centerpoint/model_frame'),
            'tracking_enabled' : LaunchConfiguration('centerpoint/tracking_enabled'),
            'tracking_frame'   : LaunchConfiguration('centerpoint/tracking_frame'),
        }.items()
    )


    return LaunchDescription([
        # args
        centerpoint_image_arg,
        centerpoint_model_arg,
        centerpoint_radar_input_topic_arg,
        centerpoint_model_frame_arg,
        centerpoint_tracking_enabled_arg,
        centerpoint_tracking_frame_arg,

        # lexus
        lexus,

        # perception nodes
        centerpoint_detector,
    ])
