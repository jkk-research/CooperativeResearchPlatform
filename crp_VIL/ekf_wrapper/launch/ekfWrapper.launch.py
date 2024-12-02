from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from ament_index_python.packages import get_package_share_directory


def generate_launch_description():
    ekf_current_pose_topic_arg = DeclareLaunchArgument(
        'ekf_current_pose_topic',
        default_value='current_pose',
        description='Name of the current pose (geometry_msgs/PoseStamped) topic')
    ekf_vehicle_status_topic_arg = DeclareLaunchArgument(
        'ekf_vehicle_status_topic',
        default_value='vehicle_status',
        description='Name of the vehicle status (geometry_msgs/TwistStamped) topic where linear.x is the speed and angular.z is the tire angle')
    ekf_nav_sat_fix_topic_arg = DeclareLaunchArgument(
        'ekf_nav_sat_fix_topic',
        default_value='fix',
        description='Name of the nav sat fix (sensor_msgs/NavSatFix) topic')
    ekf_imu_topic_arg = DeclareLaunchArgument(
        'ekf_imu_topic',
        default_value='imu',
        description='Name of the IMU (sensor_msgs/Imu) topic')

    vehicle_param_c1_arg = DeclareLaunchArgument(
        'vehicle_param_c1',
        default_value='3000.0',
        description='Vehicle parameter c1')
    vehicle_param_c2_arg = DeclareLaunchArgument(
        'vehicle_param_c2',
        default_value='3000.0',
        description='Vehicle parameter c2')
    vehicle_param_m_arg = DeclareLaunchArgument(
        'vehicle_param_m',
        default_value='180.0',
        description='Vehicle parameter m')
    vehicle_param_jz_arg = DeclareLaunchArgument(
        'vehicle_param_jz',
        default_value='270.0',
        description='Vehicle parameter jz')
    vehicle_param_l1_arg = DeclareLaunchArgument(
        'vehicle_param_l1',
        default_value='0.624',
        description='Vehicle parameter l1')
    vehicle_param_l2_arg = DeclareLaunchArgument(
        'vehicle_param_l2',
        default_value='0.676',
        description='Vehicle parameter l2')
    vehicle_param_swr_arg = DeclareLaunchArgument(
        'vehicle_param_swr',
        default_value='1.0',
        description='Vehicle parameter swr')

    ekf_frame_arg = DeclareLaunchArgument(
        'ekf_frame',
        default_value='map',
        description='Frame of the EKF pose')
    
    select_gps_arg = DeclareLaunchArgument(
        'select_gps',
        default_value='nova',
        description='Select the GPS to use: nova or duro')
    
    ekf_localizer = Node(
        package='kalman_pos',
        executable='kalman_pos_node',
        output='screen',
        parameters=[{
            'pose_topic' :           LaunchConfiguration('ekf_current_pose_topic'),
            'vehicle_status_topic' : LaunchConfiguration('ekf_vehicle_status_topic'),
            'nav_sat_fix_topic' :    LaunchConfiguration('ekf_nav_sat_fix_topic'),
            'imu_topic' :            LaunchConfiguration('ekf_imu_topic'),
            # 'inspvax_topic' :      "/lexus3/gps/nova/inspvax",

            'est_cog_topic' :                   "/ekf/estimated_pose_cog",
            'est_trav_distance_odom_topic' :    "/ekf/distance",
            'est_trav_distance_est_pos_topic' : "/ekf/estimated_trav_dist_est_pos",
            'est_baselink_topic' :              "/ekf/estimated_pose_baselink",
            'est_accuracy_topic' :              "/ekf/estimation_accuracy",
            
            'loop_rate_hz' : 60,
            'estimation_method' : 10,
            'gnss_source' : "none",
            'vehicle_type' : "lexus",
            'dynamic_time_calc' : True,
            'kinematic_model_max_speed' : 0.3,
            'do_not_wait_for_gnss_msgs' : True,
            'msg_timeout' : 2000.0,

            'vehicle_param_c1' :  LaunchConfiguration('vehicle_param_c1'),
            'vehicle_param_c2' :  LaunchConfiguration('vehicle_param_c2'),
            'vehicle_param_m' :   LaunchConfiguration('vehicle_param_m'),
            'vehicle_param_jz' :  LaunchConfiguration('vehicle_param_jz'),
            'vehicle_param_l1' :  LaunchConfiguration('vehicle_param_l1'),
            'vehicle_param_l2' :  LaunchConfiguration('vehicle_param_l2'),
            'vehicle_param_swr' : LaunchConfiguration('vehicle_param_swr'),
        }]
    )

    ekf_topic_converter_node = Node(
        package='ekf_wrapper',
        executable='ekf_topic_converter',
        name='ekf_topic_converter',
        output='screen',
        parameters=[{
            'ekf_frame': LaunchConfiguration('ekf_frame')
        }]
    )

    return LaunchDescription([
        ekf_current_pose_topic_arg,
        ekf_vehicle_status_topic_arg,
        ekf_nav_sat_fix_topic_arg,
        ekf_imu_topic_arg,
        vehicle_param_c1_arg,
        vehicle_param_c2_arg,
        vehicle_param_m_arg,
        vehicle_param_jz_arg,
        vehicle_param_l1_arg,
        vehicle_param_l2_arg,
        vehicle_param_swr_arg,
        ekf_frame_arg,
        select_gps_arg,

        ekf_localizer,
        ekf_topic_converter_node
    ])
