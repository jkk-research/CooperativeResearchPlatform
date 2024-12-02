from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch.conditions import LaunchConfigurationEquals
from launch.launch_description_sources import PythonLaunchDescriptionSource, AnyLaunchDescriptionSource
from ament_index_python.packages import get_package_share_directory
from os.path import join


def generate_launch_description():
    
    # ARGS

    select_gps_arg = DeclareLaunchArgument(
        'select_gps',
        default_value='duro',
        description='Select the GPS to use: nova or duro')

    # novatel gps
    novatel_namespace_arg = DeclareLaunchArgument(
        'novatel_namespace',
        default_value='gps/nova',
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

    # behavior planning
    behavior_current_scenario_arg = DeclareLaunchArgument(
        'behavior_current_scenario',
        default_value='laneFollowWithDefaultSpeed',
        description='Current scenario'
    )
    
    # sensor abstraction
    vehicle_tire_angle_topic_arg = DeclareLaunchArgument(
        'vehicle_tire_angle_topic',
        default_value='/sensing/vehicle/tire_angle',
        description='Length of the scenario in meters')
    local_path_length_arg = DeclareLaunchArgument(
        'local_path_length',
        default_value='70.0',
        description='Length of the scenario in meters')
    

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
                get_package_share_directory('novatel_gps_launcher'),
                'launch',
                'novatel.launch.py')
        ),
        condition = LaunchConfigurationEquals('select_gps', 'nova')
    )

    duro_gps = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('duro_gps_launcher'),
                'launch',
                'duro.launch.py')
        ),
        condition = LaunchConfigurationEquals('select_gps', 'duro')
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
        ),
        # launch_arguments={
        #     'namespace': 'pacmod'
        # }.items()
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
        select_gps_arg,
        novatel_namespace_arg,
        novatel_ip_arg,
        novatel_port_arg,
        novatel_imu_frame_id_arg,
        novatel_frame_id_arg,
        duro_ip_arg,
        duro_port_arg,
        lanelet_file_path_arg,
        lanelet_map_frame_id_arg,
        lanelet_output_topic_arg,
        behavior_current_scenario_arg,
        vehicle_tire_angle_topic_arg,
        lanelet_visualization_topic_arg,
        vehicle_tire_angle_topic_arg,
        local_path_length_arg,

        # core
        crp_core,

        # vehicle nodes
        novatel_gps,
        duro_gps,
        static_tf,
        vehicle_can,
        vehicle_speed_control,
        # lidar_left,
        # lidar_center,
        # lidar_right,
        # camera_zed,
        camera_mpc,
        pacmod_extender,
        lexus_speed_control,

        # nodes
        lanelet_file_loader,
    ])
