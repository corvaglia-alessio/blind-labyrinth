#include "lpc17xx.h"
#include "blind_labyrinth.h"
#include "../GLCD/GLCD.H"
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

int step = 16;								
char x = 7;
char y = 7;
char dir = 'E';
char mode = 'm';
int started=0;
int win=0;
										
	
//based on direction, it draw the correct symbol in correct place using coordinates x and y and the step value
void draw_robot(void){
	switch(dir){
		case 'E':
			//GUI_Text((step*x), (50+(step*y)), (uint8_t*) ">", Black, White);
			LCD_DrawTriangle((step*x), (50+(step*y)), 14, 'e', Blue2);
			break;
		case 'N':
			//GUI_Text((step*x), (50+(step*y)), (uint8_t*) "^", Black, White);
			LCD_DrawTriangle((step*x), (50+14+(step*y)), 14, 'n', Blue2);
			break;
		case 'S':
			//GUI_Text((step*x), (50+(step*y)), (uint8_t*) "v", Black, White);
			LCD_DrawTriangle((step*x), (50+(step*y)), 14, 's', Blue2);
			break;
		case 'W':
			//GUI_Text((step*x), (50+(step*y)), (uint8_t*) "<", Black, White);
			LCD_DrawTriangle((14+(step*x)), (50+(step*y)), 14, 'w', Blue2);
			break;
		default:
		break;
	}
}

//draw a space to clear the symbol in the actual place x,y
void clear_robot(void){
		//GUI_Text((step*x), (50+(step*y)), (uint8_t*) " ", White, White);
		switch(dir){
		case 'E':
			//GUI_Text((step*x), (50+(step*y)), (uint8_t*) ">", Black, White);
			LCD_DrawTriangle((step*x), (50+(step*y)), 14, 'e', White);
			break;
		case 'N':
			//GUI_Text((step*x), (50+(step*y)), (uint8_t*) "^", Black, White);
			LCD_DrawTriangle((step*x), (50+14+(step*y)), 14, 'n', White);
			break;
		case 'S':
			//GUI_Text((step*x), (50+(step*y)), (uint8_t*) "v", Black, White);
			LCD_DrawTriangle((step*x), (50+(step*y)), 14, 's', White);
			break;
		case 'W':
			//GUI_Text((step*x), (50+(step*y)), (uint8_t*) "<", Black, White);
			LCD_DrawTriangle((14+(step*x)), (50+(step*y)), 14, 'w', White);
			break;
		default:
		break;
	}
}
		
//move the robot in the new position
void move(void){
	switch(dir){ //based on direction
		case 'E':
			if(((x+1)<15) && (lab[y][x+1]!=1)){ //check for the end of the labyrinth and if the next step is an oblstacle
				clear_robot(); //remove the previous robot from the display
				x++; //update coordinate
				draw_robot(); //re-draw the robot in the new position
				look(); //check for obstacles in front of the robot
				win_check(); //check if the player is on an exit
			}
			break;
		case 'N':
			if(((y-1)>-1) && (lab[y-1][x]!=1)){ //check for the end of the labyrinth and if the next step is an oblstacle
				clear_robot();
				y--;
				draw_robot();
				look();
				win_check();
			}
			break;
		case 'S':
			if(((y+1)<13) && (lab[y+1][x]!=1)){ //check for the end of the labyrinth and if the next step is an oblstacle
				clear_robot();
				y++;
				draw_robot();
				look();
				win_check();
			}
			break;
		case 'W':
			if(((x-1)>-1) && (lab[y][x-1]!=1)){ //check for the end of the labyrinth and if the next step is an oblstacle
				clear_robot();
				x--;
				draw_robot();
				look();
				win_check();
			}
			break;
		default:
		break;
	}
}

//check for obstacles in front of the robot
void look(void){
	int d;
	int obs=0;
	switch(dir){ //based on the actual direction
		case 'E':
			for(d=1; d<=6; d++){ //check in the front of the robot step-by-step (up to 5 slot of distance from the obstacle)
				if(x+d<15){ //if not out of the matrix
					if(lab[y][x+d]==1){ //if there is an obstacle, stop the search
						obs=1;
						break;
					}
				}
				else{ //if out of the matrix stop the search
					break;
				}
			}
			if(obs==1){ //if an obstacle has been detected, draw it
					GUI_Text((step*(x+d)),(50+(step*y)), (uint8_t *) "  ", White, Red);
			}
			break;
		case 'S':
			for(d=1; d<=6; d++){
				if(y+d<13){
					if(lab[y+d][x]==1){
						obs=1;
						break;
					}
				}
				else{
					break;
				}
			}
			if(obs==1){
					GUI_Text((step*(x)),(50+(step*(y+d))), (uint8_t *) "  ", White, Red);
			}
			break;
		case 'W':
			for(d=1; d<=6; d++){
				if(x-d>-1){
					if(lab[y][x-d]==1){
						obs=1;
						break;
					}
				}
				else{
					break;
				}
			}
			if(obs==1){
					GUI_Text((step*(x-d)),(50+(step*y)), (uint8_t *) "  ", White, Red);
			}
			break;
		case 'N':
			for(d=1; d<=6; d++){
				if(y-d>-1){
					if(lab[y-d][x]==1){
						obs=1;
						break;
					}
				}
				else{
					break;
				}
			}
			if(obs==1){
					GUI_Text((step*(x)),(50+(step*(y-d))), (uint8_t *) "  ", White, Red);
			}
			break;
	}
	
}

//perfrom the win check
void win_check(void){
	int i;
	if(lab[y][x]==2){ //if the player is on a tile identified with 2 -> exit and win
		for(i=49; i<258; i++){ //color the entire labyrinth area of green
			LCD_DrawLine(0, i, 240, i, Green);
		}
		//messages for the player
		GUI_Text(0, 25, (uint8_t *) "           GAME STOP          ", Red, Green);
		GUI_Text(0, 106, (uint8_t *) "           YOU WIN!        ", Red, Green);
		win=1;
	}
}

//clear the area from obstacles
void clear_button(void){
	int i;
	for(i=49; i<258; i++){ //re-draw the white labyrinth area
		LCD_DrawLine(0, i, 240, i, White);
	}
	draw_robot(); //re-draw the robot in its last position and direction
	look(); //look for obstacles in front of the robot
}

//initilize a new game, used at boot and every time restart is touched
void restart_button(void){
	int i;
	
	LCD_Clear(Blue2); //clear the display
	
	//messages
	GUI_Text(0, 0, (uint8_t *) "        BLIND LABYRINTH       ", Yellow, Blue2);
	GUI_Text(0, 25, (uint8_t *) "           GAME STOP          ", Red, White);
		
	//draw the labyrinth area
	for(i=49; i<258; i++){
			LCD_DrawLine(0, i, 240, i, White);
	}
	
	//draw the buttons
	GUI_Text(0, 49, (uint8_t *) "TOUCH HERE TO START A NEW GAME", Blue2, White);
	GUI_Text(0, 280, (uint8_t *) "       RESTART", Blue2, White);
	GUI_Text(120, 280, (uint8_t *) "CLEAR          ", Black, Yellow);
	
	//bring the robot in the initial position and direction
	x=7;
	y=7;
	dir='E';
	
	//refresh state variable
	started=0;
	win=0;
}

//start a new game
void play(void){
	started=1; //update state variable
	GUI_Text(0, 25, (uint8_t *) "           MOVE MODE          ", Green, White); //mode message 
	GUI_Text(0, 49, (uint8_t *) "                               ", Blue2, White); //remove the start message
	draw_robot(); //show the robot on the screen
	look(); //look for obstacle in front of the robot
}
