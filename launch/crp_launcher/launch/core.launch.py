from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.launch_description_sources import PythonLaunchDescriptionSource
from ament_index_python.packages import get_package_share_directory
from os.path import join


def generate_launch_description():
    # vehicle control
    lat_accel_limit_arg = DeclareLaunchArgument(
        'lat_accel_lim',
        default_value='3.0',
        description='Lateral acceleration limit')
    jerk_limit_arg = DeclareLaunchArgument(
        'jerk_lim',
        default_value='0.5f',
        description='Jerk limit')
    
    # vehicle params
    vehicle_param_L_arg = DeclareLaunchArgument(
        'vehicle_param_L',
        default_value='2.7',
        description='Vehicle parameter: ')
    
    # NODES

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

    planner_lon_intelligent_speed_adjust = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('plan_lon_intelligent_speed_adjust'),
                'launch',
                'plan_lon_intelligent_speed_adjust.launch.py'
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

    vehicle_control_lat = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('ctrl_vehicle_control_lat_compensatory'),
                'launch',
                'ctrl_vehicle_control_lat_compensatory.launch.py')
        )
    )

    vehicle_control_long = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            join(
                get_package_share_directory('ctrl_vehicle_control_long'),
                'launch',
                'ctrl_vehicle_control_long.launch.py')
        )
    )

    return LaunchDescription([
        # args
        lat_accel_limit_arg,
        jerk_limit_arg,
        vehicle_param_L_arg,

        # nodes
        environmental_fusion,
        behavior_planning,
        planner_lat_lane_follow_ldm,
        planner_lon_intelligent_speed_adjust,
        motion_planning,
        vehicle_control,
        vehicle_control_lat,
        vehicle_control_long,

        
    ])
