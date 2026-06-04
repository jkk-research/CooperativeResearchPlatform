from launch import LaunchDescription
from launch_ros.actions import Node, ComposableNodeContainer
from launch_ros.descriptions import ComposableNode
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration

def generate_launch_description():
    # Nodes
    
    mpc_25_wrapper = Node(
        package='mpc_25_wrapper',
        executable='mpc_25_wrapper',
        name='mpc_25_wrapper',
        output='screen'
    )

    return LaunchDescription([
        mpc_25_wrapper
    ])
