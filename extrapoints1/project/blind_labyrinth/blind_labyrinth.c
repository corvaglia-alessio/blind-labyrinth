#include "lpc17xx.h"
#include "../led/led.h"
#include "blind_labyrinth.h"
#include "../timer/timer.h"

//TIMER 0 IS USED FOR MAKING DIRECTION LEDS BLINKING, SETTING THE RIGHT FREQUENCY EVERY TIME BASED ON DISTANCES FROM OBSTACLES
//TIMER 1 IS USED FOR MAKING THE RUNNING LED BLINKING, 1Hz IF THE ROBOT IS MOVING, 5Hz IF THE ROBOT IS IN FRONT OF AN OBSTACLE
//TIMER 2 IS USED FOR MESAURING THE 1 SECOND PRESSURE OF THE RUN BUTTON
//RIT IS USED FOR DE-BOUNCING THE ROTATE BUTTON

char lab [13][15] ={2,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
										0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,
										0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,
										0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,
										0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,
										1,1,1,1,1,0,0,1,0,1,0,0,0,1,0,
										0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
										1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
										1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
										1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,
										1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,
										1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,
										2,0,1,0,0,0,0,0,0,0,0,0,0,0,2};

char x = 7;
char y = 7;
char dir = 'E';
int d;
int started = 0; //at the first boot or after a reset the game is stopped
int reset = 1; //necessary to enable the int0 button for starting a new game only at the first boot or after a reset
int green = 0; //1 if in front of the robot there is a green tile
int win = 0; //1 if the game end with success
																				
void leds(void){
	
	//clean all timers and values "connected" to leds
	LED_Off(0);
	LED_Off(2);
	LED_Off(1);
	LED_Off(3);
	disable_timer(0);
	LPC_TIM0->IR = 1;			/* clear interrupt flag */
	disable_timer(1);
	LPC_TIM1->IR = 1;			/* clear interrupt flag */
	LED_Off(5);
	LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */		
	LPC_TIM2->IR = 1;
	green = 0;
	
	if(lab[y][x] != 2){ //if the robot is not on a green tile
		switch(dir) {
			
			case 'E': //east direction (increasing x)
				for (d=1; d<=6; d++){
					if(x+d == 15){ //check for a wall
						break;
					}
					if(lab[y][x+d]==2){ //check for a green tile
						green=1;
						break;
					}	
					if(lab[y][x+d]==1){ //check for an obstacle
						break;
					}
				}
				if(green==1){ //if there is an exit in front of the robot
					LED_On(2);
				}
				else{
					if(d==1){ //0 slots - dir_led 8hz
						dir_led_blink(0.125);
					}
					else if(d==2 || d==3){ //1 slot and 2 slots - dir_led 4hz
						dir_led_blink(0.25);
					}
					else if(d==4 || d==5 || d==6){ //3 slots, 4 slots, 5, slots - dir_led 2hz
						dir_led_blink(0.5);
					}
					else{ //more than 5 slots - no blink
						LED_On(2);
					}
				}
			break;
				
			case 'W': //west direction (decreasing x)
				for (d=1; d<=6; d++){
					if(x-d == -1){ //check for a wall
						break;
					}
					if(lab[y][x-d]==2){ //check for a green tile
						green=1;
						break;
					}	
					if(lab[y][x-d]==1){ //check for an obstacle
						break;
					}
				}
				if(green==1){ //there is an exit in front of the robot
					LED_On(0);
				}
				else{
					if(d==1){ //0 slots - dir_led 8hz
						dir_led_blink(0.125);
					}
					else if(d==2 || d==3){ //1 slot and 2 slots - dir_led 4hz
						dir_led_blink(0.25);
					}
					else if(d==4 || d==5 || d==6){ //3 slots, 4 slots, 5, slots - dir_led 2hz
						dir_led_blink(0.5);
					}
					else{ //more than 5 slots - no blink
						LED_On(0);
					}
				}
			break;
				
			case 'S': //south direction (increasing y)
				for (d=1; d<=6; d++){
					if(y+d == 13){ //check for a wall
						break;
					}
					if(lab[y+d][x]==2){ //check for a green tile
						green=1;
						break;
					}	
					if(lab[y+d][x]==1){ //check for an obstacle
						break;
					}
				}
				if(green==1){ //there is an exit in front of the robot
					LED_On(1);
				}
				else{
					if(d==1){ //0 slots - dir_led 8hz
						dir_led_blink(0.125);
					}
					else if(d==2 || d==3){ //1 slot and 2 slots - dir_led 4hz
						dir_led_blink(0.25);
					}
					else if(d==4 || d==5 || d==6){ //3 slots, 4 slots, 5, slots - dir_led 2hz
						dir_led_blink(0.5);
					}
					else{ //more than 5 slots - no blink
						LED_On(1);
					}
				}
			break;
				
			case 'N': //nord direction (decreasing y)
				for (d=1; d<=6; d++){ //check for a wall
					if(y-d == -1){
						break;
					}
					if(lab[y-d][x]==2){ //check for a green tile
						green=1;
						break;
					}	
					if(lab[y-d][x]==1){ //check for an obstacle
							break;
					}
				}
				if(green==1){ //there is an exit in front of the robot
					LED_On(3);
				}
				else{
					if(d==1){ //0 slots - dir_led 8hz
						dir_led_blink(0.125);
					}
					else if(d==2 || d==3){ //1 slot and 2 slots - dir_led 4hz
						dir_led_blink(0.25);
					}
					else if(d==4 || d==5 || d==6){ //3 slots, 4 slots, 5, slots - dir_led 2hz
						dir_led_blink(0.5);
					}
					else{ //more than 5 slots - no blink
						LED_On(3);
					}
				}
			break;			
		}
		
		if(d==1 && green==0){ //run_led 5hz, obstacle in front of the robot
			run_led_blink();
		}
	}
	else{ //the robot is on a green tile
		started=0; //all buttons are locked with the exception of reset, it must be pressed and then int0 must be pressed to start a new game
		win=1;
		won();
	}
}

void dir_led_blink(float period){ //initalize the timer for half of the blinking period
	float semiperiod = period/2; //period/2 low, period/2 high
	float value = semiperiod * 25000000; 
	reset_timer(0);
	init_timer(0, value);
	enable_timer(0);
}

void run_led_blink(){
	reset_timer(1);
	init_timer(1, (25000000 * 0.1)); //5hz - 0.2 period - 0.1s low, 0.1s high
	enable_timer(1);
}


void won(){ //timer of 1hz initialized and enabled, the handler will make all leds blinking together
	reset_timer(0);
	init_timer(0, (25000000 * 0.5)); //1hz - 1s period - 0.5s low, 0.5s high
	enable_timer(0);
}
