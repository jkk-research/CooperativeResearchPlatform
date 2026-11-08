#!/usr/bin/env bash

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
  apl_launcher
)

packages_string=${packages[*]}

# install the dependencies of everything colcon builds below (apl_launcher also pulls in prcp_late_fusion from crp_SIL)
packages_paths=$(colcon list --packages-up-to $packages_string -p)

rosdep install --from-paths $packages_paths --ignore-src -r -y
colcon build --packages-up-to $packages_string $build_args
