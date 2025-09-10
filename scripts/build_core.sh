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
  crp_launcher
)

packages_string=${packages[*]}

rosdep install --from-paths $script_dir/../crp_APL --ignore-src -r -y
colcon build --packages-up-to $packages_string $build_args
