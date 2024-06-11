from launch import LaunchDescription
from launch_ros.actions import Node
from launch_ros.actions import PushRosNamespace
from launch_ros.substitutions import FindPackageShare
from launch.actions import DeclareLaunchArgument, GroupAction, IncludeLaunchDescription
from launch.launch_description_sources import AnyLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch.conditions import IfCondition
from ament_index_python.packages import get_package_share_directory
import os


def generate_launch_description():
    # ARGS

    driverModel_params = os.path.join(
        'driverParams.yaml'
    )

    # NODES
    plan_lat_lane_follow_ldm = Node(
        package='plan_lat_lane_follow_ldm',
        executable='plan_lat_lane_follow_ldm',
        name='plan_lat_lane_follow_ldm',
        output='screen',
        parameters=[
            # driverParams
            driverModel_params
        ]
    )

    return LaunchDescription([
        GroupAction(
            actions=[
                plan_lat_lane_follow_ldm
            ]
        )
    ])