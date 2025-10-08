#!/usr/bin/env bash


read -p "Please specify the bag folder path: " bag_folder

bag_folder="$(realpath -m "$bag_folder")"

if [ ! -d "$bag_folder" ]; then
    read -p "Folder does not exist. Do you want to create it? (y/n): " create_folder
    if [[ "$create_folder" =~ ^[Yy]$ ]]; then
        mkdir -p "$bag_folder"
        echo "Folder created: $bag_folder"
    else
        echo "Folder not created. Exiting."
        return 0
    fi
fi

export crp_bag_folder="$bag_folder"

if ! grep -q "CRP env vars" ~/.bashrc; then
    echo -e "\n# CRP env vars" >> ~/.bashrc
fi

if ! grep -q "export crp_bag_folder=" ~/.bashrc; then
    echo "export crp_bag_folder=\"$bag_folder\"" >> ~/.bashrc
else
    sed -i '/export crp_bag_folder=/c\export crp_bag_folder="'"$bag_folder"'"' ~/.bashrc
fi

if [[ "$(basename -- "$0")" == "setup_recording.sh" ]]; then
    echo -e "\n|> Run 'source ~/.bashrc' or restart your terminal to apply the changes.\n|> You can also source this script next time to set everything automatically: 'source ./setup_recording.sh'"
else
    echo "crp_bag_folder set to $bag_folder and added to ~/.bashrc"
fi

unset bag_folder
unset create_folder