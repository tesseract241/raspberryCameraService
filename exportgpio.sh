#!/bin/bash
BUTTON_PIN=27; MOTION_SENSOR_PIN=17
gpio -g export BUTTON_PIN in
gpio -g export MOTION_SENSOR_PIN in
gpio -g mode BUTTON_PIN up
gpio -g mode MOTION_SENSOR_PIN tri
gpio -g edge BUTTON_PIN both
gpio -g edge MOTION_SENSOR_PIN rising
exit 0
