/***********************************************************************************************************************
* Copyright (C) . All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : main.c
* Version      :  
* Device(s)    : CMS32L051
* Tool-Chain   : MDK(armcc)
* Description  : This file is a template.
* Creation Date: 2019/12/25
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro Definitions
***********************************************************************************************************************/
	
/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include <stdio.h>
#include "CMS32L051.h"
#include "userdefine.h"
#include "clk.h"
#include "gpio.h"
#include "key.h"
#include "rtc.h"
#include "sci.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern rtc_counter_value_t date_time;
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
// Setting wakeup time from deepsleep 
//----------------------------------------------------------------------- 	
	CNTLD = (0xF1 << 8) | 0x40; // key = 0xF1, value = 0x40
	
//-----------------------------------------------------------------------
// Init UART0 for retarget printf/scanf etc. 
//----------------------------------------------------------------------- 
	SystemCoreClockUpdate();
	status = UART0_Init(SystemCoreClock, 19200);
	if(status == MD_ERROR)
	{
		while(1); // The baud rate cannot get at the current system clock frequency.
	}
	
	printf("CNTLD = 0x%02X\n", CNTLD);
	
//-----------------------------------------------------------------------
// PCLBUZ0 
//-----------------------------------------------------------------------
	CLKBUZ0_PORT_SETTING();
	PCBZ->CKS0  = 0x83;    	// FMAIN/2^3	
	
//-----------------------------------------------------------------------
// Enable KR0 falling edge interrupt request to wakeup system
//-----------------------------------------------------------------------   
	KEY_Init(1 << 0);               // Initializes P70/KR0
	KEY_Start();                    // Enable KEY Interrupt
	__disable_irq(); 	/* don't entry ISR when wake up by intp0 */
	
//-----------------------------------------------------------------------
// RTC Constant Period Interrupt Demo (1 second)
//-----------------------------------------------------------------------
	//CLK_Osc_Setting(OSC_PORT, OSC_OSCILLATOR); /* SubOSC enable */
  RTC_Init(RTC_FIL);
  RTC_Start();
  date_time.year  = 0x19;
  date_time.month = 0x06;
  date_time.week  = SATURDAY; 
  date_time.day   = 0x01;
  date_time.hour  = 0x08;
  date_time.min   = 0x30;
  date_time.sec   = 0x00;
  RTC_Set_CounterValue(&date_time);
  //RTC_Set_ConstPeriodInterruptOn(ONESEC);
	RTC_1HZ_Output();

//-----------------------------------------------------------------------
// P72: Wakeup Flag
//-----------------------------------------------------------------------	
	GPIO_Output_Enable(&PORT->P7, 1<<2);	
	
//-----------------------------------------------------------------------
// wait a moment
//-----------------------------------------------------------------------	
	while(1U)
	{		
	//-----------------------------------------------------------------------
	// Stop Entry
	//-----------------------------------------------------------------------	
		__STOP();
		
	//-----------------------------------------------------------------------
	// Stop Restore
	//-----------------------------------------------------------------------	
		PORT->P7 ^= (1<<2); /* P72 toggle */
		
		INTC_ClearPendingIRQ(RTC_IRQn);
		NVIC_ClearPendingIRQ(RTC_IRQn);		
		INTC_ClearPendingIRQ(KEY_IRQn);
		NVIC_ClearPendingIRQ(KEY_IRQn);
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
