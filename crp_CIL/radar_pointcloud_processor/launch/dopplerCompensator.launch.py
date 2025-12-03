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

    doppler_compensator_namespace_arg = DeclareLaunchArgument(
        'doppcomp/namespace',
        default_value='doppcomp',
        description='Namespace for the doppler compensator node.'
    )

    input_pcl_topic_arg = DeclareLaunchArgument(
        'doppcomp/input_pcl_topic',
        default_value='points',
        description='Input PointCloud2 topic for the doppler compensator.'
    )

    twist_topic_arg = DeclareLaunchArgument(
        'doppcomp/twist_topic',
        default_value='/sensing/vehicle/twist',
        description='Input TesitWithCovarianceStamped topic for the doppler compensator.'
    )

    output_pcl_topic_arg = DeclareLaunchArgument(
        'doppcomp/output_pcl_topic',
        default_value='compensated_points',
        description='Input PointCloud2 topic for the doppler compensator.'
    )

    override_ego_twist_frame_arg = DeclareLaunchArgument(
        'doppcomp/override_ego_twist_frame',
        default_value='',
        description='Override the frame of the radar for the transformation of the points. Leave empty to use the frameid defined in the input point cloud message.'
    )

    coppler_compensator_node = Node(
        package='radar_pointcloud_processor',
        executable='doppler_compensator',
        name='doppler_compensator',
        output='screen',
        parameters=[{
            'doppcomp/input_pcl_topic'         : LaunchConfiguration('doppcomp/input_pcl_topic'),
            'doppcomp/twist_topic'             : LaunchConfiguration('doppcomp/twist_topic'),
            'doppcomp/output_pcl_topic'        : LaunchConfiguration('doppcomp/output_pcl_topic'),
            'doppcomp/override_ego_twist_frame': LaunchConfiguration('doppcomp/override_ego_twist_frame')
        }]
    )


    return LaunchDescription([
        doppler_compensator_namespace_arg,
        input_pcl_topic_arg,
        twist_topic_arg,
        output_pcl_topic_arg,
        override_ego_twist_frame_arg,
        
        GroupAction(
            actions=[
                PushRosNamespace(LaunchConfiguration('doppcomp/namespace')),

                coppler_compensator_node
            ]
        )
    ])