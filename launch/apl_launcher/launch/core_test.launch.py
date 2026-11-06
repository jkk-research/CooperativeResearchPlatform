from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, DeclareLaunchArgument
from launch.launch_description_sources import PythonLaunchDescriptionSource
from ament_index_python.packages import get_package_share_directory
from os.path import join


def generate_launch_description():
    # ARGUMENTS

    ctrl_use_combined_controllerArg = DeclareLaunchArgument(
        'ctrl_use_combined_controller',
        default_value='false',
        description='Whether to use combined controller (if set to false then separate lateral and longitudinal controllers will be used)'
    )

    ctrl_combined_methodArg = DeclareLaunchArgument(
        'ctrl_combined_method',
        default_value='lqr',
        description='Lat controller to use. Possible values: lqr'
    )
    ctrl_lat_methodArg = DeclareLaunchArgument(
        'ctrl_lat_method',
        default_value='nmpc',
        description='Lat controller to use. Possible values: comp, purep, stanley, nmpc'
    )
    ctrl_long_methodArg = DeclareLaunchArgument(
        'ctrl_long_method',
        default_value='long',
        description='Controller to use. Possible values: long'
    )

    # CORE

    crp_core = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('crp_launcher'),
                'launch',
                'core.launch.py')
        )
    )

    # TEST NODE

    test_node = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('test_node'),
                'launch',
                'test_node.launch.py')
        )
    )

    return LaunchDescription([
        # arguments

        ctrl_use_combined_controllerArg,
        ctrl_combined_methodArg,
        ctrl_lat_methodArg,
        ctrl_long_methodArg,

        # nodes

        test_node,
        crp_core,
    ])
