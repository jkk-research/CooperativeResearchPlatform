script_dir=$(cd $(dirname ${BASH_SOURCE:-$0}); pwd)
cd $script_dir
while [ ! -e "src/" ]; do
  cd ../
done
echo "Build location: "$(pwd)

# build core
trap 'exit' INT # trap ctrl-c
$script_dir/build_core.sh
trap - INT

# build lexus packages
packages=(
  tier4_debug_msgs
  tier4_external_api_msgs
  tier4_control_msgs
  tier4_system_msgs
  tier4_api_msgs
  tier4_vehicle_msgs
  duro_gps_driver
  duro_gps_wrapper
  kvaser_interface
  lanelet_handler
  lexus_bringup
  mcap_rec
  mpc_camera_driver
  novatel_gps_msgs
  novatel_gps_driver
  novatel_gps_wrapper
  pacmod_extender
  pacmod_interface
  kalman_pos
  ekf_wrapper
  prcp_sensor_abstraction
  ouster_sensor_msgs
  ouster_ros
)

packages_string=""
for package in "${packages[@]}"; do
  packages_string+="$package "
done

packages_paths=$(colcon list --packages-up-to $packages_string -p)

rosdep install --from-paths $packages_paths --ignore-src -r -y

colcon build --packages-select $packages_string