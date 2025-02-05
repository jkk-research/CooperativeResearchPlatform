from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration

def generate_launch_description():
    # args
    lat_accel_limit_arg = DeclareLaunchArgument(
        'lat_accel_lim',
        default_value='3.0',
        description='Lateral acceleration limit')
    jerk_limit_arg = DeclareLaunchArgument(
        'jerk_lim',
        default_value='0.5',
        description='Jerk limit')
    vehicle_param_L_arg = DeclareLaunchArgument(
        'vehicle_param_L',
        default_value='2.79',
        description='Vehicle parameter: ')
    
    # nodes
    vehicle_control = Node(
        package="ctrl_vehicle_control",
        executable="ctrl_vehicle_control",
        name="ctrl_vehicle_control",
        parameters=[{
            "lat_accel_lim": LaunchConfiguration('lat_accel_lim'),
            "jerk_lim": LaunchConfiguration('jerk_lim'),
            "vehicle_param_L": LaunchConfiguration('vehicle_param_L')
        }]
    )

    return LaunchDescription([
        lat_accel_limit_arg,
        jerk_limit_arg,
        vehicle_param_L_arg,

        vehicle_control
    ])

    return ld