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
    
    scenario_abstraction = Node(
        package='prcp_sensor_abstraction',
        executable='scenario_abstraction',
        name='scenario_abstraction',
        output='screen',
        parameters=[{
            'local_path_length': LaunchConfiguration('local_path_length')
        }]
    )
    
    ego_abstraction = Node(
        package='prcp_sensor_abstraction',
        executable='ego_abstraction',
        name='ego_abstraction',
        output='screen',
        parameters=[{
            'vehicle_tire_angle_topic': LaunchConfiguration('vehicle_tire_angle_topic')
        }]
    )

    return LaunchDescription([
        vehicle_tire_angle_topic_arg,
        local_path_length_arg,
        scenario_abstraction,
        ego_abstraction
    ])
