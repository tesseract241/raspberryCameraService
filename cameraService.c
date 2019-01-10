#include <stdlib.h>
#include <stdio.h>
#include <wiringPi.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

#define BUTTON_PIN 21
#define MOTION_SENSOR_PIN 17
#define HOLDING_TIME 2 
#define VIDEO_LENGTH "5000" // In milliseconds

volatile _Bool link_frame_check 	= FALSE;
volatile _Bool show_video_check		= FALSE;

volatile _Bool is_button_pressed 	= FALSE;


void alrm_handler(int signum){
	link_frame_check = TRUE;
}

void sigusr1_handler(int signum){
	show_video_check = TRUE;
}


void show_video(){
	// screenON(); // To be added when I get the display and can write code for it
	system("fbcp &");
	system("raspivid -f -w 1920 -h 1080 -ex auto -t " VIDEO_LENGTH);
	system("pkill fbcp");
	// screenOFF(5); // To be added when I get the display and can write code for it
}

void take_picture(){
	time_t t = time(NULL);
	char filename[15], buf[75];
	strftime(filename, 15, "%Y%m%d%H%M%S", localtime(&t));
	sprintf(buf, "raspistill -w 1920 -h 1080 -q 75 -e png -l %s.png", filename);
	system(buf);
}

void link_frame(){
	time_t t = time(NULL);
	char filename[15], buf[75];
	strftime(filename, 15, "%Y%m%d%H%M%S", localtime(&t));
	sprintf(buf, "raspistill -w 1920 -h 1080 -q 75 -e png -n -t 300 -o %s.png", filename);
	system(buf);
}

void button_click_handler(){
//wiringPi interrupts run in a normal thread, not in ISR context, so all of these calls are fine.
	is_button_pressed=!is_button_pressed;
	if(is_button_pressed){
		alarm(HOLDING_TIME);
//The reason to use kill instead of raise is that we are in a different thread, so raise would happen  in a different thread from the one where sigsuspend is running.
		kill(getpid(), SIGUSR1);
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
	sigaddset(&mask, SIGUSR1);
	signal(SIGALRM, alrm_handler);
	signal(SIGUSR1, sigusr1_handler);
	wiringPiISR(BUTTON_PIN, INT_EDGE_BOTH, button_click_handler);
	wiringPiISR(MOTION_SENSOR_PIN, INT_EDGE_FALLING, take_picture);
	
	// sigprocmask blocks the signals that we use so that we can safely check the global variables that their handlers check
	// sigsuspend unblocks those signals and makes the process sleep until they arrive, at which point it blocks them again, so that we can once again check them and,
	// if the global variables have been modified, escape the inner while loop and execute the proper code. At this point we go back to the inner while and the loop repeats.
	
	sigprocmask(SIG_BLOCK, &mask, &oldmask);
	while(1){
		while(!link_frame_check && !show_video_check) {sigsuspend(&oldmask);}
			if(link_frame_check){
				link_frame_check 	= FALSE;
				link_frame();
			}
			else{
				show_video_check 	= FALSE;
				show_video();
			}
	}
}
