from launch import LaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.actions import IncludeLaunchDescription, DeclareLaunchArgument, GroupAction
from launch.conditions import LaunchConfigurationEquals
from ament_index_python.packages import get_package_share_directory
from os.path import join


def generate_launch_description():
    # ARGUMENTS

    # vehicle control
    lat_accel_limit_arg = DeclareLaunchArgument(
        'lat_accel_lim',
        default_value='3.0',
        description='Lateral acceleration limit')
    jerk_limit_arg = DeclareLaunchArgument(
        'jerk_lim',
        default_value='0.5',
        description='Jerk limit')
    vehicle_param_L_arg = DeclareLaunchArgument(
        'vehicle_param_L',
        default_value='2.79',
        description='Vehicle parameter: ')

    ctrlUseCombinedControllerArg = DeclareLaunchArgument(
        'ctrlUseCombinedController',
        default_value='false',
        description='Whether to use combined controller (if set to false then separate lateral and longitudinal controllers will be used)'
    )

    ctrlCombinedMethodArg = DeclareLaunchArgument(
        'ctrlCombinedMethod',
        default_value='lqr',
        description='Lat controller to use. Possible values: lqr'
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


    ctrlLqrConfigFileArg = DeclareLaunchArgument(
        'ctrlLqrConfigFile',
        default_value=join(
            get_package_share_directory('crp_launcher'),
            'config',
            'control',
            'ctrlLqrParams.yaml'
        ),
        description='Path to lqr control configuration file'
    )
    ctrlTestConfigFileArg = DeclareLaunchArgument(
        'ctrlTestConfigFile',
        default_value=join(
            get_package_share_directory('crp_launcher'),
            'config',
            'control',
            'ctrlTestParams.yaml'
        ),
        description='Path to test control configuration file'
    )
    ctrlCompensatoryConfigFileArg = DeclareLaunchArgument(
        'ctrlCompensatoryConfigFile',
        default_value=join(
            get_package_share_directory('crp_launcher'),
            'config',
            'control',
            'ctrlCompensatoryParams.yaml'
        ),
        description='Path to compensatory control configuration file'
    )
    ctrlPurePConfigFileArg = DeclareLaunchArgument(
        'ctrlPurePConfigFile',
        default_value=join(
            get_package_share_directory('crp_launcher'),
            'config',
            'control',
            'ctrlPurePParams.yaml'
        ),
        description='Path to pure pursuit control configuration file'
    )
    ctrlStanleyConfigFileArg = DeclareLaunchArgument(
        'ctrlStanleyConfigFile',
        default_value=join(
            get_package_share_directory('crp_launcher'),
            'config',
            'control',
            'ctrlStanleyParams.yaml'
        ),
        description='Path to stanley control configuration file'
    )
    ctrlLongConfigFileArg = DeclareLaunchArgument(
        'ctrlLongConfigFile',
        default_value=join(
            get_package_share_directory('crp_launcher'),
            'config',
            'control',
            'ctrlLongParams.yaml'
        ),
        description='Path to long control configuration file'
    )
    
    # NODES

    environmental_fusion = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('prcp_situation_analysis'),
                'launch',
                'environmental_fusion.launch.py')
        )
    )

    ############
    # PLANNING #
    ############

    behavior_planning = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('plan_behavior_planning'),
                'launch',
                'behavior_planning.launch.py'
            )
        )
    )

    motion_planning = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('plan_motion_planning'),
                'launch',
                'motion_planning.launch.py'
            )
        )
    )

    planner_lat_lane_follow_ldm = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('plan_lat_lane_follow_ldm'),
                'launch',
                'plan_lat_lane_follow_ldm.launch.py'
            )
        )
    )

    planner_lon_intelligent_speed_adjust = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('plan_lon_intelligent_speed_adjust'),
                'launch',
                'plan_lon_intelligent_speed_adjust.launch.py'
            )
        )
    )

    ###########
    # CONTROL #
    ###########

    vehicle_control = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('ctrl_vehicle_control'),
                'launch',
                'ctrl_vehicle_control.launch.py')
        )
    )

    # combined controllers
    vehicle_control_lqr = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('ctrl_vehicle_control_lqr'),
                'launch',
                'ctrl_vehicle_control_lqr.launch.py')
        ),
        condition=LaunchConfigurationEquals('ctrlCombinedMethod', 'lqr')
    )

    vehicle_control_test = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('ctrl_vehicle_control_test'),
                'launch',
                'ctrl_vehicle_control_test.launch.py')
        ),
        condition=LaunchConfigurationEquals('ctrlCombinedMethod', 'test')
    )


    # lateral controllers
    vehicle_control_lat_compensatory = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('ctrl_vehicle_control_lat_compensatory'),
                'launch',
                'ctrl_vehicle_control_lat_compensatory.launch.py')
        ),
        condition=LaunchConfigurationEquals('ctrlLatMethod', 'comp')
    )
    vehicle_control_lat_pure_p = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('ctrl_vehicle_control_lat_pure_p'),
                'launch',
                'ctrl_vehicle_control_lat_pure_p.launch.py')
        ),
        condition=LaunchConfigurationEquals('ctrlLatMethod', 'purep')
    )
    vehicle_control_lat_stanley = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('ctrl_vehicle_control_lat_stanley'),
                'launch',
                'ctrl_vehicle_control_lat_stanley.launch.py')
        ),
        condition=LaunchConfigurationEquals('ctrlLatMethod', 'stanley')
    )

    # longitudinal controllers
    vehicle_control_long = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('ctrl_vehicle_control_long'),
                'launch',
                'ctrl_vehicle_control_long.launch.py')
        ),
        condition=LaunchConfigurationEquals('ctrlLongMethod', 'long')
    )


    return LaunchDescription([
        # arguments
        lat_accel_limit_arg,
        jerk_limit_arg,
        vehicle_param_L_arg,

        ctrlUseCombinedControllerArg,
        ctrlCombinedMethodArg,
        ctrlLatMethodArg,
        ctrlLongMethodArg,

        ctrlLqrConfigFileArg,
        ctrlTestConfigFileArg,
        ctrlCompensatoryConfigFileArg,
        ctrlPurePConfigFileArg,
        ctrlStanleyConfigFileArg,
        ctrlLongConfigFileArg,
        
        # nodes

        environmental_fusion,

        behavior_planning,
        planner_lat_lane_follow_ldm,
        planner_lon_intelligent_speed_adjust,
        motion_planning,

        vehicle_control,
        GroupAction(
            [
                vehicle_control_lqr,
                vehicle_control_test,
            ],
            condition=LaunchConfigurationEquals('ctrlUseCombinedController', 'true')
        ),
        GroupAction(
            [
                vehicle_control_lat_compensatory,
                vehicle_control_lat_pure_p,
                vehicle_control_lat_stanley,
            ],
            condition=LaunchConfigurationEquals('ctrlUseCombinedController', 'false')
        ),
        GroupAction(
            [
                vehicle_control_long
            ],
            condition=LaunchConfigurationEquals('ctrlUseCombinedController', 'false')
        ),
    ])
