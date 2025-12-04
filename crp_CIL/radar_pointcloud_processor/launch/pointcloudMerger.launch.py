from launch import LaunchDescription
from launch_ros.actions import Node, PushRosNamespace
from launch.actions import DeclareLaunchArgument, GroupAction
from launch.substitutions import LaunchConfiguration


def generate_launch_description():
    
    # ARGS

    namespace_arg = DeclareLaunchArgument(
        'pcdmerger/namespace',
        default_value='pcdmerger',
        description='Namespace for the merger node.'
    )

    in_pcd_topics_arg = DeclareLaunchArgument(
        'pcdmerger/in_pcd_topics',
        default_value='[points1, points2]',
        description='PointCloud2 topics to be merged. The first one will be the trigger for merging.'
    )

    out_topic_arg = DeclareLaunchArgument(
        'pcdmerger/out_topic',
        default_value='merged_points',
        description='Output PointCloud2 topic for the merger.'
    )

    ego_frame_arg = DeclareLaunchArgument(
        'pcdmerger/ego_frame',
        default_value='base_link',
        description='Input localization (PoseWithCovarianceStamped) topic for the aggregator.'
    )

    merge_trigger_timeout_arg = DeclareLaunchArgument(
        'pcdmerger/merge_trigger_timeout_sec',
        default_value='0.1',
        description='Timeout for the trigger topic. The trigger is changed if the current trigger times out.'
    )

    pointcloud_merger_node = Node(
        package='radar_pointcloud_processor',
        executable='pointcloud_merger',
        name='pointcloud_merger',
        output='screen',
        parameters=[{
            'pcdmerger/in_pcd_topics'            : LaunchConfiguration('pcdmerger/in_pcd_topics'),
            'pcdmerger/out_topic'                : LaunchConfiguration('pcdmerger/out_topic'),
            'pcdmerger/ego_frame'                : LaunchConfiguration('pcdmerger/ego_frame'),
            'pcdmerger/merge_trigger_timeout_sec': LaunchConfiguration('pcdmerger/merge_trigger_timeout_sec'),
        }]
    )


    return LaunchDescription([
        namespace_arg,
        in_pcd_topics_arg,
        out_topic_arg,
        ego_frame_arg,
        merge_trigger_timeout_arg,
        
        GroupAction(
            actions=[
                PushRosNamespace(LaunchConfiguration('pcdmerger/namespace')),

                pointcloud_merger_node
            ]
        )
    ])