from launch import LaunchDescription
from launch_ros.actions import Node
from launch.substitutions import LaunchConfiguration
from launch.actions import DeclareLaunchArgument


def generate_launch_description():
    use_sim_time = LaunchConfiguration('use_sim_time', default='false')
    
    return LaunchDescription([
        DeclareLaunchArgument(
            'use_sim_time',
            default_value='false',
            description='Use simulation (sim) time.'
        ),
        Node(
            package='prcp_grid_cam',
            executable='prcp_grid_cam_node',
            name='prcp_grid_cam',
            output='screen',
            parameters=[
                {'camera_info_topic': '/zed2i/zed_node/left/camera_info'},
                {'camera_stream_topic': '/cam/segmentation/label'},
                {'use_sim_time': use_sim_time},
                
                {'position_x': 0.0},      
                {'position_y': 0.0},      

                # size of a grid cell
                {'length_x': 100.0},
                {'length_y': 100.0},
                {'cell_size': 0.15},
                {'camera_horizon_offset': 0.035},
                {'pitch_in_deg': 0.0}
            ]
        )
    ])
