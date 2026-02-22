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

    # kinematic state
    kinematic_state_source_arg = DeclareLaunchArgument(
        'kinematic_state_source',
        default_value='gnss',
        description='Kinematic state (twist+accel) source. Possible values: [gnss, vehicle].'
    )
    twist_topic_arg = DeclareLaunchArgument(
        'twist_topic',
        default_value=['/sensing/', LaunchConfiguration('kinematic_state_source'), '/twist'],
        description='Name of the twist topic. Default: derived from kinemtaic_state_source arg.'
    )
    accel_topic_arg = DeclareLaunchArgument(
        'accel_topic',
        default_value=['/sensing/', LaunchConfiguration('kinematic_state_source'), '/accel'],
        description='Name of the accel topic. Default: derived from kinemtaic_state_source arg.'
    )

    # localization
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

    # radar doppler compensation
    doppcomp_input_pcd_topic_arg = DeclareLaunchArgument(
        'doppcomp/input_pcd_topic',
        default_value='points',
        description='Input PointCloud2 topic for the doppler compensator.'
    )
    doppcomp_output_pcd_topic_arg = DeclareLaunchArgument(
        'doppcomp/output_pcd_topic',
        default_value='dvcompensated_points',
        description='Input PointCloud2 topic for the doppler compensator.'
    )
    doppcomp_override_ego_twist_frame_arg = DeclareLaunchArgument(
        'doppcomp/override_ego_twist_frame',
        default_value='base_link', # TODO add frame_id to twist topic then remove this
        description='Override the frame of the radar for the transformation of the points. Leave empty to use the frameid defined in the input point cloud message.'
    )

    # radar pointcloud aggregation
    pcdagg_input_pcd_topic_arg = DeclareLaunchArgument(
        'pcdagg/input_pcd_topic',
        default_value='dvcompensated_points',
        description='Input PointCloud2 topic for the aggregator.'
    )
    pcdagg_output_pcd_topic_arg = DeclareLaunchArgument(
        'pcdagg/output_pcd_topic',
        default_value='aggregated_points',
        description='Output PointCloud2 topic for the aggregator.'
    )
    pcdagg_aggregation_time_window_arg = DeclareLaunchArgument(
        'pcdagg/aggregation_time_window_sec',
        default_value='0.5',
        description='Time window for the aggregation.'
    )
    pcdagg_ego_twist_frame_arg = DeclareLaunchArgument(
        'pcdagg/override_ego_twist_frame',
        default_value='', # TODO add frame_id to twist topic then remove this
        description='Override the frame of the radar for the transformation of the points. Leave empty to use the frameid defined in the input point cloud message.'
    )

    # radar pointcloud merger
    radar_pointcloud_merger_in_pcd_topics_arg = DeclareLaunchArgument(
        'pcdmerger/in_pcd_topics',
        default_value='''[
            /sensing/radar/fc/aggregated_points, 
            /sensing/radar/fl/aggregated_points, 
            /sensing/radar/fr/aggregated_points, 
            /sensing/radar/rl/aggregated_points, 
            /sensing/radar/rr/aggregated_points
        ]''',
        description='PointCloud2 topics to be merged. The first one will be the trigger for merging.'
    )
    radar_pointcloud_merger_out_topic_arg = DeclareLaunchArgument(
        'pcdmerger/out_topic',
        default_value='merged_points',
        description='Output PointCloud2 topic for the merger.'
    )
    radar_pointcloud_merger_ego_frame_arg = DeclareLaunchArgument(
        'pcdmerger/ego_frame',
        default_value='base_link',
        description='Input localization (PoseWithCovarianceStamped) topic for the aggregator.'
    )
    radar_pointcloud_merger_merge_trigger_timeout_arg = DeclareLaunchArgument(
        'pcdmerger/merge_trigger_timeout_sec',
        default_value='0.1',
        description='Timeout for the trigger topic. The trigger is changed if the current trigger times out.'
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

    novatel_gps_wrapper = IncludeLaunchDescription(
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

    radar_driver_fc = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('rd6_driver'),
                'launch',
                'rd6.launch.py')
        ),
        launch_arguments={
            'radar_config_file' : join(get_package_share_directory('crp_launcher'),'config','radar','fcRadarParams.yaml'),
            'radar_interface'   : 'can4',
            'radar_namespace'   : '/sensing/radar/fc',
            'radar_locations_frame_id' : 'radar_front_center',
            'publish_debug'   : LaunchConfiguration('radar/settings/publish_debug')
        }.items()
    )

    radar_doppler_compensator_fc = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('radar_pointcloud_preprocessor'),
                'launch',
                'dopplerCompensator.launch.py'
            )
        ),
        launch_arguments={
            'doppcomp/namespace'               : '/sensing/radar/fc',
            'doppcomp/input_pcd_topic'         : LaunchConfiguration('doppcomp/input_pcd_topic'),
            'doppcomp/output_pcd_topic'        : LaunchConfiguration('doppcomp/output_pcd_topic'),
            'doppcomp/twist_topic'             : LaunchConfiguration('twist_topic'),
            'doppcomp/override_ego_twist_frame': LaunchConfiguration('doppcomp/override_ego_twist_frame')
        }.items()
    )

    radar_pointcloud_aggregator_fc = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('radar_pointcloud_preprocessor'),
                'launch',
                'pointcloudAggregator.launch.py'
            )
        ),
        launch_arguments={
            'pcdagg/namespace'                  : '/sensing/radar/fc',
            'pcdagg/input_pcd_topic'            : LaunchConfiguration('pcdagg/input_pcd_topic'),
            'pcdagg/output_pcd_topic'           : LaunchConfiguration('pcdagg/output_pcd_topic'),
            'pcdagg/twist_topic'                : LaunchConfiguration('twist_topic'),
            'pcdagg/aggregation_time_window_sec': LaunchConfiguration('pcdagg/aggregation_time_window_sec'),
            'pcdagg/override_ego_twist_frame'   : LaunchConfiguration('pcdagg/override_ego_twist_frame')
        }.items()
    )

    radar_driver_fl = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('rd6_driver'),
                'launch',
                'rd6.launch.py')
        ),
        launch_arguments={
            'radar_config_file' : join(get_package_share_directory('crp_launcher'),'config','radar','flRadarParams.yaml'),
            'radar_interface'   : 'can2',
            'radar_namespace'   : '/sensing/radar/fl',
            'radar_locations_frame_id' : 'radar_front_left',
            'publish_debug'   : LaunchConfiguration('radar/settings/publish_debug')
        }.items()
    )

    radar_doppler_compensator_fl = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('radar_pointcloud_preprocessor'),
                'launch',
                'dopplerCompensator.launch.py'
            )
        ),
        launch_arguments={
            'doppcomp/namespace'               : '/sensing/radar/fl',
            'doppcomp/input_pcd_topic'         : LaunchConfiguration('doppcomp/input_pcd_topic'),
            'doppcomp/output_pcd_topic'        : LaunchConfiguration('doppcomp/output_pcd_topic'),
            'doppcomp/twist_topic'             : LaunchConfiguration('twist_topic'),
            'doppcomp/override_ego_twist_frame': LaunchConfiguration('doppcomp/override_ego_twist_frame')
        }.items()
    )

    radar_pointcloud_aggregator_fl = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('radar_pointcloud_preprocessor'),
                'launch',
                'pointcloudAggregator.launch.py'
            )
        ),
        launch_arguments={
            'pcdagg/namespace'                  : '/sensing/radar/fl',
            'pcdagg/input_pcd_topic'            : LaunchConfiguration('pcdagg/input_pcd_topic'),
            'pcdagg/output_pcd_topic'           : LaunchConfiguration('pcdagg/output_pcd_topic'),
            'pcdagg/twist_topic'                : LaunchConfiguration('twist_topic'),
            'pcdagg/aggregation_time_window_sec': LaunchConfiguration('pcdagg/aggregation_time_window_sec'),
            'pcdagg/override_ego_twist_frame'   : LaunchConfiguration('pcdagg/override_ego_twist_frame')
        }.items()
    )

    radar_driver_fr = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('rd6_driver'),
                'launch',
                'rd6.launch.py')
        ),
        launch_arguments={
            'radar_config_file' : join(get_package_share_directory('crp_launcher'),'config','radar','frRadarParams.yaml'),
            'radar_interface'   : 'can3',
            'radar_namespace'   : '/sensing/radar/fr',
            'radar_locations_frame_id' : 'radar_front_right',
            'publish_debug'   : LaunchConfiguration('radar/settings/publish_debug')
        }.items()
    )

    radar_doppler_compensator_fr = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('radar_pointcloud_preprocessor'),
                'launch',
                'dopplerCompensator.launch.py'
            )
        ),
        launch_arguments={
            'doppcomp/namespace'               : '/sensing/radar/fr',
            'doppcomp/input_pcd_topic'         : LaunchConfiguration('doppcomp/input_pcd_topic'),
            'doppcomp/output_pcd_topic'        : LaunchConfiguration('doppcomp/output_pcd_topic'),
            'doppcomp/twist_topic'             : LaunchConfiguration('twist_topic'),
            'doppcomp/override_ego_twist_frame': LaunchConfiguration('doppcomp/override_ego_twist_frame')
        }.items()
    )

    radar_pointcloud_aggregator_fr = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('radar_pointcloud_preprocessor'),
                'launch',
                'pointcloudAggregator.launch.py'
            )
        ),
        launch_arguments={
            'pcdagg/namespace'                  : '/sensing/radar/fr',
            'pcdagg/input_pcd_topic'            : LaunchConfiguration('pcdagg/input_pcd_topic'),
            'pcdagg/output_pcd_topic'           : LaunchConfiguration('pcdagg/output_pcd_topic'),
            'pcdagg/twist_topic'                : LaunchConfiguration('twist_topic'),
            'pcdagg/aggregation_time_window_sec': LaunchConfiguration('pcdagg/aggregation_time_window_sec'),
            'pcdagg/override_ego_twist_frame'   : LaunchConfiguration('pcdagg/override_ego_twist_frame')
        }.items()
    )

    radar_driver_rl = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('rd6_driver'),
                'launch',
                'rd6.launch.py')
        ),
        launch_arguments={
            'radar_config_file' : join(get_package_share_directory('crp_launcher'),'config','radar','rlRadarParams.yaml'),
            'radar_interface'   : 'can0',
            'radar_namespace'   : '/sensing/radar/rl',
            'radar_locations_frame_id' : 'radar_rear_left',
            'publish_debug'   : LaunchConfiguration('radar/settings/publish_debug')
        }.items()
    )

    radar_doppler_compensator_rl = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('radar_pointcloud_preprocessor'),
                'launch',
                'dopplerCompensator.launch.py'
            )
        ),
        launch_arguments={
            'doppcomp/namespace'               : '/sensing/radar/rl',
            'doppcomp/input_pcd_topic'         : LaunchConfiguration('doppcomp/input_pcd_topic'),
            'doppcomp/output_pcd_topic'        : LaunchConfiguration('doppcomp/output_pcd_topic'),
            'doppcomp/twist_topic'             : LaunchConfiguration('twist_topic'),
            'doppcomp/override_ego_twist_frame': LaunchConfiguration('doppcomp/override_ego_twist_frame')
        }.items()
    )

    radar_pointcloud_aggregator_rl = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('radar_pointcloud_preprocessor'),
                'launch',
                'pointcloudAggregator.launch.py'
            )
        ),
        launch_arguments={
            'pcdagg/namespace'                  : '/sensing/radar/rl',
            'pcdagg/input_pcd_topic'            : LaunchConfiguration('pcdagg/input_pcd_topic'),
            'pcdagg/output_pcd_topic'           : LaunchConfiguration('pcdagg/output_pcd_topic'),
            'pcdagg/twist_topic'                : LaunchConfiguration('twist_topic'),
            'pcdagg/aggregation_time_window_sec': LaunchConfiguration('pcdagg/aggregation_time_window_sec'),
            'pcdagg/override_ego_twist_frame'   : LaunchConfiguration('pcdagg/override_ego_twist_frame')
        }.items()
    )

    radar_driver_rr = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('rd6_driver'),
                'launch',
                'rd6.launch.py')
        ),
        launch_arguments={
            'radar_config_file' : join(get_package_share_directory('crp_launcher'),'config','radar','rrRadarParams.yaml'),
            'radar_interface'   : 'can1',
            'radar_namespace'   : '/sensing/radar/rr',
            'radar_locations_frame_id' : 'radar_rear_right',
            'publish_debug'   : LaunchConfiguration('radar/settings/publish_debug')
        }.items()
    )

    radar_doppler_compensator_rr = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('radar_pointcloud_preprocessor'),
                'launch',
                'dopplerCompensator.launch.py'
            )
        ),
        launch_arguments={
            'doppcomp/namespace'               : '/sensing/radar/rr',
            'doppcomp/input_pcd_topic'         : LaunchConfiguration('doppcomp/input_pcd_topic'),
            'doppcomp/output_pcd_topic'        : LaunchConfiguration('doppcomp/output_pcd_topic'),
            'doppcomp/twist_topic'             : LaunchConfiguration('twist_topic'),
            'doppcomp/override_ego_twist_frame': LaunchConfiguration('doppcomp/override_ego_twist_frame')
        }.items()
    )

    radar_pointcloud_aggregator_rr = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('radar_pointcloud_preprocessor'),
                'launch',
                'pointcloudAggregator.launch.py'
            )
        ),
        launch_arguments={
            'pcdagg/namespace'                  : '/sensing/radar/rr',
            'pcdagg/input_pcd_topic'            : LaunchConfiguration('pcdagg/input_pcd_topic'),
            'pcdagg/output_pcd_topic'           : LaunchConfiguration('pcdagg/output_pcd_topic'),
            'pcdagg/twist_topic'                : LaunchConfiguration('twist_topic'),
            'pcdagg/aggregation_time_window_sec': LaunchConfiguration('pcdagg/aggregation_time_window_sec'),
            'pcdagg/override_ego_twist_frame'   : LaunchConfiguration('pcdagg/override_ego_twist_frame')
        }.items()
    )

    radar_pointcloud_merger = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('radar_pointcloud_preprocessor'),
                'launch',
                'pointcloudMerger.launch.py'
            )
        ),
        launch_arguments={
            'pcdmerger/namespace'                : '/sensing/radar',
            'pcdmerger/in_pcd_topics'            : LaunchConfiguration('pcdmerger/in_pcd_topics'),
            'pcdmerger/out_topic'                : LaunchConfiguration('pcdmerger/out_topic'),
            'pcdmerger/ego_frame'                : LaunchConfiguration('pcdmerger/ego_frame'),
            'pcdmerger/merge_trigger_timeout_sec': LaunchConfiguration('pcdmerger/merge_trigger_timeout_sec'),
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

        kinematic_state_source_arg,
        twist_topic_arg,
        accel_topic_arg,

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
        doppcomp_input_pcd_topic_arg,
        doppcomp_output_pcd_topic_arg,
        doppcomp_override_ego_twist_frame_arg,
        pcdagg_input_pcd_topic_arg,
        pcdagg_output_pcd_topic_arg,
        pcdagg_aggregation_time_window_arg,
        pcdagg_ego_twist_frame_arg,
        radar_pointcloud_merger_in_pcd_topics_arg,
        radar_pointcloud_merger_out_topic_arg,
        radar_pointcloud_merger_ego_frame_arg,
        radar_pointcloud_merger_merge_trigger_timeout_arg,
        
        ekf_current_pose_topic_arg,
        ekf_vehicle_status_topic_arg,
        ekf_navsatfix_name,
        ekf_nav_sat_fix_topic_arg,
        ekf_imu_topic_arg,
        ekf_frame_arg,

        lanelet_file_path_arg,
        lanelet_map_frame_id_arg,
        lanelet_output_topic_arg,
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
        static_tf,
        novatel_gps_wrapper,
        novatel_gps_oem7_driver,
        duro_gps,
        os_lidars_merged,
        radar_driver_fc,
        radar_doppler_compensator_fc,
        radar_pointcloud_aggregator_fc,
        radar_driver_fl,
        radar_doppler_compensator_fl,
        radar_pointcloud_aggregator_fl,
        radar_driver_fr,
        radar_doppler_compensator_fr,
        radar_pointcloud_aggregator_fr,
        radar_driver_rl,
        radar_doppler_compensator_rl,
        radar_pointcloud_aggregator_rl,
        radar_driver_rr,
        radar_doppler_compensator_rr,
        radar_pointcloud_aggregator_rr,
        radar_pointcloud_merger,
        ekf_wrapper,
        vehicle_can,
        camera_mpc,
        pacmod_extender,
        sensor_abstraction,
        vehicle_speed_control,

        lanelet_file_loader,
    ])
