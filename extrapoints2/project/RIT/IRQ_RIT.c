/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../led/led.h"
#include "../blind_labyrinth/blind_labyrinth.h"
#include "../GLCD/GLCD.H"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
extern char mode;
extern char dir;
extern int win;
int select=0;
int up=0;
int down=0;
int left=0;
int right=0;
extern int started;

void RIT_IRQHandler (void)
{		
		if(started==1 && win==0){
			int mask = 0xFF000000;
			int val = (LPC_GPIO1->FIOPIN) & mask;
			
			//------------------------------------------------------------------select
			if(val==0xFD000000){	
			select++;
			switch(select){
				case 1: //at the first pressure, check the actual mode and change it
					if(mode=='m'){
						mode='e';
							GUI_Text(0, 25, (uint8_t *) "         EXPLORE MODE         ", Red, White);

					}
					else{
						mode='m';
							GUI_Text(0, 25, (uint8_t *) "           MOVE MODE          ", Green, White);
					}
					break;
				default:
					break;
			}
		}
		else{
				select=0; //button released
		}
		
		//---------------------------------------------------------------------down
		if(val==0xFB000000){	
			down++;
			if(down==1){ //at the first pressure, change the robot direction immediately
					clear_robot(); //remove the previous robot
					dir='S'; //update the direction
					draw_robot(); //re-draw the updated robot
					look(); //look for obstacles in front of the robot
			}
			if((down%20)==0){ //if the button has been pressed for 1 second
				if(mode=='m'){ //and if the game is in move mode
					move(); //perform the move action (out-of bound and on-obstacles checks are performed by the move function
				}
			}

		}
		else{
			down=0; //button released
		}
		
		//----------------------------------------------------------------------left
		if(val==0xF7000000){	
			left++;
			if(left==1){
					clear_robot();
					dir='W';
					draw_robot();
					look();
					
			}
			if((left%20)==0){
				if(mode=='m'){
					move();
				}
			}
		}
		else{
			left=0;
		}
		
		//-------------------------------------------------------------------------right
		if(val==0xEF000000){	
			right++;
			if(right==1){
					clear_robot();
					dir='E';
					draw_robot();
					look();
			}
			if((right%20)==0){
				if(mode=='m'){
					move();
				}
			}
		}
		else{
			right=0;
		}
		
		//---------------------------------------------------------------------------up
		if(val==0xDF000000 ){	
			up++;
			if(up==1){
					clear_robot();
					dir='N';
					draw_robot();
					look();
			}
			if((up%20)==0){
				if(mode=='m'){
					move();
				}
			}
		}
		else{
			up=0;
		}
	}
	LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
}

/******************************************************************************
**                            End Of File
******************************************************************************/
