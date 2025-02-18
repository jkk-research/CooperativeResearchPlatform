from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from ament_index_python.packages import get_package_share_directory
from os.path import join


def generate_launch_description():
    # ARGUMENTS

    ctrlCompensatoryConfigFileArg = DeclareLaunchArgument(
        'ctrlCompensatoryConfigFile',
        default_value=join(
            get_package_share_directory('ctrl_vehicle_control_lat_compensatory'),
            'config',
            'ctrlCompensatoryParams.yaml'
        ),
        description='Path to the compensatory control configuration file'
    )

    # NODES

    ctrl_vehicle_control_lat_compensatory = Node(
        package="ctrl_vehicle_control_lat_compensatory",
        executable="ctrl_vehicle_control_lat_compensatory",
        parameters=[
            LaunchConfiguration('ctrlCompensatoryConfigFile'),
        ]
    )

    return LaunchDescription([
        ctrlCompensatoryConfigFileArg,

        ctrl_vehicle_control_lat_compensatory
    ])