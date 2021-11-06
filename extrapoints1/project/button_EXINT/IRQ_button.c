#include "button.h"
#include "lpc17xx.h"
#include "../blind_labyrinth/blind_labyrinth.h"
#include "../RIT/RIT.h"		  			/* you now need RIT library 			 */
#include "../timer/timer.h"
extern char dir;
extern char x;
extern char y;
extern int started;
extern int reset;
extern int d;
extern int green;

void EINT0_IRQHandler (void)	  	/* INT0	- start											 */
{		
	if(started==0 && reset==1){		//reset + game stopped -> if int0 is pressed the game starts and the reset status is removed
		started=1;
		reset=0;
		NVIC_DisableIRQ(EINT0_IRQn);		/* disable Button interrupts			 */
		leds();
	}
	LPC_PINCON->PINSEL4    &= ~(1 << 20);     /* GPIO pin selection */
	LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */

}


void EINT1_IRQHandler (void)	  	/* KEY1	- rotate											 */
{
	if(started==1){
		enable_RIT();										/* enable RIT to count 50ms				 */
		NVIC_DisableIRQ(EINT1_IRQn);		/* disable Button interrupts			 */
		LPC_PINCON->PINSEL4    &= ~(1 << 22);     /* GPIO pin selection */
	
		LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
	}
}

void EINT2_IRQHandler (void)	  	/* KEY2	- run											 */
{
	if(started==1 && (d>1 || green==1)){ //if the game is running and it is possible to perform at least one step
		reset_timer(1);
		init_timer(1, (25000000 * 0.5)); //run led, 1Hz, 0.5s high, 0.5s low
		enable_timer(1);
		enable_timer(2);
		NVIC_DisableIRQ(EINT2_IRQn);		/* disable Button interrupts			 */
		LPC_PINCON->PINSEL4    &= ~(1 << 24);     /* GPIO pin selection */
		LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */	
	}		
}


