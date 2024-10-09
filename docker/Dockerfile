FROM ghcr.io/autowarefoundation/autoware:latest-devel-cuda

RUN apt-get update && apt-get install -y nano && rm -rf /var/lib/apt/lists/*

ARG USERNAME=ros
ARG USER_UID=1000
ARG USER_GID=$USER_UID

SHELL ["/bin/bash", "-c"]

# Create a non-root user
RUN groupadd --gid $USER_GID $USERNAME \
    && useradd -s /bin/bash --uid $USER_UID --gid $USER_GID -m $USERNAME \
    && mkdir /home/${USERNAME}/.config && chown ${USER_UID}:${USER_GID} /home/${USERNAME}/.config

# Set up sudo
RUN apt-get update \
  && apt-get install -y sudo \
  && echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME\
  && chmod 0440 /etc/sudoers.d/$USERNAME \
  && rm -rf /var/lib/apt/lists/*

RUN echo ". /opt/ros/humble/setup.bash" >> /etc/bash.bashrc
RUN echo ". /autoware/install/setup.bash" >> /etc/bash.bashrc
RUN echo ". /usr/share/colcon_argcomplete/hook/colcon-argcomplete.bash" >> /etc/bash.bashrc

USER ros

# Create workspace, copy crp source
RUN mkdir -p /home/ros/ros2_ws/src
COPY . /home/ros/ros2_ws/src/CooperativeResearchPlatform

# tools
RUN sudo apt-get update && \
    sudo apt-get install -y iputils-ping && \
    sudo apt-get install -y net-tools && \
    sudo apt-get install -y ros-humble-rosbag2-storage-mcap && \
    sudo rm -rf /var/lib/apt/lists/*

# install kvaser drivers
RUN sudo apt-get update && \
    sudo apt-get install -y software-properties-common apt-transport-https && \
    sudo apt-add-repository ppa:astuff/kvaser-linux && \
    sudo apt-get update && \
    sudo apt-get install -y kvaser-canlib-dev kvaser-drivers-dkms && \
    sudo sh -c 'echo "deb [trusted=yes] https://s3.amazonaws.com/autonomoustuff-repo/ $(lsb_release -sc) main" > /etc/apt/sources.list.d/autonomoustuff-public.list' && \
    sudo apt-get update && \
    sudo apt-get install -y ros-$ROS_DISTRO-kvaser-interface && \
    sudo rm -rf /var/lib/apt/lists/*

# crp
RUN . /opt/ros/humble/setup.bash && \
    . /autoware/install/setup.bash && \
    cd /home/ros/ros2_ws && \
    sudo apt-get update && \
    rosdep update && \
    rosdep install --from-paths src --ignore-src -y && \
    colcon build --symlink-install && \
    echo ". /home/ros/ros2_ws/install/setup.bash" >> /home/ros/.bashrc && \
    sudo rm -rf /var/lib/apt/lists/*

WORKDIR /home/ros/ros2_ws/
ENTRYPOINT [ "/bin/bash" ]
