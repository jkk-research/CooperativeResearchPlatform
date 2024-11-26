from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='lqr_on_rails',
            executable='lqr_controller.py',
            name='lqr_controller',
            output='screen',
            parameters=[{
                'dt': 0.03,
                'wheel_base': 1.9,
                'max_steer_tire_angle': 0.436332
            }]
        )
    ])