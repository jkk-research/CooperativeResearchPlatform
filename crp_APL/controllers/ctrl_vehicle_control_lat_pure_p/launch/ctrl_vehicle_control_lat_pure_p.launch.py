from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from ament_index_python.packages import get_package_share_directory
from os.path import join


def generate_launch_description():
    # ARGUMENTS

    ctrlPurePConfigFileArg = DeclareLaunchArgument(
        'ctrlPurePConfigFile',
        default_value=join(
            get_package_share_directory('ctrl_vehicle_control_lat_pure_p'),
            'config',
            'ctrlPurePParams.yaml'
        ),
        description='Path to the pure pursuit control configuration file'
    )

    vehicle_param_L_arg = DeclareLaunchArgument(
        '/vehicle_params/wheelbase',
        default_value='2.79',
        description='Vehicle parameter: Wheelbase [m]')
    
    # NODES

    ctrl_vehicle_control_lat_pure_p = Node(
        package="ctrl_vehicle_control_lat_pure_p",
        executable="ctrl_vehicle_control_lat_pure_p",
        parameters=[
            LaunchConfiguration('ctrlPurePConfigFile'),
            {
                '/vehicle_params/wheelbase': LaunchConfiguration('/vehicle_params/wheelbase')
            }
        ]
    )

    return LaunchDescription([
        ctrlPurePConfigFileArg,
        vehicle_param_L_arg,

        ctrl_vehicle_control_lat_pure_p
    ])