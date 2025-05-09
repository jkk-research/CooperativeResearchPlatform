from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration, TextSubstitution

def generate_launch_description():
    maximumSpeedInit_arg = DeclareLaunchArgument(
        '/test/maximumSpeedInit', default_value=TextSubstitution(text='10.0')
    )

    previewDistance_arg = DeclareLaunchArgument(
        '/test/previewDistance', default_value=TextSubstitution(text='100.0')
    )
    
    test_node = Node(
        package="test_node",
        executable="test_node",
        name="test_node",
        output="screen",
        parameters=[{
            '/test/maximumSpeedInit': LaunchConfiguration('/test/maximumSpeedInit'),
            '/test/previewDistance': LaunchConfiguration('/test/previewDistance')
        }]
    )

    return LaunchDescription([
        maximumSpeedInit_arg,
        previewDistance_arg,
        test_node
    ])
