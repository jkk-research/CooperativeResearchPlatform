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

    # lanelet handler
    lanelet_file_path_arg = DeclareLaunchArgument(
        'map_file_path',
        default_value='/home/dev/crp_ws/src/lanelet2_maps/ZalaZone/ZalaZone_Motorway_local.osm',
        description='Length of the scenario in meters')
    lanelet_map_frame_id_arg = DeclareLaunchArgument(
        'map_frame_id',
        default_value="map_zala_0",
        description='Frame id of the lanelet2 map')
    lanelet_output_topic_arg = DeclareLaunchArgument(
        'map_output_topic',
        default_value='map/global_static_map_from_file/lanelet2_map',
        description='Length of the scenario in meters')
    lanelet_visualization_topic_arg = DeclareLaunchArgument(
        'map_visualization_topic',
        default_value="/map/global_static_map_from_file/lanelet2_map_visualization",
        description='Output topic for the lanelet2 map binary')

    # sensor abstraction
    vehicle_tire_angle_topic_arg = DeclareLaunchArgument(
        'vehicle_tire_angle_topic',
        default_value='/sensing/vehicle/tire_angle',
        description='Length of the scenario in meters')
    local_path_length_arg = DeclareLaunchArgument(
        'local_path_length',
        default_value='100.0',
        description='Length of the scenario in meters')
    
    # vehicle control
    lat_accel_limit_arg = DeclareLaunchArgument(
        'lat_accel_lim',
        default_value='3.0',
        description='Lateral acceleration limit')
    jerk_limit_arg = DeclareLaunchArgument(
        'jerk_lim',
        default_value='0.5',
        description='Jerk limit')

    # controllers
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
        default_value='nmpc',
        description='Lat controller to use. Possible values: comp, purep, stanley, nmpc'
    )
    ctrl_long_method_arg = DeclareLaunchArgument(
        'ctrl_long_method',
        default_value='long',
        description='Controller to use. Possible values: long'
    )

    # vehicle parameters
    vehicle_params = load_yaml(join(
        get_package_share_directory('crp_launcher'),
        'config',
        'vehicle',
        'golfParams.yaml'
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
                'core.launch.py')
        )
    )

    # NODES

    # static_tf = IncludeLaunchDescription(
    #     PythonLaunchDescriptionSource(
    #         join(
    #             get_package_share_directory('tf_crp'),
    #             'launch',
    #             'tf_static.launch.py')
    #     )
    # )

    # vehicle_speed_control = IncludeLaunchDescription(
    #     AnyLaunchDescriptionSource(
    #         join(
    #             get_package_share_directory('actuator_control'),
    #             'launch',
    #             'actuator_control.launch.py')
    #     )
    # )

    lanelet_file_loader = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('lanelet_handler'),
                'launch',
                'laneletFileLoader.launch.py')
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

    can_driver = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('can_driver'),
                'launch',
                'can_driver.launch.py')
        )
    )

    mpc_25_wrapper = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('mpc_25_wrapper'),
                'launch',
                'mpc_25_wrapper.launch.py')
        )
    )


    return LaunchDescription([
        # args
        lanelet_file_path_arg,
        lanelet_map_frame_id_arg,
        lanelet_output_topic_arg,
        vehicle_tire_angle_topic_arg,
        lanelet_visualization_topic_arg,
        vehicle_tire_angle_topic_arg,
        local_path_length_arg,
        lat_accel_limit_arg,
        jerk_limit_arg,
        ctrl_use_combined_controller_arg,
        ctrl_combined_method_arg,
        ctrl_lat_method_arg,
        ctrl_long_method_arg,
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
        sensor_abstraction,

        lanelet_file_loader,
        can_driver,
        mpc_25_wrapper,
    ])
