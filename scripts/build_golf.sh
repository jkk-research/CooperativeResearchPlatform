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
  lanelet_handler
  mcap_rec
  prcp_sensor_abstraction
  scenario_generator
  ros2_socketcan_msgs
  ros2_socketcan
  crp_if
  can_if
  can_driver
)

packages_string=${packages[*]}

packages_paths=$(colcon list --packages-up-to $packages_string -p)

rosdep install --from-paths $packages_paths --ignore-src -r -y

colcon build --packages-select $packages_string $build_args
