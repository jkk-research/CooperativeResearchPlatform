script_dir=$(cd $(dirname ${BASH_SOURCE:-$0}); pwd)
cd $script_dir
while [ ! -e "src/" ]; do
  cd ../
done
echo $(pwd)

rosdep install --from-paths src --ignore-src -r -y
colcon build
