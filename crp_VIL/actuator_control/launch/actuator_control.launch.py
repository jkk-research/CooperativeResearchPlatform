from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from ament_index_python.packages import get_package_share_directory


def generate_launch_description():
    actuator_control = Node(
        namespace="lexus3",
        package='actuator_control',
        executable='actuator_control',
        output='screen'
    )

    return LaunchDescription([
        actuator_control
    ])
