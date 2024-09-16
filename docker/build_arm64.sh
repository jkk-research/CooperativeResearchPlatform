#!/bin/bash

if [ ! -f "docker/Dockerfile" ]; then
    echo "Please run the script from the root directory of the project"
    exit 1
fi

docker build --platform linux/arm64 -t crp_arm64 -f docker/Dockerfile .
