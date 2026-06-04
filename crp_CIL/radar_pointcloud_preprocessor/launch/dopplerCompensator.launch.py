from launch import LaunchDescription
from launch_ros.actions import Node, PushRosNamespace
from launch.actions import DeclareLaunchArgument, GroupAction
from launch.substitutions import LaunchConfiguration


def generate_launch_description():
    
    # ARGS

    doppler_compensator_namespace_arg = DeclareLaunchArgument(
        'doppcomp/namespace',
        default_value='doppcomp',
        description='Namespace for the doppler compensator node.'
    )

    input_pcd_topic_arg = DeclareLaunchArgument(
        'doppcomp/input_pcd_topic',
        default_value='points',
        description='Input PointCloud2 topic for the doppler compensator.'
    )

    output_pcd_topic_arg = DeclareLaunchArgument(
        'doppcomp/output_pcd_topic',
        default_value='dvcompensated_points',
        description='Input PointCloud2 topic for the doppler compensator.'
    )

    twist_topic_arg = DeclareLaunchArgument(
        'doppcomp/twist_topic',
        default_value='/sensing/vehicle/twist',
        description='Input TesitWithCovarianceStamped topic for the doppler compensator.'
    )

    override_ego_twist_frame_arg = DeclareLaunchArgument(
        'doppcomp/override_ego_twist_frame',
        default_value='',
        description='Override the frame of the radar for the transformation of the points. Leave empty to use the frameid defined in the input point cloud message.'
    )

    doppler_compensator_node = Node(
        package='radar_pointcloud_preprocessor',
        executable='doppler_compensator',
        name='doppler_compensator',
        output='screen',
        parameters=[{
            'doppcomp/input_pcd_topic'         : LaunchConfiguration('doppcomp/input_pcd_topic'),
            'doppcomp/output_pcd_topic'        : LaunchConfiguration('doppcomp/output_pcd_topic'),
            'doppcomp/twist_topic'             : LaunchConfiguration('doppcomp/twist_topic'),
            'doppcomp/override_ego_twist_frame': LaunchConfiguration('doppcomp/override_ego_twist_frame')
        }]
    )


    return LaunchDescription([
        doppler_compensator_namespace_arg,
        input_pcd_topic_arg,
        twist_topic_arg,
        output_pcd_topic_arg,
        override_ego_twist_frame_arg,
        
        GroupAction(
            actions=[
                PushRosNamespace(LaunchConfiguration('doppcomp/namespace')),

                doppler_compensator_node
            ]
        )
    ])