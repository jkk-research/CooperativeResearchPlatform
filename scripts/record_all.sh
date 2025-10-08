#!/usr/bin/env bash


script_dir=$(cd $(dirname ${BASH_SOURCE:-$0}); pwd)

# Parse command line arguments
topics=()
exclude_topics=()
while [[ $# -gt 0 ]]; do
    case "$1" in
        -p|-path|--path)
            shift
            if [[ $# -gt 0 ]]; then
                full_path="$1"
                shift
            else
                echo "Error: -path requires a file name argument."
                exit 1
            fi
            ;;
        -h|--help)
            echo "Usage: $0 [-path <file_name>] [-h|--help]"
            echo ""
            echo "Options:"
            echo "  -p, -path, --path   Specify the output file name (without extension)."
            echo "  -x, --exclude       Exclude topics from recording (list after flag)."
            echo "  -t, --topics        Record only specified topics (list after flag)."
            echo "  -h, --help          Show this help message."
            exit 0
            ;;
        -x|--exclude)
            shift
            while [[ $# -gt 0 && ! "$1" =~ ^- ]]; do
                exclude_topics+=("$1")
                shift
            done
            ;;
        -t|--topics)
            shift
            while [[ $# -gt 0 && ! "$1" =~ ^- ]]; do
                topics+=("$1")
                shift
            done
            ;;
        *)
            # If no flags are set, treat the argument as the file name if not already set
            file_name="$1"
            shift
            ;;
    esac
done

if [ -z "$full_path" ]; then
    if [ -n "$crp_bag_folder" ]; then
        echo "crp_bag_folder is set to '$crp_bag_folder'."
    else
        echo -e "crp_bag_folder is not set. Run or source scripts/setup_recording.sh to set it:\n > source $script_dir/setup_recording.sh"
        exit 0
    fi

    folder_name=$crp_bag_folder/$(date -I)_crp
    full_path=$folder_name/$file_name
else
    # If file_name is set, set folder_name accordingly
    folder_name=$(dirname "$full_path")
fi

mkdir -p $folder_name

bag_output="${full_path}_$(date +%Y%m%d_%H%M%S)"

echo "Writing to file: $bag_output"

if [ ${#exclude_topics[@]} -gt 0 ]; then
    ros2 bag record -s mcap -o $bag_output --max-cache-size 1048576000 --storage-config-file $script_dir/config/mcap_writer_options.yaml --all -x "${exclude_topics[@]}"
elif [ ${#topics[@]} -gt 0 ]; then
    ros2 bag record -s mcap -o $bag_output --max-cache-size 1048576000 --storage-config-file $script_dir/config/mcap_writer_options.yaml "${topics[@]}"
else
    ros2 bag record -s mcap -o $bag_output --max-cache-size 1048576000 --storage-config-file $script_dir/config/mcap_writer_options.yaml --all
fi
