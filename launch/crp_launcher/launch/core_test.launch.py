from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, DeclareLaunchArgument
from launch.launch_description_sources import PythonLaunchDescriptionSource
from ament_index_python.packages import get_package_share_directory
from os.path import join


def generate_launch_description():
    # ARGUMENTS

    ctrlCompensatoryConfigFileArg = DeclareLaunchArgument(
        'ctrlCompensatoryConfigFile',
        default_value=join(
            get_package_share_directory('crp_launcher'),
            'config',
            'control',
            'compensatoryParams.yaml'
        ),
        description='Path to compensatory control configuration file'
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
        ctrlCompensatoryConfigFileArg,

        # nodes
        test_node,
        crp_core,
    ])
