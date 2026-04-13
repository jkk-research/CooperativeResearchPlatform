#!/usr/bin/env bash


echo "Sending configuration for front-center radar..."
ros2 service call /sensing/radar/fc/sendConfig std_srvs/srv/Trigger | tail -2

echo "Sending configuration for front-left radar..."
ros2 service call /sensing/radar/fl/sendConfig std_srvs/srv/Trigger | tail -2

echo "Sending configuration for front-right radar..."
ros2 service call /sensing/radar/fr/sendConfig std_srvs/srv/Trigger | tail -2

echo "Sending configuration for rear-left radar..."
ros2 service call /sensing/radar/rl/sendConfig std_srvs/srv/Trigger | tail -2

echo "Sending configuration for rear-right radar..."
ros2 service call /sensing/radar/rr/sendConfig std_srvs/srv/Trigger | tail -2
