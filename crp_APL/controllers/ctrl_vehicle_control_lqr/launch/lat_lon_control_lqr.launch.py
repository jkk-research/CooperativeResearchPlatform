from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    ctrl_vehicle_control_lat_lqr = Node(
            package='ctrl_vehicle_control_lqr',
            executable='lqr_controller.py',
            name='lqr_controller',
            output='screen',
            parameters=[{
                'dt': 0.0333,
                'wheel_base': 2.79,
                'max_steer_tire_angle': 0.436332,
                'Q': [0.2, 0.0, 0.2, 0, 10.5], # Q matrix diagonal elements 
                'R': [10.0, 1.0], # R matrix diagonal elements
            }]
        )
    
    return LaunchDescription([
        ctrl_vehicle_control_lat_lqr
    ])