#!/bin/bash
BUTTON_PIN=27; MOTION_SENSOR_PIN=17
gpio export $BUTTON_PIN in
gpio export $MOTION_SENSOR_PIN in
gpio -g mode $BUTTON_PIN up
gpio -g mode $MOTION_SENSOR_PIN tri
gpio edge $BUTTON_PIN both
gpio edge $MOTION_SENSOR_PIN rising
exit 0
