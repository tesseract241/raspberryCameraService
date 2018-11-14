#!/bin/bash
BUTTON_PIN=18; MOTION_SENSOR_PIN=17
gpio export BUTTON_PIN in
gpio export BUTTON_PIN in
gpio mode input up BUTTON_PIN
gpio mode input tri MOTION_SENSOR_PIN
gpio edge BUTTON_PIN both
gpio edge MOTION_SENSOR_PIN rising
exit 0
