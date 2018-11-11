#!/bin/bash
BUTTON_PIN=12; MOTION_SENSOR_PIN=13
gpio mode input up BUTTON_PIN
gpio mode input MOTION_SENSOR_PIN
exit 0
