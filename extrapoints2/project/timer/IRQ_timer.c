/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <string.h>
#include "lpc17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h" 
#include "../TouchPanel/TouchPanel.h"
#include "../blind_labyrinth/blind_labyrinth.h"

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
extern int started;
extern int win;

void TIMER0_IRQHandler (void)
{
	
  getDisplayPoint(&display, Read_Ads7846(), &matrix ) ; //get touch coordinates
	#ifdef SIMULATOR
	if(display.x <= 240 && display.x > 0 && display.y < 0x013D){
	#else
	if(display.x <= 240 && display.x > 0 && display.y != 0){
	#endif
		if(display.y >=45 && display.y <=258 && started==0){ //start game on the lab area, available only if the game is not already started
			play();
		}
		if(display.y >= 0x118 && display.y <= 0x125 && display.x > 0x77 && started==1 && win==0){ //clear, available only when the game has started and the player has not already win
			clear_button();
		}
		if(display.y >= 0x118 && display.y <= 0x125 && display.x < 0x6E){ //restart
			restart_button();
		}
	}
	else{
		//do nothing if touch returns values out of bounds
	}
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
