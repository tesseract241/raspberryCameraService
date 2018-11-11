# raspberryCameraService
The motivation for this project is simple: I wanted a front door spy-hole that could take photos of people at the door and save them, with eventual face recognition to hook up to it.  
The raspberry has a very easy to use camera module, and a command line utility called raspicam that provides an easy to use interface.  
This repository serves as the missing part: it takes input from a motion sensor to automatically trigger the camera, from a button (which can be the one that the person at the door uses to ring the bell, and the one on the inside to activate the screen hooked up to the raspberry).  
I hope to add support for a web interface, allowing a mobile app to trigger the same functionality by interfacing with this service.

Dependencies:
This project makes use of [WiringPi](http://wiringpi.com/).  
Otherwise, everything is part of the GNU C Library.
