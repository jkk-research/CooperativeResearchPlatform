script_dir=$(cd $(dirname ${BASH_SOURCE:-$0}); pwd)
cd $script_dir
while [ ! -e "src/" ]; do
  cd ../
done
echo $(pwd)

rosdep install --from-paths src --ignore-src -r -y
colcon build --packages-select \
tier4_planning_msgs \
autoware_common_msgs \
autoware_planning_msgs \
autoware_perception_msgs \
autoware_control_msgs \
autoware_localization_msgs \
autoware_map_msgs \
crp_msgs \
prcp_sensor_abstraction \
prcp_situation_analysis \
planner_base \
plan_behavior_planning \
plan_lat_lane_follow \
plan_lat_lane_follow_ldm \
plan_lon_emergency \
plan_lon_intelligent_speed_adjust \
plan_motion_planning \
crp_launcher \
ctrl_vehicle_control \
test_node
