from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from ament_index_python.packages import get_package_share_directory
from os.path import join


def generate_launch_description():
    # ARGUMENTS

    ctrlLongConfigFileArg = DeclareLaunchArgument(
        'ctrlLongConfigFile',
        default_value=join(
            get_package_share_directory('ctrl_vehicle_control_long'),
            'config',
            'ctrlLongParams.yaml'
        ),
        description='Path to the long control configuration file'
    )

    # NODES

    ctrl_vehicle_control_long = Node(
        package="ctrl_vehicle_control_long",
        executable="ctrl_vehicle_control_long",
        parameters=[
            LaunchConfiguration('ctrlLongConfigFile'),
        ]
    )

    return LaunchDescription([
        ctrlLongConfigFileArg,
        
        ctrl_vehicle_control_long
    ])
