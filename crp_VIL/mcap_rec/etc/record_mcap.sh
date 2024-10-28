#!/usr/bin/env bash

## ROS 2 rosbag recorder (MCAP format)

DIR1="$(pwd)"
DIR2="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
cd /mnt/bag
DIR3=$(date -I)_crp
mkdir -p $DIR3
cd $DIR3
TEXT1="$1" # first argument is the text
TIME1="$2"  # second argument is the time
FILE1="${TEXT1}${TIME1}"
PWD1="$(pwd)"
echo "Writing to file: $PWD1/$FILE1"
# 524288000 byte is ~0.5 GB
# 1048576000 byte is ~ 1 GB
TOPICS_FILE=$DIR2/lexus_preset.txt
TOPICS="$(cat $TOPICS_FILE)"
ros2 bag record -s mcap -o $FILE1 --max-cache-size 1048576000 --storage-config-file $DIR2/mcap_writer_options.yaml $TOPICS