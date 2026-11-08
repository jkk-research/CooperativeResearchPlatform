from launch import LaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.actions import IncludeLaunchDescription, DeclareLaunchArgument, GroupAction
from launch.conditions import LaunchConfigurationEquals
from ament_index_python.packages import get_package_share_directory
from os.path import join


def generate_launch_description():
    # ARGUMENTS
    
    # NODES

    prcp_late_fusion = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('prcp_late_fusion'),
                'launch',
                'prcp_late_fusion.launch.py')
        )
    )

    prcp_scenario_abstraction = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('prcp_sensor_abstraction'),
                'launch',
                'sensor_abstraction.launch.py')
        )
    )

    return LaunchDescription([
     
        # nodes

        prcp_scenario_abstraction,
        prcp_late_fusion,        
        
    ])
