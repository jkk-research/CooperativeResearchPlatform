from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch.launch_description_sources import PythonLaunchDescriptionSource, AnyLaunchDescriptionSource
from ament_index_python.packages import get_package_share_directory
from os.path import join


def generate_launch_description():
    
    # ARGS

    # lanelet handler
    lanelet_file_path_arg = DeclareLaunchArgument(
        'map_file_path',
        default_value='/home/david/test/test.osm',
        description='Length of the scenario in meters'
    )
    lanelet_map_frame_id_arg = DeclareLaunchArgument(
        'map_frame_id',
        default_value="map_zala_0",
        description='Frame id of the lanelet2 map'
    )
    lanelet_output_topic_arg = DeclareLaunchArgument(
        'map_output_topic',
        default_value='/home/david/test/test.osm',
        description='Length of the scenario in meters'
    )
    vehicle_tire_angle_topic_arg = DeclareLaunchArgument(
        'vehicle_tire_angle_topic',
        default_value='/map/global_static_map_from_file/lanelet2_map',
        description='Length of the scenario in meters'
    )
    lanelet_visualization_topic_arg = DeclareLaunchArgument(
        'map_visualization_topic',
        default_value="/map/global_static_map_from_file/lanelet2_map_visualization",
        description='Output topic for the lanelet2 map binary'
    )

    # sensor abstraction
    vehicle_tire_angle_topic_arg = DeclareLaunchArgument(
        'vehicle_tire_angle_topic',
        default_value='/sensing/vehicle/tire_angle',
        description='Length of the scenario in meters'
    )
    local_path_length_arg = DeclareLaunchArgument(
        'local_path_length',
        default_value='70.0',
        description='Length of the scenario in meters'
    )
    
    # NODES

    novatel_gps = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('novatel_gps_launcher'),
                'launch',
                'novatel.launch.py')
        ),
        launch_arguments={
            'novatel_namespace': 'gps/nova',
            'novatel_ip': '192.168.10.12',
            'novatel_port': '3001',
            'novatel_imu_frame_id': '/lexus3/nova/imu',
            'novatel_frame_id': '/lexus3/gps/nova'
        }.items()
    )

    static_tf = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('lexus_bringup'),
                'launch',
                'tf_static.launch.py')
        )
    )

    vehicle_can = IncludeLaunchDescription(
        AnyLaunchDescriptionSource(
            join(
                get_package_share_directory('lexus_bringup'),
                'launch',
                'drivers',
                'can_pacmod3.launch.xml')
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

    environmental_fusion = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('prcp_situation_analysis'),
                'launch',
                'environmental_fusion.launch.py')
        )
    )

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

    vehicle_control = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('ctrl_vehicle_control'),
                'launch',
                'ctrl_vehicle_control.launch.py')
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

        # vehicle nodes
        novatel_gps,
        static_tf,
        vehicle_can,

        # nodes
        lanelet_file_loader,
        sensor_abstraction,
        environmental_fusion,
        behavior_planning,
        motion_planning,
        vehicle_control,

        planner_lat_lane_follow_ldm
    ])
