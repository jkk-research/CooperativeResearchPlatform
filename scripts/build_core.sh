script_dir=$(cd $(dirname ${BASH_SOURCE:-$0}); pwd)
cd $script_dir
while [ ! -e "src/" ]; do
  cd ../
done
echo "Build location: "$(pwd)

build_args=$@

packages=$(colcon list --base-paths $script_dir/../crp_APL -n)
# add non-APL packages
packages+=(
  tier4_planning_msgs
  autoware_common_msgs
  autoware_planning_msgs
  autoware_perception_msgs
  autoware_control_msgs
  autoware_localization_msgs
  autoware_vehicle_msgs
  autoware_map_msgs
  crp_launcher
)

packages_string=${packages[*]}

rosdep install --from-paths $script_dir/../crp_APL --ignore-src -r -y
colcon build --packages-select $packages_string $build_args
