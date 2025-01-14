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
#include "CMS32L051.h"
#include "userdefine.h"
#include "rtc.h"

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
	
//-----------------------------------------------------------------------
// Vector Table in SRAM region
//-----------------------------------------------------------------------
#if 1
	__disable_irq();
	SCB->VTOR = 0x20000200;
	__DMB();	
	__enable_irq();
#endif
	
//-----------------------------------------------------------------------
// 15bits IntervalTimer (Operation Clock: fIL)
//-----------------------------------------------------------------------
	PORT->PMC7 &= ~(3<<1);
	PORT->P7   = 0x02U;
	PORT->PM7 &= ~(3<<1);
	IT_Init(RTC_FIL, 3750);
	IT_Start();

	while(1)
	{	
		__WFI();  	/* Sleep */
		PORT->P7 ^= 3<<1; 	// Toggle P71, P72
	}
	
	IT_Stop();

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


