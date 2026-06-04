from launch import LaunchDescription
from launch_ros.actions import Node, ComposableNodeContainer
from launch_ros.descriptions import ComposableNode
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration

def generate_launch_description():
    # Arguments
    novatel_namespace_arg = DeclareLaunchArgument(
        'novatel_namespace',
        default_value='lexus3/gps/nova',
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
    
    novatel_topic_converter = Node(
        package='novatel_gps_wrapper',
        executable='novatel_topic_converter',
        name='novatel_topic_converter',
        namespace=LaunchConfiguration("novatel_namespace"),
        output='screen'
    )

    return LaunchDescription([
        novatel_namespace_arg,
        novatel_ip_arg,
        novatel_port_arg,
        novatel_imu_frame_id_arg,
        novatel_frame_id_arg,

        novatel_topic_converter
    ])
