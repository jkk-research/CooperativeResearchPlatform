from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.actions import DeclareLaunchArgument, ExecuteProcess
from launch.substitutions import LaunchConfiguration
from launch.conditions import LaunchConfigurationEquals
from launch.launch_description_sources import PythonLaunchDescriptionSource, AnyLaunchDescriptionSource
from ament_index_python.packages import get_package_share_directory
from os.path import join


def generate_launch_description():

    # ARGUMENTS

    change_controller_PGain = ExecuteProcess(
        cmd=[[
            'ros2 param set ',
            '/CtrlVehicleControlLat ',
            '/ctrl/fbPGain ',
            '2.0'
        ]],
        shell=True
    )

    change_controller_IGain = ExecuteProcess(
        cmd=[[
            'ros2 param set ',
            '/CtrlVehicleControlLat ',
            '/ctrl/fbIGain ',
            '0.05'
        ]],
        shell=True
    )
    
    change_controller_DGain = ExecuteProcess(
        cmd=[[
            'ros2 param set ',
            '/CtrlVehicleControlLat ',
            '/ctrl/fbDGain ',
            '0.1'
        ]],
        shell=True
    )
    
    change_controller_ThetaGain = ExecuteProcess(
        cmd=[[
            'ros2 param set ',
            '/CtrlVehicleControlLat ',
            '/ctrl/fbThetaGain ',
            '0.0'
        ]],
        shell=True
    )
    
    change_controller_fbLookAheadTime = ExecuteProcess(
        cmd=[[
            'ros2 param set ',
            '/CtrlVehicleControlLat ',
            '/ctrl/fbLookAheadTime ',
            '0.1'
        ]],
        shell=True
    )

    change_controller_steeringLpFilter = ExecuteProcess(
        cmd=[[
            'ros2 param set ',
            '/CtrlVehicleControlLat ',
            '/ctrl/steeringAngleLPFilter ',
            '0.8'
        ]],
        shell=True
    )
    
    change_controller_ffGainOffsetGround = ExecuteProcess(
        cmd=[[
            'ros2 param set ',
            '/CtrlVehicleControlLat ',
            '/ctrl/ffGainOffsetGround ',
            '0.0'
        ]],
        shell=True
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

        # nodes
        test_node,
        crp_core,

        # args
        change_controller_PGain,
        change_controller_IGain,
        change_controller_DGain,
        change_controller_ThetaGain,
        change_controller_steeringLpFilter,
        change_controller_ffGainOffsetGround,
        change_controller_fbLookAheadTime
    ])
