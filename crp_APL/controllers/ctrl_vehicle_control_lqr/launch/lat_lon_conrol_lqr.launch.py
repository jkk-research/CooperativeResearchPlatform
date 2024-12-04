from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='ctrl_vehicle_control_lqr',
            executable='lqr_controller.py',
            name='lqr_controller',
            output='screen',
            parameters=[{
                'dt': 0.03,
                'wheel_base': 1.9,
                'max_steer_tire_angle': 0.436332,
                'Q': [0.001, 0.0, 0.001, 0, 0.6], # Q matrix diagonal elements 
                'R': [30.0, 2.0], # R matrix diagonal elements
            }]
        )
    ])