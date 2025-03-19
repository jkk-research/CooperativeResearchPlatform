from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from ament_index_python.packages import get_package_share_directory
from os.path import join


def generate_launch_description():
    # ARGUMENTS

    ctrlStanleyConfigFileArg = DeclareLaunchArgument(
        'ctrlStanleyConfigFile',
        default_value=join(
            get_package_share_directory('ctrl_vehicle_control_lat_stanley'),
            'config',
            'ctrlStanleyParams.yaml'
        ),
        description='Path to the stanley control configuration file'
    )

    vehicle_param_L_arg = DeclareLaunchArgument(
        '/vehicle_params/wheelbase',
        default_value='2.79',
        description='Vehicle parameter: Wheelbase [m]')

    # NODES

    ctrl_vehicle_control_lat_stanley = Node(
        package="ctrl_vehicle_control_lat_stanley",
        executable="ctrl_vehicle_control_lat_stanley",
        parameters=[
            LaunchConfiguration('ctrlStanleyConfigFile'),
            {
                '/vehicle_params/wheelbase': LaunchConfiguration('/vehicle_params/wheelbase')
            }
        ]
    )

    return LaunchDescription([
        ctrlStanleyConfigFileArg,
        vehicle_param_L_arg,

        ctrl_vehicle_control_lat_stanley
    ])
