/***********************************************************************************************************************
* Copyright (C) . All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : main.c
* Version      :  
* Device(s)    : CMS32L051
* Tool-Chain   : MDK(armcc)
* Description  : This file is a template.
* Creation Date: 2019/4/30
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro Definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "CMS32L051.h"
#include "userdefine.h"
#include "gpio.h"
#include "key.h"
#include "intp.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
volatile uint32_t g_ticks;

void delayMS(uint32_t n)
{
		g_ticks = n;
		while(g_ticks);
}

int main()
{
	uint32_t msCnt; 	// count value of 1ms
	uint32_t i;

//-----------------------------------------------------------------------
// Initialize the ports to prevent the leakage current 
//-----------------------------------------------------------------------
	GPIO_PullUp_Enable(&PORT->P0, 0xFF);
	GPIO_PullUp_Enable(&PORT->P1, 0xFF);
	GPIO_PullUp_Enable(&PORT->P2, 0xFF);
	GPIO_PullUp_Enable(&PORT->P3, 0xFF);
	GPIO_PullUp_Enable(&PORT->P4, 0xFF);
	GPIO_PullUp_Enable(&PORT->P5, 0xFF);
	GPIO_PullUp_Enable(&PORT->P6, 0xFF);
	GPIO_PullUp_Enable(&PORT->P7, 0xFF);
	GPIO_Output_Enable(&PORT->P12, 0x1F);	// P120, P121/X1, P122/X2, P123/XT1, P124/XT2 output low level
	GPIO_PullUp_Enable(&PORT->P13, 0xFF);
	GPIO_PullUp_Enable(&PORT->P14, 0xFF);
	
//-----------------------------------------------------------------------
// P140/PCLBUZ0 output 
//-----------------------------------------------------------------------  
	CLKBUZ0_PORT_SETTING();
	PCBZ->CKS0  = 0x83;    	// FMAIN/2^3	
	
//-----------------------------------------------------------------------
// Systick setting 
//-----------------------------------------------------------------------   
	g_ticks = 1000; 	// 1000ms
	SystemCoreClockUpdate();
	msCnt = SystemCoreClock / 1000;
	SysTick_Config(msCnt);
	
//-----------------------------------------------------------------------
// Enable KR0 falling edge interrupt request to wakeup system
//-----------------------------------------------------------------------   
	KEY_Init(1 << 0);               // Initializes P70/KR0
	KEY_Start();                    // Enable KEY Interrupt
	
//-----------------------------------------------------------------------
// Enable INTP3 falling edge interrupt request to wakeup system
//-----------------------------------------------------------------------   
	INTP_Init(1 << 3, INTP_BOTH);   // Initializes P30/INTP3
	INTP_Start(1<<3);               // Enable INTP3 Interrupt
	
//-----------------------------------------------------------------------
// LED ON (500ms) --> LED OFF
//----------------------------------------------------------------------- 
	GPIO_Output_Enable(&PORT->P7, 3 << 1);   // P71, P72 output enable
	GPIO_Set_Value(&PORT->P7, 0x00);         // LED ON: P71, P72 output low Level
	delayMS(500);                            // LED ON: 500ms for reconnect debugger
	GPIO_Set_Value(&PORT->P7, 0x06);         // LED OFF: P71, P72 output High Level
	
//-----------------------------------------------------------------------
// DeepSleep 10 times
// P70/KR0 falling edge wakeup system and blinky P71 LED
//-----------------------------------------------------------------------

	for(i=10; i>0; )
	{
		/*** enter power down ***/
		CGC->PMUKEY = 0x192A;
		CGC->PMUKEY = 0x3E4F;
		CGC->PMUCTL = 1;

		/* disable LVR */
		LVD->LVIM = 0x80;//disable LVR,option byte 0xC1 select interrupt&reset mode
		__STOP(); 		// DeepSleep
		/* enable LVR */
		LVD->LVIM = 0x00;
		
		delayMS(100); 	// Filter out key jitter
        PORT->P7 ^= (1 << 1); 	// P71 toggle 
        i--;
	}

//-----------------------------------------------------------------------
// LED blinky on target board 
// P71, 72 drives LED on board
//----------------------------------------------------------------------- 
	// LED Blinky
	PORT->P7 = 0x02U;  
	while(1)
	{
		delayMS(250);
		PORT->P7 ^= (3<<1); 	// Toggle P71, P72
	}
}

/***********************************************************************************************************************
* Function Name: SysTick Handler
* Description  : Decreament the g_ticks value
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void SysTick_Handler(void)
{
	g_ticks--;
}

