from launch_ros.actions import Node
from launch import LaunchDescription

def generate_launch_description():
    pacmod_extender = Node(
            namespace='lexus3',
            package='pacmod_extender',
            executable='pacmod_extender_node',
            name='pacmod_extender',
            output='screen'
        )
    
    return LaunchDescription([
        pacmod_extender
    ])