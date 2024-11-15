import datetime

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch.actions import ExecuteProcess, OpaqueFunction, IncludeLaunchDescription
from launch_ros.substitutions import FindPackageShare
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.actions import TimerAction


DeclareLaunchArgument("tag", description="a tag to the recorded file", default_value="bosch01"),
tag_ = (LaunchConfiguration("tag"))


def exit_process_function(_launch_context):
    # ~/mcap/mcap info /mnt/bag/2024-04-04_bosch/scenario01_lexus3_2024-04-04_17-00/scenario01_lexus3_2024-04-04_17-00_0.mcap 
    print("Recording finished.")
    # tag_ to string
    tag_ = _launch_context.launch_configurations["tag"]
    print("")
    print("ros2 bag info /mnt/bag/" + date_ + "_bosch/"+ tag_ +"_lexus3_" + date_and_time_)
    print("~/mcap/mcap info /mnt/bag/" + date_ + "_bosch/"+ tag_ +"_lexus3_" + date_and_time_ + "/" + tag_ +"_lexus3_" + date_and_time_ + "_0.mcap")

def get_date_and_time():
    """Get the current date and time."""
    return datetime.datetime.now().strftime('%Y-%m-%d_%H-%M')

def get_date():
    """Get the current date."""
    return datetime.datetime.now().strftime('%Y-%m-%d')

date_and_time_ = get_date_and_time()
date_ = get_date()

def generate_launch_description():
    
    return LaunchDescription([
        ExecuteProcess(
            cmd=['/home/dev/crp_ws/src/CooperativeResearchPlatform/crp_VIL/mcap_rec/etc/record_mcap.sh', tag_, date_and_time_],
            log_cmd=True,
            shell=True,
            output='screen',
            # sigterm_timeout='0',
            # sigkill_timeout='0',
            on_exit=[
                OpaqueFunction(
                    function=exit_process_function
                ),
                # LogInfo(msg='This message prints properly.')
            ]
        ), 
        # static tf
        TimerAction(
        period=2.0, # delay
        actions=[    
            IncludeLaunchDescription(
                PythonLaunchDescriptionSource([
                    FindPackageShare("lexus_bringup"), '/launch', '/tf_static.launch.py'])
            ),
        ]),
    ])