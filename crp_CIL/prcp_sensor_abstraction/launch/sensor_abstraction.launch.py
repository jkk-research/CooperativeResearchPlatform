from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration


def generate_launch_description():
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

    localization_source_arg = DeclareLaunchArgument(
        'localization_source',
        default_value='gnss',
        description='Localization source [ekf or gnss]'
    )
    
    scenario_abstraction = Node(
        package='prcp_sensor_abstraction',
        executable='scenario_abstraction',
        name='scenario_abstraction',
        output='screen',
        parameters=[{
            'local_path_length': LaunchConfiguration('local_path_length'),
            'localization_source': LaunchConfiguration('localization_source')
        }]
    )
    
    ego_abstraction = Node(
        package='prcp_sensor_abstraction',
        executable='ego_abstraction',
        name='ego_abstraction',
        output='screen',
        parameters=[{
            'vehicle_tire_angle_topic': LaunchConfiguration('vehicle_tire_angle_topic'),
            'localization_source': LaunchConfiguration('localization_source')
        }]
    )

    return LaunchDescription([
        vehicle_tire_angle_topic_arg,
        local_path_length_arg,
        localization_source_arg,

        scenario_abstraction,
        ego_abstraction
    ])
