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
        default_value='0.5f',
        description='Jerk limit')

    vehicle_param_L_arg = DeclareLaunchArgument(
        '/vehicle_params/L',
        default_value='2.79',
        description='Vehicle parameter: Wheelbase [m]')
    vehicle_param_c1_arg = DeclareLaunchArgument(
        '/vehicle_params/c1',
        default_value='3000.0',
        description='Vehicle parameter: Front wheel cornering stiffness (for single track model) [N/rad]')
    vehicle_param_c2_arg = DeclareLaunchArgument(
        '/vehicle_params/c2',
        default_value='3000.0',
        description='Vehicle parameter: Rear wheel cornering stiffness (for single track model) [N/rad]')
    vehicle_param_m_arg = DeclareLaunchArgument(
        '/vehicle_params/m',
        default_value='180.0',
        description='Vehicle parameter: Wheelbase [m]')
    vehicle_param_jz_arg = DeclareLaunchArgument(
        '/vehicle_params/jz',
        default_value='270.0',
        description='Vehicle parameter: Moment of inertia (z axle) [kg*m2]')
    vehicle_param_l1_arg = DeclareLaunchArgument(
        '/vehicle_params/l1',
        default_value='0.624',
        description='Vehicle parameter: CoG distance from the front axle [m]' )
    vehicle_param_l2_arg = DeclareLaunchArgument(
        '/vehicle_params/l2',
        default_value='0.676',
        description='Vehicle parameter: CoG distance from the rear axle [m]')
    vehicle_param_swr_arg = DeclareLaunchArgument(
        '/vehicle_params/swr',
        default_value='1.0',
        description='Vehicle parameter: Steering wheel ratio')

    ctrl_use_combined_controller_arg = DeclareLaunchArgument(
        'ctrl_use_combined_controller',
        default_value='false',
        description='Whether to use combined controller (if set to false then separate lateral and longitudinal controllers will be used)'
    )

    ctrl_combined_method_arg = DeclareLaunchArgument(
        'ctrl_combined_method',
        default_value='lqr',
        description='Lat controller to use. Possible values: lqr'
    )
    ctrl_lat_method_arg = DeclareLaunchArgument(
        'ctrl_lat_method',
        default_value='comp',
        description='Lat controller to use. Possible values: comp, purep, stanley'
    )
    ctrl_long_method_arg = DeclareLaunchArgument(
        'ctrl_long_method',
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
        condition=LaunchConfigurationEquals('ctrl_combined_method', 'lqr')
    )

    # lateral controllers
    vehicle_control_lat_compensatory = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('ctrl_vehicle_control_lat_compensatory'),
                'launch',
                'ctrl_vehicle_control_lat_compensatory.launch.py')
        ),
        condition=LaunchConfigurationEquals('ctrl_lat_method', 'comp')
    )
    vehicle_control_lat_pure_p = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('ctrl_vehicle_control_lat_pure_p'),
                'launch',
                'ctrl_vehicle_control_lat_pure_p.launch.py')
        ),
        condition=LaunchConfigurationEquals('ctrl_lat_method', 'purep')
    )
    vehicle_control_lat_stanley = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('ctrl_vehicle_control_lat_stanley'),
                'launch',
                'ctrl_vehicle_control_lat_stanley.launch.py')
        ),
        condition=LaunchConfigurationEquals('ctrl_lat_method', 'stanley')
    )

    # longitudinal controllers
    vehicle_control_long = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('ctrl_vehicle_control_long'),
                'launch',
                'ctrl_vehicle_control_long.launch.py')
        ),
        condition=LaunchConfigurationEquals('ctrl_long_method', 'long')
    )


    return LaunchDescription([
        # arguments
        lat_accel_limit_arg,
        jerk_limit_arg,
        vehicle_param_L_arg,
        vehicle_param_c1_arg,
        vehicle_param_c2_arg,
        vehicle_param_m_arg,
        vehicle_param_jz_arg,
        vehicle_param_l1_arg,
        vehicle_param_l2_arg,
        vehicle_param_swr_arg,

        ctrl_use_combined_controller_arg,
        ctrl_combined_method_arg,
        ctrl_lat_method_arg,
        ctrl_long_method_arg,

        ctrlLqrConfigFileArg,
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
            ],
            condition=LaunchConfigurationEquals('ctrl_use_combined_controller', 'true')
        ),
        GroupAction(
            [
                vehicle_control_lat_compensatory,
                vehicle_control_lat_pure_p,
                vehicle_control_lat_stanley,
            ],
            condition=LaunchConfigurationEquals('ctrl_use_combined_controller', 'false')
        ),
        GroupAction(
            [
                vehicle_control_long
            ],
            condition=LaunchConfigurationEquals('ctrl_use_combined_controller', 'false')
        ),
    ])
