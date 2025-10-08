#!/usr/bin/env bash


script_dir=$(cd $(dirname ${BASH_SOURCE:-$0}); pwd)

exclude_topics="$(tr '\n' '|' < <(grep '[^[:space:]]' "$script_dir/config/camera_topics.txt") | sed 's/|$//')"
lidar_topics="$(tr '\n' '|' < <(grep '[^[:space:]]' "$script_dir/config/lidar_topics.txt") | sed 's/|$//')"
exclude_topics="${exclude_topics}|${lidar_topics}"

exclude_topics="${exclude_topics##|}"  # Remove leading | if any
exclude_topics="${exclude_topics%%|}"  # Remove trailing | if any

echo "Excluding topics: $exclude_topics"

$script_dir/record_all.sh "$@" -x $exclude_topics

