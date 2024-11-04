from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument, GroupAction, IncludeLaunchDescription

def generate_launch_description():
    # NODES
    plan_lat_lane_follow_ldm = Node(
        package='plan_lat_lane_follow_ldm',
        executable='plan_lat_lane_follow_ldm',
        name='plan_lat_lane_follow_ldm',
        output='screen',
        parameters=[
            {"/plan_lat_lane_follow_ldm/nodePointDistances": [25.0, 50.0, 75.0]},
            {"/plan_lat_lane_follow_ldm/trajectoryResolution": 1.0},
            {"/plan_lat_lane_follow_ldm/pLeft": [
                0.0, 0.0, 0.0,
                0.0, 0.0, 0.0,
                0.0, 0.0, 0.0]},
            {"/plan_lat_lane_follow_ldm/pRight": [
                0.0, 0.0, 0.0,
                0.0, 0.0, 0.0,
                0.0, 0.0, 0.0]},
            {"/plan_lat_lane_follow_ldm/pStraight": -10.0}
        ]
    )

    return LaunchDescription([
        GroupAction(
            actions=[
                plan_lat_lane_follow_ldm
            ]
        )
    ])