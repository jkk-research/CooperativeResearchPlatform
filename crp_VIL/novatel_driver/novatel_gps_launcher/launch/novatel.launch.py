from launch import LaunchDescription
from launch_ros.actions import ComposableNodeContainer
from launch_ros.descriptions import ComposableNode
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration

def generate_launch_description():
    # Arguments
    novatel_namespace_arg = DeclareLaunchArgument(
        'novatel_namespace',
        default_value='',
        description='Namespace for the Novatel GPS')

    novatel_ip_arg = DeclareLaunchArgument(
        'novatel_ip',
        default_value='192.168.1.11',
        description='IP address of the Novatel GPS')
    
    novatel_port_arg = DeclareLaunchArgument(
        'novatel_port',
        default_value='3002',
        description='Port of the Novatel GPS')
    
    novatel_imu_frame_id_arg = DeclareLaunchArgument(
        'novatel_imu_frame_id',
        default_value='/nissan9/nova/imu',
        description='IMU frame id of the Novatel GPS')
    
    novatel_frame_id_arg = DeclareLaunchArgument(
        'novatel_frame_id',
        default_value='/nissan9/nova/gps',
        description='Frame id of the Novatel GPS')
    
    # Nodes
    novatel_container = ComposableNodeContainer(
        name='novatel_gps_container',
        namespace='',
        package='rclcpp_components',
        executable='component_container',
        composable_node_descriptions=[
            ComposableNode(
                package='novatel_gps_driver',
                namespace=LaunchConfiguration("novatel_namespace"),
                plugin='novatel_gps_driver::NovatelGpsNode',
                name='novatel_gps',
                parameters=[{
                    'connection_type': 'tcp',
                    'device': [LaunchConfiguration("novatel_ip"), ":", LaunchConfiguration("novatel_port")],
                    'verbose': False,
                    'imu_sample_rate': -1.0,
                    'use_binary_messages': True,
                    'publish_novatel_positions': False,
                    'publish_imu_messages_': True,
                    'publish_novatel_utm_positions': True,
                    'publish_imu_messages': True,
                    'publish_novatel_velocity': False,
                    'publish_novatel_psrdop2': False,
                    'imu_frame_id': LaunchConfiguration("novatel_imu_frame_id"),
                    'frame_id': LaunchConfiguration("novatel_frame_id"),
                    'publish_novatel_dual_antenna_heading': True,
                    'x_coord_offset': 0.0,
                    'y_coord_offset': 0.0,
                    'z_coord_exact_height': 1.8,
                    'z_coord_ref_switch': "exact",
                    'tf_frame_id': "map",
                    'tf_child_frame_id': LaunchConfiguration("novatel_frame_id"),
                    'utm_frame_id': "map",
                }]
            )
        ],
        output='screen'
    )

    return LaunchDescription([
        novatel_namespace_arg,
        novatel_ip_arg,
        novatel_port_arg,
        novatel_imu_frame_id_arg,
        novatel_frame_id_arg,

        novatel_container
    ])
