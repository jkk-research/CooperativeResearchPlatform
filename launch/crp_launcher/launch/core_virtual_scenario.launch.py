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
        '/vehicle_params/wheelbase',
        default_value='2.79',
        description='Vehicle parameter: Wheelbase [m]')
    vehicle_param_c1_arg = DeclareLaunchArgument(
        '/vehicle_params/front_whl_corn_stiff',
        default_value='3000.0',
        description='Vehicle parameter: Front wheel cornering stiffness (for single track model) [N/rad]')
    vehicle_param_c2_arg = DeclareLaunchArgument(
        '/vehicle_params/rear_whl_corn_stiff',
        default_value='3000.0',
        description='Vehicle parameter: Rear wheel cornering stiffness (for single track model) [N/rad]')
    vehicle_param_m_arg = DeclareLaunchArgument(
        '/vehicle_params/mass',
        default_value='180.0',
        description='Vehicle parameter: Wheelbase [m]')
    vehicle_param_jz_arg = DeclareLaunchArgument(
        '/vehicle_params/inertia_z',
        default_value='270.0',
        description='Vehicle parameter: Moment of inertia (z axle) [kg*m2]')
    vehicle_param_l1_arg = DeclareLaunchArgument(
        '/vehicle_params/front_axle_from_cog',
        default_value='0.624',
        description='Vehicle parameter: CoG distance from the front axle [m]' )
    vehicle_param_l2_arg = DeclareLaunchArgument(
        '/vehicle_params/rear_axle_from_cog',
        default_value='0.676',
        description='Vehicle parameter: CoG distance from the rear axle [m]')
    vehicle_param_swr_arg = DeclareLaunchArgument(
        '/vehicle_params/steering_ratio',
        default_value='1.0',
        description='Vehicle parameter: Steering wheel ratio')
    
    # NODES

    scenario_generator = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('prcp_scenario_generator'),
                'launch',
                'scenario_generator.launch.py')
        )
    )

    ############
    # PLANNING #
    ############

    # RUNS IN EXTERNAL COMPONENTS

    ###########
    # CONTROL #
    ###########

    # RUNS IN EXTERNAL COMPONENTS


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
        
        # nodes
        scenario_generator        
    ])
