#!/bin/bash
BUTTON_PIN=18; MOTION_SENSOR_PIN=17
gpio export BUTTON_PIN in
gpio export MOTION_SENSOR_PIN in
gpio mode BUTTON_PIN up
gpio mode MOTION_SENSOR_PIN tri
gpio edge BUTTON_PIN both
gpio edge MOTION_SENSOR_PIN rising
exit 0
