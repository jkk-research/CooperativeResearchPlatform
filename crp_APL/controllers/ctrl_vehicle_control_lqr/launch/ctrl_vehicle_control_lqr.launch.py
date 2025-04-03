from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from ament_index_python.packages import get_package_share_directory
from os.path import join

def generate_launch_description():
    # ARGUMENTS

    ctrlLqrConfigFileArg = DeclareLaunchArgument(
        'ctrlLqrConfigFile',
        default_value=join(
            get_package_share_directory('ctrl_vehicle_control_lqr'),
            'config',
            'ctrlLqrParams.yaml'
        ),
        description='Path to the lqr control configuration file'
    )

    vehicle_param_L_arg = DeclareLaunchArgument(
        '/vehicle_params/wheelbase',
        default_value='2.79',
        description='Vehicle parameter: Wheelbase [m]')

    # NODES

    ctrl_vehicle_control_lat_lqr = Node(
            package='ctrl_vehicle_control_lqr',
            executable='lqr_controller.py',
            name='ctrl_vehicle_control_lqr',
            output='screen',
            parameters=[
                LaunchConfiguration('ctrlLqrConfigFile'),
                {
                    '/vehicle_params/wheelbase': LaunchConfiguration('/vehicle_params/wheelbase')
                }
            ]
        )
    
    return LaunchDescription([
        ctrlLqrConfigFileArg,
        vehicle_param_L_arg,

        ctrl_vehicle_control_lat_lqr
    ])