from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration

def generate_launch_description():
    duro_ip_arg = DeclareLaunchArgument(
        'duro_ip',
        default_value='192.168.10.10',
        description='IP address of the duro GPS')
    
    duro_port_arg = DeclareLaunchArgument(
        'duro_port',
        default_value='55555',
        description='Port of the duro GPS')
    
    duro_node = Node(
        package="duro_gps_driver",
        executable="duro_node",
        parameters=[
            {"ip_address": LaunchConfiguration("duro_ip")},
            {"port": LaunchConfiguration("duro_port")},
            {"gps_receiver_frame_id": "duro"},
            {"imu_frame_id": "duro"},
            {"utm_frame_id": "map"},
            {"orientation_source": "gps"},
            {"z_coord_ref_switch": "exact"},
            {"x_coord_offset": 0.0},
            {"y_coord_offset": 0.0},
            # {"x_coord_offset": -697237.0}, # map_gyor_0
            # {"y_coord_offset": -5285644.0}, # map_gyor_0
            # {"x_coord_offset": -639770.0}, # map_zala_0
            # {"y_coord_offset": -5195040.0}, # map_zala_0
            {"z_coord_exact_height": 1.8},
            {"tf_frame_id": "map"},
            {"zero_based_pose": False},
            {"tf_child_frame_id": "lexus3/duro/heading"},
            {"euler_based_orientation": True}           
        ],
        namespace = "gps/duro",
    )
    
    return LaunchDescription([
        duro_ip_arg,
        duro_port_arg,
        duro_node
    ])