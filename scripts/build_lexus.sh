script_dir=$(cd $(dirname ${BASH_SOURCE:-$0}); pwd)
cd $script_dir
while [ ! -e "src/" ]; do
  cd ../
done
echo "Build location: "$(pwd)

build_args=$@

# build core
trap 'exit' INT # trap ctrl-c
$script_dir/build_core.sh $build_args
if [ $? == 0 ]
then printf "\n! Core build success !\n\n"
else
printf "\n! Core build failed !\n\n"
exit 1
fi
trap - INT

# build lexus packages
packages=(
  tier4_debug_msgs
  tier4_external_api_msgs
  tier4_control_msgs
  tier4_system_msgs
  tier4_api_msgs
  tier4_vehicle_msgs
  tier4_map_msgs
  map_loader
  duro_gps_driver
  duro_gps_wrapper
  kvaser_interface
  lanelet_handler
  lexus_bringup
  actuator_control
  mcap_rec
  mpc_camera_driver
  novatel_gps_msgs
  novatel_gps_driver
  novatel_gps_wrapper
  pacmod_extender
  kalman_pos
  ekf_wrapper
  prcp_sensor_abstraction
  ouster_sensor_msgs
  ouster_ros
)

packages_string=${packages[*]}

packages_paths=$(colcon list --packages-up-to $packages_string -p)

rosdep install --from-paths $packages_paths --ignore-src -r -y

colcon build --packages-select $packages_string $build_args
