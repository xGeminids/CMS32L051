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
#include "stdio.h"
#include "CMS32L051.h"
#include "userdefine.h"
#include "gpio.h"
#include "key.h"
#include "rtc.h"
#include "clk.h"
#include "sci.h"

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
	MD_STATUS status;
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
	GPIO_Output_Enable(&PORT->P12, 0x07);	// P120, P121/X1, P122/X2 output low level
	GPIO_PullUp_Enable(&PORT->P13, 0xFF);
	GPIO_PullUp_Enable(&PORT->P14, 0xFF);
	
//-----------------------------------------------------------------------
// Systick setting 
//-----------------------------------------------------------------------   
	SystemCoreClockUpdate();
	msCnt = SystemCoreClock / 1000;
	SysTick_Config(msCnt);

//-----------------------------------------------------------------------
// PCLBUZ0 output 
//-----------------------------------------------------------------------  
	CLKBUZ0_PORT_SETTING();
	PCBZ->CKS0  = 0x83;    	// FMAIN/2^3
	
//-----------------------------------------------------------------------
// Enable KR0 falling edge interrupt request to wakeup system
//-----------------------------------------------------------------------   
	KEY_Init(1 << 0);                       // Initializes P70/KR0
	KEY_Start();                            // Enable KEY Interrupt
	
//-----------------------------------------------------------------------
// LED ON (500ms) --> LED OFF
//----------------------------------------------------------------------- 
	GPIO_Output_Enable(&PORT->P7, 3 << 1);  // P71, P72 output enable
	GPIO_Set_Value(&PORT->P7, 0x00);        // LED OFF: P30, P31 output low Level
	delayMS(500);                           // LED ON: 500ms for reconnect debugger
	GPIO_Set_Value(&PORT->P7, 0x06);        // LED OFF: P71, P72 output High Level	
	
//-----------------------------------------------------------------------
// RTC Constant Period Interrupt Demo (1 second)
//-----------------------------------------------------------------------
	CLK_SubOsc_Setting(OSC_OSCILLATOR, OSC_ULTRA_LOW_POWER); 		/* SubOSC enable */
	//CLK_SubOsc_Setting(OSC_OSCILLATOR, OSC_LOW_POWER); 		/* SubOSC enable */
	//CLK_SubOsc_Setting(OSC_OSCILLATOR, OSC_NORMAL_POWER); 	/* SubOSC enable */
	RTC_Init(RTC_FSUB);
	CLK_Fclk_Select(MAINCLK_FSUB);
	CLK_Hoco_Stop();
	RTC_Start();
	date_time.year  = 0x19;
	date_time.month = 0x08;
	date_time.week  = SATURDAY; 
	date_time.day   = 0x31;
	date_time.hour  = 0x08;
	date_time.min   = 0x30;
	date_time.sec   = 0x00;
	RTC_Set_CounterValue(&date_time);
	//RTC_Set_ConstPeriodInterruptOn(ONESEC);  // interrupt every second 

//-----------------------------------------------------------------------  
// Systick setting
//-----------------------------------------------------------------------
	SysTick->CTRL  &= ~SysTick_CTRL_ENABLE_Msk;  /* Disable SysTick */
	
//-----------------------------------------------------------------------
// Sleep 4 times
// P70/KR0 falling edge wakeup system and blinky P72 LED
//-----------------------------------------------------------------------
	for(i=4; i>0; )
	{
		__WFI(); 			// Sleep
		if(0U == (PORT->P7 & 0x01)) // check P70 value is 0
		{
			PORT->P7 ^= (1 << 2); 	// P72 toggle 
			i--;
		}
	}

//-----------------------------------------------------------------------  
// Systick setting
//----------------------------------------------------------------------- 
	CLK_Hoco_Start();
	CLK_Fclk_Select(MAINCLK_FIH);  
	SysTick->CTRL  |= SysTick_CTRL_ENABLE_Msk;  /* Enable SysTick */
	
//-----------------------------------------------------------------------
// LED blinky on target board 
// P71, P72 drives LED on board
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

