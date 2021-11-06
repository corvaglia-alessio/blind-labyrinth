/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "timer.h"
#include "../led/led.h"
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
extern char dir;
extern char win;
extern char x;
extern char y;
int count = 0;
extern char lab [13][15];
extern int d;

void TIMER0_IRQHandler (void)
{
	int state;
	if(win==0){
			switch(dir){
				case 'E':
					state = LED_state(2);
					if(state == 0){
						LED_On(2);
					}
					else{
						LED_Off(2);
					}
					break;
				case 'S':
					state = LED_state(1);
					if(state == 0){
						LED_On(1);
					}
					else{
						LED_Off(1);
					}
					break;
				case 'N':
					state = LED_state(3);
					if(state == 0){
						LED_On(3);
					}
					else{
						LED_Off(3);
					}
					break;
				case 'W':
					state = LED_state(0);
					if(state == 0){
						LED_On(0);
					}
					else{
						LED_Off(0);
					}
					break;
			}
	}
	else{
		state = LED_state(7);
		if(state==0){
			LED_On(0);
			LED_On(1);
			LED_On(2);
			LED_On(3);
			LED_On(4);
			LED_On(5);
			LED_On(6);
			LED_On(7);
		}
		else{
			LED_Off(0);
			LED_Off(1);
			LED_Off(2);
			LED_Off(3);
			LED_Off(4);
			LED_Off(5);
			LED_Off(6);
			LED_Off(7);
		}
	}
	LPC_TIM0->IR = 1;			/* clear interrupt flag */
	enable_timer(0);
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
	int state = LED_state(5);
		if(state == 0){
			LED_On(5);
		}
		else{
			LED_Off(5);
		}
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
** Function name:		Timer2_IRQHandler
**
** Descriptions:		Timer/Counter 2 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER2_IRQHandler (void) //when the run button has been pressed for 1sec
{			
	reset_timer(2);
	disable_timer(2);
	if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){ //if the button is still pressed (after 1 second)
		switch(dir){ //check, based on direction, if it is possible to move, and if it is, it moves of one step
			case 'E':
				if(x<14 && (lab[y][x+1] != 1)){
					x++;
				}
			break;
			case 'S':
				if(y<12 && (lab[y+1][x] != 1)){
					y++;
				}
			break;
			case 'W':
				if(x>0 && (lab[y][x-1] != 1)){
					x--;
				}
			break;
			case 'N':
				if(y>0 && (lab[y-1][x] != 1)){
					y--;
				}
			break;
		}
	}
	leds(); //refresh leds
	NVIC_EnableIRQ(EINT2_IRQn);							 /* re-enable Button interrupts			*/
	LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 0 pin selection */
	LPC_TIM2->IR = 1;			/* clear interrupt flag */	
	return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
