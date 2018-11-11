#include <stdlib.h>
#include "Wire.h"
#include "Button.h"
#include <time.h>
#include <signal.h>

#define BUTTON_PIN 12
#define MOTION_SENSOR_PIN 13
#define HOLDING_TIME 2


void button_click_handler(){
	alarm(HOLDING_TIME);
	raise(SIGUSR1);
}

void button_unclick_handler(){
	alarm(0);
}

void button_hold_handler(){
	raise(SIGUSR2);
}

void motion_sensor_handler(){
	raise(SIGUSR2);
}

void show_video(int signum){
	// screenON(); // To be added when I get the display and can write code for it
	system("raspivid -w 1280 -h 720 -ex auto -awb")
	// screenOFF(5); // To be added when I get the display and can write code for it
}

void take_picture(int signum){
	time_t t = time(NULL);
	char filename[15], buf[70]; // It should be 67, better to be safe
	strftime(&filename, 15, "%Y%m%d%H%M%S", localtime(&t));
	snprintf(buf, "raspistill -w 1920 -h 1080 -q 75 -e png -t 0 -o %s.png", &filename);
	system(&buf);
}

int main(){
	wiringPiSetupSys();
	signal(SIGALRM, take_picture);
	wiringPilSR(BUTTON_PIN, INT_EDGE_FALLING, button_click_handler);					// Need to check whether the button click makes the voltage fall or rise
	wiringPilSR(BUTTON_PIN, INT_EDGE_RISING, button_unclick_handler);					// Need to check whether the button unclick makes the voltage fall or rise
	wiringPilSR(MOTION_SENSOR_PIN, INT_EDGE_FALLING, motion_sensor_handler); // Need to check whether the motion sensor makes the voltage fall or rise
	signal(SIGUSR1, show_video);
	signal(SIGUSR2, take_picture);
	while(1) pause;
}
