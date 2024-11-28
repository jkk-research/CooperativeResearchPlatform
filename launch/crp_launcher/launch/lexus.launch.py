from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, DeclareLaunchArgument, SetLaunchConfiguration
from launch.conditions import LaunchConfigurationEquals
from launch.substitutions import LaunchConfiguration, PythonExpression
from launch.launch_description_sources import PythonLaunchDescriptionSource, AnyLaunchDescriptionSource
from ament_index_python.packages import get_package_share_directory
from os.path import join


def generate_launch_description():
    # ARGS

    localization_source_arg = DeclareLaunchArgument(
        'localization_source',
        default_value='ekf',
        description='Localization source [ekf or gnss]')
    select_gps_arg = DeclareLaunchArgument(
        'select_gps',
        default_value='duro',
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
        default_value='/home/dev/maps/mw.osm',
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
        default_value='70.0',
        description='Length of the scenario in meters')
    
    # vehicle parameters
    vehicle_param_c1_arg = DeclareLaunchArgument(
        'vehicle_param_c1',
        default_value='5000.0',
        description='Vehicle parameter: ')
    vehicle_param_c2_arg = DeclareLaunchArgument(
        'vehicle_param_c2',
        default_value='5000.0',
        description='Vehicle parameter: ')
    vehicle_param_m_arg = DeclareLaunchArgument(
        'vehicle_param_m',
        default_value='2300.0',
        description='Vehicle parameter: ')
    vehicle_param_jz_arg = DeclareLaunchArgument(
        'vehicle_param_jz',
        default_value='4500.0',
        description='Vehicle parameter: ')
    vehicle_param_l1_arg = DeclareLaunchArgument(
        'vehicle_param_l1',
        default_value='1.236',
        description='Vehicle parameter:' )
    vehicle_param_l2_arg = DeclareLaunchArgument(
        'vehicle_param_l2',
        default_value='1.553',
        description='Vehicle parameter: ')
    vehicle_param_swr_arg = DeclareLaunchArgument(
        'vehicle_param_swr',
        default_value='14.8',
        description='Vehicle parameter: ')

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

    duro_gps = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('duro_gps_wrapper'),
                'launch',
                'duro.launch.py')
        ),
        condition = LaunchConfigurationEquals('select_gps', 'duro')
    )
    
    ekf_wrapper = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('ekf_wrapper'),
                'launch',
                'ekfWrapper.launch.py')
        )
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
    

    lidar_left = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('lexus_bringup'),
                'launch',
                'drivers',
                'os_32_left_b.launch.py')
        )
    )

    lidar_center = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('lexus_bringup'),
                'launch',
                'drivers',
                'os_64_center_b.launch.py')
        )
    )

    lidar_right = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('lexus_bringup'),
                'launch',
                'drivers',
                'os_32_right_b.launch.py')
        )
    )

    camera_zed = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('lexus_bringup'),
                'launch',
                'drivers',
                'zed_default_a.launch.py')
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
                get_package_share_directory('lexus_bringup'),
                'launch',
                'speed_control.launch.py')
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

    lexus_speed_control = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('lexus_bringup'),
                'launch',
                'speed_control.launch.py')
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
        vehicle_param_c1_arg,
        vehicle_param_c2_arg,
        vehicle_param_m_arg,
        vehicle_param_jz_arg,
        vehicle_param_l1_arg,
        vehicle_param_l2_arg,
        vehicle_param_swr_arg,

        # core
        crp_core,

        # vehicle nodes
        static_tf,
        novatel_gps,
        duro_gps,
        ekf_wrapper,
        vehicle_can,
        vehicle_speed_control,
        # lidar_left,
        # lidar_center,
        # lidar_right,
        # camera_zed,
        camera_mpc,
        pacmod_extender,
        sensor_abstraction,
        lexus_speed_control,

        # nodes
        lanelet_file_loader,
    ])
