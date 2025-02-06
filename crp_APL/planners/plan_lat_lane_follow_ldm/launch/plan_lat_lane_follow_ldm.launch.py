from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    # NODES
    plan_lat_lane_follow_ldm = Node(
        package='plan_lat_lane_follow_ldm',
        executable='plan_lat_lane_follow_ldm',
        name='plan_lat_lane_follow_ldm',
        output='screen',
        parameters=[
            {"/plan_lat_lane_follow_ldm/nodePointDistances": [10.0, 39.0, 96.0]},
            {"/plan_lat_lane_follow_ldm/trajectoryResolution": 1.0},
            {"/plan_lat_lane_follow_ldm/pLeft": [
                0.0, 0.0, 0.0,
                0.0, 0.0, 0.0,
                0.0, 0.0, 0.0]},
            {"/plan_lat_lane_follow_ldm/pRight": [
                0.0, 0.0, 0.0,
                0.0, 0.0, 0.0,
                0.0, 0.0, 0.0]},
            {"/plan_lat_lane_follow_ldm/pStraight": 0.0}
        ]
    )

    return LaunchDescription([
        plan_lat_lane_follow_ldm
    ])