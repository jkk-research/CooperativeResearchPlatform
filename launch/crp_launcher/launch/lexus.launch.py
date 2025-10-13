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

    localization_source_arg = DeclareLaunchArgument(
        'localization_source',
        default_value='gnss',
        description='Localization source [ekf or gnss]')
    select_gps_arg = DeclareLaunchArgument(
        'select_gps',
        default_value='nova',
        description='Select the GPS to use: nova or duro')

    # novatel gps
    novatel_namespace_arg = DeclareLaunchArgument(
        'novatel_namespace',
        default_value='/lexus3/gps/nova',
        description='Namespace for the Novatel GPS')
    novatel_ip_arg = DeclareLaunchArgument(
        'novatel_ip',
        default_value='192.168.10.12',
        description='IP address of the Novatel GPS')
    novatel_port_arg = DeclareLaunchArgument(
        'novatel_port',
        default_value='3002',
        description='Port of the Novatel GPS')
    novatel_imu_frame_id_arg = DeclareLaunchArgument(
        'novatel_imu_frame_id',
        default_value='/lexus3/nova/imu',
        description='IMU frame id of the Novatel GPS')
    novatel_frame_id_arg = DeclareLaunchArgument(
        'novatel_frame_id',
        default_value='/lexus3/gps/nova',
        description='Frame id of the Novatel GPS')

    # duro gps
    duro_ip_arg = DeclareLaunchArgument(
        'duro_ip',
        default_value='192.168.10.11',
        description='IP address of the duro GPS')
    duro_port_arg = DeclareLaunchArgument(
        'duro_port',
        default_value='55555',
        description='Port of the duro GPS')
    duro_namespace_arg = DeclareLaunchArgument(
        'duro_namespace',
        default_value='/lexus3/gps/duro',
        description='Namespace for the Duro GPS')
    
    # radar
    radar_publish_debug_arg = DeclareLaunchArgument(
        'radar/settings/publish_debug',
        default_value='true',
        description='Enable or disable debug publishing'
    )

    # ekf
    ekf_current_pose_topic_arg = DeclareLaunchArgument(
        'ekf_current_pose_topic',
        default_value=['/lexus3/gps/', LaunchConfiguration('select_gps'), '/current_pose'],
        description='Name of the current pose (geometry_msgs/PoseStamped) topic')
    ekf_vehicle_status_topic_arg = DeclareLaunchArgument(
        'ekf_vehicle_status_topic',
        default_value='/lexus3/vehicle_status',
        description='Name of the vehicle status (geometry_msgs/TwistStamped) topic where linear.x is the speed and angular.z is the tire angle')
    ekf_navsatfix_name = SetLaunchConfiguration(
        'navsatfix_name',
        PythonExpression([
            "'fix' if '", LaunchConfiguration('select_gps'), "' == 'nova' else 'navsatfix'"
        ])
    )
    ekf_nav_sat_fix_topic_arg = DeclareLaunchArgument(
        'ekf_nav_sat_fix_topic',
        default_value=['/lexus3/gps/', LaunchConfiguration('select_gps'), '/', LaunchConfiguration('navsatfix_name')],
        description='Name of the nav sat fix (sensor_msgs/NavSatFix) topic')
    ekf_imu_topic_arg = DeclareLaunchArgument(
        'ekf_imu_topic',
        default_value=['/lexus3/gps/', LaunchConfiguration('select_gps'), '/imu'],
        description='Name of the IMU (sensor_msgs/Imu) topic')
    ekf_frame_arg = DeclareLaunchArgument(
        'ekf_frame',
        default_value='map',
        description='Frame of the EKF pose')

    # lanelet handler
    lanelet_file_path_arg = DeclareLaunchArgument(
        'map_file_path',
        default_value='/home/dev/crp_ws/src/lanelet2_maps/ZalaZone/ZalaZone.osm',
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
    
    # pdp
    pdm = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('pdp_wrapper'),
                'launch',
                'pdpWrapper.launch.py')
        ),
    )

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
                'core.launch.py')
        )
    )

    # NODES

    novatel_gps = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('novatel_gps_wrapper'),
                'launch',
                'novatel.launch.py')
        ),
        condition = LaunchConfigurationEquals('select_gps', 'nova')
    )


    novatel_gps_oem7_driver = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('novatel_oem7_driver'),
                'launch',
                'oem7_net.launch.py')

        ),
        condition = LaunchConfigurationEquals('select_gps', 'nova')
    )

    duro_gps = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('duro_gps_wrapper'),
                'launch',
                'duro.launch.py')
        ),
        condition = LaunchConfigurationEquals('select_gps', 'duro')
    )

    os_lidars_merged = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('lexus_bringup'),
                'launch',
                'drivers',
                'os_composable_raw_merged_b_autoware.launch.py')
        )
    )

    radar_fc = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('rd6_driver'),
                'launch',
                'rd6.launch.py')
        ),
        launch_arguments={
            'radar_config_file' : join(get_package_share_directory('crp_launcher'),'config','radar','fcRadarParams.yaml'),
            'radar_interface'   : 'can4',
            'radar_namespace'   : 'radar/fc',
            'radar_locations_frame_id' : 'radar_front_center',
            'publish_debug'   : LaunchConfiguration('radar/settings/publish_debug')
        }.items()
    )

    radar_fl = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('rd6_driver'),
                'launch',
                'rd6.launch.py')
        ),
        launch_arguments={
            'radar_config_file' : join(get_package_share_directory('crp_launcher'),'config','radar','flRadarParams.yaml'),
            'radar_interface'   : 'can2',
            'radar_namespace'   : 'radar/fl',
            'radar_locations_frame_id' : 'radar_front_left',
            'publish_debug'   : LaunchConfiguration('radar/settings/publish_debug')
        }.items()
    )

    radar_fr = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('rd6_driver'),
                'launch',
                'rd6.launch.py')
        ),
        launch_arguments={
            'radar_config_file' : join(get_package_share_directory('crp_launcher'),'config','radar','frRadarParams.yaml'),
            'radar_interface'   : 'can3',
            'radar_namespace'   : 'radar/fr',
            'radar_locations_frame_id' : 'radar_front_right',
            'publish_debug'   : LaunchConfiguration('radar/settings/publish_debug')
        }.items()
    )

    radar_rl = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('rd6_driver'),
                'launch',
                'rd6.launch.py')
        ),
        launch_arguments={
            'radar_config_file' : join(get_package_share_directory('crp_launcher'),'config','radar','rlRadarParams.yaml'),
            'radar_interface'   : 'can0',
            'radar_namespace'   : 'radar/rl',
            'radar_locations_frame_id' : 'radar_rear_left',
            'publish_debug'   : LaunchConfiguration('radar/settings/publish_debug')
        }.items()
    )

    radar_rr = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('rd6_driver'),
                'launch',
                'rd6.launch.py')
        ),
        launch_arguments={
            'radar_config_file' : join(get_package_share_directory('crp_launcher'),'config','radar','rrRadarParams.yaml'),
            'radar_interface'   : 'can1',
            'radar_namespace'   : 'radar/rr',
            'radar_locations_frame_id' : 'radar_rear_right',
            'publish_debug'   : LaunchConfiguration('radar/settings/publish_debug')
        }.items()
    )
    

    ekf_wrapper = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('ekf_wrapper'),
                'launch',
                'ekfWrapper.launch.py')
        ),
        condition = LaunchConfigurationEquals('localization_source', 'ekf')
    )

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

    camera_mpc = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('mpc_camera_driver'),
                'launch',
                'mpc_driver.launch.py')
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


    return LaunchDescription([
        # args
        localization_source_arg,
        select_gps_arg,

        novatel_namespace_arg,
        novatel_ip_arg,
        novatel_port_arg,
        novatel_imu_frame_id_arg,
        novatel_frame_id_arg,
        duro_ip_arg,
        duro_port_arg,
        duro_namespace_arg,
        radar_publish_debug_arg,
        ekf_current_pose_topic_arg,
        ekf_vehicle_status_topic_arg,
        ekf_navsatfix_name,
        ekf_nav_sat_fix_topic_arg,
        ekf_imu_topic_arg,
        ekf_frame_arg,
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

        # pdp
        pdm,

        # core
        crp_core,

        # vehicle nodes
        static_tf,
        novatel_gps,
        novatel_gps_oem7_driver,
        duro_gps,
        os_lidars_merged,
        radar_fc,
        radar_fl,
        radar_fr,
        radar_rl,
        radar_rr,
        ekf_wrapper,
        vehicle_can,
        camera_mpc,
        pacmod_extender,
        sensor_abstraction,
        vehicle_speed_control,

        lanelet_file_loader,
    ])
