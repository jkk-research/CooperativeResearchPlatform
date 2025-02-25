from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from ament_index_python.packages import get_package_share_directory
from os.path import join


def generate_launch_description():
    # ARGUMENTS

    ctrlTestConfigFileArg = DeclareLaunchArgument(
        'ctrlTestConfigFile',
        default_value=join(
            get_package_share_directory('ctrl_vehicle_control_test'),
            'config',
            'ctrlTestParams.yaml'
        ),
        description='Path to the test control configuration file'
    )

    # NODES

    ctrl_vehicle_control_test = Node(
        package="ctrl_vehicle_control_test",
        executable="ctrl_vehicle_control_test",
        parameters=[
            LaunchConfiguration('ctrlTestConfigFile'),
        ]
    )

    return LaunchDescription([
        ctrlTestConfigFileArg,

        ctrl_vehicle_control_test
    ])