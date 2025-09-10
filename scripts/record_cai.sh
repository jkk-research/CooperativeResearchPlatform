#!/usr/bin/env bash


script_dir=$(cd $(dirname ${BASH_SOURCE:-$0}); pwd)

topics_file=$script_dir/config/cai_topics.txt

topics=$(cat "$topics_file" | tr '\n' ' ')

$script_dir/record_all.sh "$@" -t $topics
