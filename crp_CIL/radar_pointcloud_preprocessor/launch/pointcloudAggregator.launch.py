from launch import LaunchDescription
from launch_ros.actions import Node, PushRosNamespace
from launch.actions import DeclareLaunchArgument, GroupAction
from launch.substitutions import LaunchConfiguration


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
        description='Output PointCloud2 topic for the aggregator.'
    )

    twist_topic_arg = DeclareLaunchArgument(
        'pcdagg/twist_topic',
        default_value='/sensing/vehicle/twist',
        description='Input ego twist (TwistWithCovarianceStamped) topic for the aggregator.'
    )

    aggregation_time_window_arg = DeclareLaunchArgument(
        'pcdagg/aggregation_time_window_sec',
        default_value='0.5',
        description='Time window for the aggregation.'
    )

    override_ego_twist_frame_arg = DeclareLaunchArgument(
        'pcdagg/override_ego_twist_frame',
        default_value='',
        description='Override the frame of the radar for the transformation of the points. Leave empty to use the frameid defined in the input point cloud message.'
    )

    pointcloud_aggregator_node = Node(
        package='radar_pointcloud_preprocessor',
        executable='pointcloud_aggregator',
        name='pointcloud_aggregator',
        output='screen',
        parameters=[{
            'pcdagg/input_pcd_topic'            : LaunchConfiguration('pcdagg/input_pcd_topic'),
            'pcdagg/output_pcd_topic'           : LaunchConfiguration('pcdagg/output_pcd_topic'),
            'pcdagg/twist_topic'                : LaunchConfiguration('pcdagg/twist_topic'),
            'pcdagg/aggregation_time_window_sec': LaunchConfiguration('pcdagg/aggregation_time_window_sec'),
            'pcdagg/override_ego_twist_frame'   : LaunchConfiguration('pcdagg/override_ego_twist_frame')
        }]
    )


    return LaunchDescription([
        namespace_arg,
        input_pcd_topic_arg,
        output_pcd_topic_arg,
        twist_topic_arg,
        aggregation_time_window_arg,
        override_ego_twist_frame_arg,
        
        GroupAction(
            actions=[
                PushRosNamespace(LaunchConfiguration('pcdagg/namespace')),

                pointcloud_aggregator_node
            ]
        )
    ])