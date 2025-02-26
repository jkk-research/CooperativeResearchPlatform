from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, DeclareLaunchArgument
from launch.launch_description_sources import PythonLaunchDescriptionSource
from ament_index_python.packages import get_package_share_directory
from os.path import join


def generate_launch_description():
    # ARGUMENTS

    ctrlUseCombinedControllerArg = DeclareLaunchArgument(
        'ctrlUseCombinedController',
        default_value='false',
        description='Whether to use combined controller (if set to false then separate lateral and longitudinal controllers will be used)'
    )
    ctrlCombinedMethodArg = DeclareLaunchArgument(
        'ctrlCombinedMethod',
        default_value='lqr',
        description='Lat controller to use. Possible values: lqr, test'
    )
    ctrlLatMethodArg = DeclareLaunchArgument(
        'ctrlLatMethod',
        default_value='comp',
        description='Lat controller to use. Possible values: comp, purep, stanley'
    )
    ctrlLongMethodArg = DeclareLaunchArgument(
        'ctrlLongMethod',
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

        ctrlUseCombinedControllerArg,
        ctrlCombinedMethodArg,
        ctrlLatMethodArg,
        ctrlLongMethodArg,

        # nodes

        test_node,
        crp_core,
    ])
