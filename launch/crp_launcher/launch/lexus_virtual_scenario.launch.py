from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, DeclareLaunchArgument, SetLaunchConfiguration
from launch.conditions import LaunchConfigurationEquals
from launch.substitutions import LaunchConfiguration, PythonExpression
from launch.launch_description_sources import PythonLaunchDescriptionSource, AnyLaunchDescriptionSource
from ament_index_python.packages import get_package_share_directory
from os.path import join
import yaml

def load_yaml(file_path):
    with open(file_path, 'r') as file:
        return yaml.safe_load(file)

def generate_launch_description():
    # ARGS

    # sensor abstraction
    vehicle_tire_angle_topic_arg = DeclareLaunchArgument(
        'vehicle_tire_angle_topic',
        default_value='/sensing/vehicle/tire_angle',
        description='Length of the scenario in meters')
    
    # vehicle parameters
    vehicle_params = load_yaml(join(
        get_package_share_directory('crp_launcher'),
        'config',
        'vehicle',
        'lexusParams.yaml'
    ))

    vehicle_param_L_arg = DeclareLaunchArgument(
        '/vehicle_params/wheelbase',
        default_value=str(vehicle_params['/vehicle_params/wheelbase']),
        description='Vehicle parameter: Wheelbase [m]')
    vehicle_param_m_arg = DeclareLaunchArgument(
        '/vehicle_params/mass',
        default_value=str(vehicle_params['/vehicle_params/mass']),
        description='Vehicle parameter: Mass of the vehicle [kg]')
    vehicle_param_jz_arg = DeclareLaunchArgument(
        '/vehicle_params/inertia_z',
        default_value=str(vehicle_params['/vehicle_params/inertia_z']),
        description='Vehicle parameter: Moment of inertia (z axle) [kg*m2]')
    vehicle_param_l1_arg = DeclareLaunchArgument(
        '/vehicle_params/front_axle_from_cog',
        default_value=str(vehicle_params['/vehicle_params/front_axle_from_cog']),
        description='Vehicle parameter: CoG distance from the front axle [m]' )
    vehicle_param_l2_arg = DeclareLaunchArgument(
        '/vehicle_params/rear_axle_from_cog',
        default_value=str(vehicle_params['/vehicle_params/rear_axle_from_cog']),
        description='Vehicle parameter: CoG distance from the rear axle [m]')
    vehicle_param_c1_arg = DeclareLaunchArgument(
        '/vehicle_params/front_whl_corn_stiff',
        default_value=str(vehicle_params['/vehicle_params/front_whl_corn_stiff']),
        description='Vehicle parameter: Front wheel cornering stiffness (for single track model) [N/rad]')
    vehicle_param_c2_arg = DeclareLaunchArgument(
        '/vehicle_params/rear_whl_corn_stiff',
        default_value=str(vehicle_params['/vehicle_params/rear_whl_corn_stiff']),
        description='Vehicle parameter: Rear wheel cornering stiffness (for single track model) [N/rad]')
    vehicle_param_swr_arg = DeclareLaunchArgument(
        '/vehicle_params/steering_ratio',
        default_value=str(vehicle_params['/vehicle_params/steering_ratio']),
        description='Vehicle parameter: Steering wheel ratio')

    # CORE

    crp_core = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('crp_launcher'),
                'launch',
                'core_virtual_scenario.launch.py')
        )
    )

    # NODES
  
    static_tf = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('lexus_bringup'),
                'launch',
                'tf_static.launch.py')
        )
    )

    pacmod_extender = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('pacmod_extender'),
                'launch',
                'pacmod_extender.launch.py')
        )
    )

    vehicle_can = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('lexus_bringup'),
                'launch',
                'drivers',
                'can_and_status.launch.py')
        )
    )

    vehicle_speed_control = IncludeLaunchDescription(
        AnyLaunchDescriptionSource(
            join(
                get_package_share_directory('actuator_control'),
                'launch',
                'actuator_control.launch.py')
        )
    )

    sensor_abstraction = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('prcp_sensor_abstraction'),
                'launch',
                'sensor_abstraction.launch.py')
        )
    )


    return LaunchDescription([
        # args
        vehicle_tire_angle_topic_arg,
        vehicle_tire_angle_topic_arg,
        vehicle_param_c1_arg,
        vehicle_param_c2_arg,
        vehicle_param_m_arg,
        vehicle_param_jz_arg,
        vehicle_param_l1_arg,
        vehicle_param_l2_arg,
        vehicle_param_swr_arg,
        vehicle_param_L_arg,

        # core
        crp_core,

        # vehicle nodes
        static_tf,
        vehicle_can,
        pacmod_extender,
        sensor_abstraction,
        vehicle_speed_control
    ])
