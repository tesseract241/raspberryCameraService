#include <stdlib.h>
#include "Wire.h"
#include "Button.h"
#include <time.h>
#include <signal.h>

#define BUTTON_PIN 18
#define MOTION_SENSOR_PIN 17
#define HOLDING_TIME 2

volatile bool take_picture_check = FALSE;
volatile bool show_video_check = FALSE;

volatile bool is_button_pressed = FALSE;


void usr1_alrm_handler(int signum){
	take_picture_check = TRUE;
}

void usr2_handler(int signum){
	show_video_check = TRUE;
}

void button_click_handler(){
	is_button_pressed=!is_button_pressed;
	if(is_button_pressed){
		alarm(HOLDING_TIME);
		raise(SIGUSR1);
	}
	else{
		alarm(0);
	}
}

void take_picture_handler(){
	raise(SIGUSR2);
}

void show_video(){
	// screenON(); // To be added when I get the display and can write code for it
	system("raspivid -w 1280 -h 720 -ex auto -awb")
	// screenOFF(5); // To be added when I get the display and can write code for it
}

void take_picture(){
	time_t t = time(NULL);
	char filename[15], buf[70]; // It should be 67, better to be safe
	strftime(&filename, 15, "%Y%m%d%H%M%S", localtime(&t));
	snprintf(buf, "raspistill -w 1920 -h 1080 -q 75 -e png -t 0 -o %s.png", &filename);
	system(&buf);
}

int main(){
	wiringPiSetupSys();
	sigset_t mask, oldmask;
	sigemptyset(&mask);
	sigaddset (&mask, SIGUSR1);
	sigaddset(&mask, SIGUSR2);
	sigaddset(&mask, SIGALRM);
	signal(SIGUSR1,usr1_alrm_handler);
	signal(SIGUSR2, usr2_handler);
	signal(SIGALRM, usr1_alrm_handler);
	wiringPilSR(BUTTON_PIN, INT_EDGE_BOTH, button_click_handler);
	wiringPilSR(MOTION_SENSOR_PIN, INT_EDGE_RISING, take_picture_handler);
	
	// sigprocmask blocks the signals that we use so that we can safely check the global variables that their handlers check
	// sigsuspend unblocks those signals and makes the process sleep until they arrive, at which point it blocks them again, so that we can once again check them and,
	// if the global variables have been modified, escape the inner while loop and execute the proper code. At this point we go back to the inner while and the loop repeats.
	
	sigprocmask(SIG_BLOCK, &mask, &oldmask);
	while(1){
		while(!take_picture_check && !show_video_check) sigsuspend(&oldmask);
		if(take_picture_check){
			take_picture();
			take_picture_check = FALSE;
		}
		if(show_video_check){
			show_video();
			show_video_check = FALSE;
		}
	}
}
