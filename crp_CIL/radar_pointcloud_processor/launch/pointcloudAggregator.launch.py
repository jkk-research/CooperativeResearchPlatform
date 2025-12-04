from launch import LaunchDescription
from launch_ros.actions import Node, PushRosNamespace
from launch.actions import DeclareLaunchArgument, GroupAction
from launch.substitutions import LaunchConfiguration

from launch.launch_description_sources import AnyLaunchDescriptionSource
from launch.actions import IncludeLaunchDescription
from os.path import join
from ament_index_python.packages import get_package_share_directory


def generate_launch_description():
    
    # ARGS

    namespace_arg = DeclareLaunchArgument(
        'pcdagg/namespace',
        default_value='pcdagg',
        description='Namespace for the aggregator node.'
    )

    input_pcd_topic_arg = DeclareLaunchArgument(
        'pcdagg/input_pcd_topic',
        default_value='points',
        description='Input PointCloud2 topic for the aggregator.'
    )

    output_pcd_topic_arg = DeclareLaunchArgument(
        'pcdagg/output_pcd_topic',
        default_value='aggregated_points',
        description='Input PointCloud2 topic for the aggregator.'
    )

    pose_topic_arg = DeclareLaunchArgument(
        'pcdagg/localization_topic',
        default_value='/sensing/gnss/pose_with_covariance',
        description='Input localization (PoseWithCovarianceStamped) topic for the aggregator.'
    )

    aggregation_time_window_arg = DeclareLaunchArgument(
        'pcdagg/aggregation_time_window_sec',
        default_value='0.5',
        description='Time window for the aggregation.'
    )

    coppler_compensator_node = Node(
        package='radar_pointcloud_processor',
        executable='pointcloud_aggregator',
        name='pointcloud_aggregator',
        output='screen',
        parameters=[{
            'pcdagg/input_pcd_topic'            : LaunchConfiguration('pcdagg/input_pcd_topic'),
            'pcdagg/output_pcd_topic'           : LaunchConfiguration('pcdagg/output_pcd_topic'),
            'pcdagg/localization_topic'         : LaunchConfiguration('pcdagg/localization_topic'),
            'pcdagg/aggregation_time_window_sec': LaunchConfiguration('pcdagg/aggregation_time_window_sec'),
        }]
    )


    return LaunchDescription([
        namespace_arg,
        input_pcd_topic_arg,
        output_pcd_topic_arg,
        pose_topic_arg,
        aggregation_time_window_arg,
        
        GroupAction(
            actions=[
                PushRosNamespace(LaunchConfiguration('pcdagg/namespace')),

                coppler_compensator_node
            ]
        )
    ])