from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    ego_fusion = Node(
        namespace='',
        package='prcp_late_fusion',
        executable='ego_fusion',
        name='ego_fusion',
        output='screen'
    )

    scenario_fusion = Node(
        namespace='',
        package='prcp_late_fusion',
        executable='scenario_fusion',
        name='scenario_fusion',
        output='screen'
    )
    
    return LaunchDescription([
        ego_fusion,
        scenario_fusion
    ])