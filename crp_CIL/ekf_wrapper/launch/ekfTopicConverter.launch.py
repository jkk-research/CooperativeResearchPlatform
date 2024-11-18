from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration

def generate_launch_description():
    ekf_frame_arg = DeclareLaunchArgument(
        'ekf_frame',
        default_value='map',
        description='Frame of the EKF pose')
    
    ekf_topic_converter_node = Node(
        package='ekf_wrapper',
        executable='ekf_topic_converter',
        name='ekf_topic_converter',
        output='screen',
        parameters=[{
            'ekf_frame': LaunchConfiguration('ekf_frame')
        }]
    )

    return LaunchDescription([
        ekf_frame_arg,
        
        ekf_topic_converter_node
    ])
