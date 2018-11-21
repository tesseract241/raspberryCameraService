#include <stdlib.h>
#include <stdio.h>
#include <wiringPi.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

#define BUTTON_PIN 27
#define MOTION_SENSOR_PIN 17
#define HOLDING_TIME 2 
#define VIDEO_LENGTH "5000" // In milliseconds

volatile _Bool take_picture_check = FALSE;
volatile _Bool show_video_check = FALSE;

volatile _Bool is_button_pressed = FALSE;


void alrm_handler(int signum){
	take_picture_check = TRUE;
}


void show_video(){
	// screenON(); // To be added when I get the display and can write code for it
	system("raspivid -f -w 480 -h 320 -ex auto -t " VIDEO_LENGTH);
	// screenOFF(5); // To be added when I get the display and can write code for it
}

void take_picture(){
	time_t t = time(NULL);
	char filename[15], buf[75];
	strftime(filename, 15, "%Y%m%d%H%M%S", localtime(&t));
	sprintf(buf, "raspistill -w 1920 -h 1080 -q 75 -e png -n -t 100 -o %s.png", filename);
	system(buf);
}

void button_click_handler(){
//wiringPi interrupts run in a normal thread, not in ISR context, so all of these calls are fine.
	is_button_pressed=!is_button_pressed;
	if(is_button_pressed){
		alarm(HOLDING_TIME);
//Even if the alarm goes off while show_video is still running is fine, as we are in a normal context.
		show_video();
	}
	else{
		alarm(0);
	}
}

int main(){
	wiringPiSetupSys();
	sigset_t mask, oldmask;
	sigemptyset(&mask);
	sigaddset(&mask, SIGALRM);
	signal(SIGUSR1, alrm_handler);
	wiringPiISR(BUTTON_PIN, INT_EDGE_BOTH, button_click_handler);
	wiringPiISR(MOTION_SENSOR_PIN, INT_EDGE_FALLING, take_picture);
	
	// sigprocmask blocks the signals that we use so that we can safely check the global variables that their handlers check
	// sigsuspend unblocks those signals and makes the process sleep until they arrive, at which point it blocks them again, so that we can once again check them and,
	// if the global variables have been modified, escape the inner while loop and execute the proper code. At this point we go back to the inner while and the loop repeats.
	
	sigprocmask(SIG_BLOCK, &mask, &oldmask);
	while(1){
		while(!take_picture_check) {sigsuspend(&oldmask);}
			take_picture();
			take_picture_check = FALSE;
	}
}
