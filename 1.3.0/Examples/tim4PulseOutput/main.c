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
#include <stdio.h>
#include "CMS32L051.h"
#include "userdefine.h"
#include "sci.h"
#include "tim4.h"

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
// Pause timer count when program stops during debugging
//-----------------------------------------------------------------------	 	 
	DBG->DBGSTOPCR |= DBG_DBGSTOPCR_FRZEN0_Msk;		
	 
//-----------------------------------------------------------------------
// Systick setting 
//-----------------------------------------------------------------------   
	g_ticks = 1000; 	// 1000ms
	SystemCoreClockUpdate();
	msCnt = SystemCoreClock / 1000;
	SysTick_Config(msCnt/100); 
	 
//-----------------------------------------------------------------------
// Init UART0 for retarget printf/scanf etc. 
//----------------------------------------------------------------------- 
#if 1
	SystemCoreClockUpdate();
	status = UART0_Init(SystemCoreClock, 19200);
	if(status == MD_ERROR)
	{
		while(1); // The baud rate cannot get at the current system clock frequency.
	}

#endif

//-----------------------------------------------------------------------
// P01/TO00 output square wave
//-----------------------------------------------------------------------	
	/* square wave (duty 50%)
	 *       _____       _____       _____       _____       
	 * TO00:      |_____|     |_____|     |_____|     |_____ 
	 */
	TM40_SquareOutput(TM4_CHANNEL_0, 100);
	delayMS(10);
	TM40_Channel_Stop(TM4_CHANNEL_0 | TM4_CHANNEL_1 | TM4_CHANNEL_2 | TM4_CHANNEL_3);
	
//-----------------------------------------------------------------------
// P16/TO01 output pwm wave
//-----------------------------------------------------------------------
	/* PWM wave (duty 40%)
	 *       ____        ____        ____        ____        
	 * TO01:     |______|    |______|    |______|    |______ 
	 */
	TM40_PWM_1Period_1Duty(100, 40);	
	delayMS(10);
	TM40_Channel_Stop(TM4_CHANNEL_0 | TM4_CHANNEL_1 | TM4_CHANNEL_2 | TM4_CHANNEL_3);

//-----------------------------------------------------------------------
// P16/TO01 output pwm wave
// P17/TO02 output pwm wave
//-----------------------------------------------------------------------
	/* PWM wave (duty 60%, 80%)
	 *       ______      ______      ______      ______      
	 * TO01:       |____|      |____|      |____|      |____ 
	 *       ________    ________    ________    ________    
	 * TO02:         |__|        |__|        |__|        |__ 
	 */
	TM40_PWM_1Period_2Duty(100, 60, 80);	
	delayMS(10);
	TM40_Channel_Stop(TM4_CHANNEL_0 | TM4_CHANNEL_1 | TM4_CHANNEL_2 | TM4_CHANNEL_3);

//-----------------------------------------------------------------------
// P16/TO01 output pwm wave
// P31/TO03 output pwm wave
//-----------------------------------------------------------------------
	/* PWM wave (duty 60%, 40%)
	 *       ______      ______      ______      ______      
	 * TO01:       |____|      |____|      |____|      |____ 
	 *       _________               _________                
	 * TO03:          |_____________|         |_____________ 
	 */
	TM40_PWM_2Period_2Duty(100, 60, 200, 80);	
	delayMS(10);
	TM40_Channel_Stop(TM4_CHANNEL_0 | TM4_CHANNEL_1 | TM4_CHANNEL_2 | TM4_CHANNEL_3);

//-----------------------------------------------------------------------
// P16/TO01 output pwm wave
// P17/TO02 output pwm wave
// P31/TO03 output pwm wave
//-----------------------------------------------------------------------
	/* PWM wave (duty 20%, 50%, 90%)
	 *       __          __          __          __           
	 * TO01:   |________|  |________|  |________|  |________ 
	 *       _____       _____       _____       _____       
	 * TO02:      |_____|     |_____|     |_____|     |_____ 
	 *       _________   _________   _________   _________   
	 * TO03:          |_|         |_|         |_|         |_ 
	 */
	TM40_PWM_1Period_3Duty(100, 20, 50, 90);
	delayMS(10);
	//TM40_Channel_Stop(TM4_CHANNEL_0 | TM4_CHANNEL_1 | TM4_CHANNEL_2 | TM4_CHANNEL_3);

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

/***********************************************************************************************************************
* Function Name: HardFault_Handler
* Description  : Hard Fault handler to report stacked register values
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
// Hard Fault handler in C, with stack frame location and LR value
// extracted from the assembly wrapper as input parameters
void hard_fault_handler_c(unsigned int * hardfault_args, unsigned lr_value)
{
	unsigned int stacked_r0;
	unsigned int stacked_r1;
	unsigned int stacked_r2;
	unsigned int stacked_r3;
	unsigned int stacked_r12;
	unsigned int stacked_lr;
	unsigned int stacked_pc;
	unsigned int stacked_psr;
	
	stacked_r0 = ((unsigned long) hardfault_args[0]);
	stacked_r1 = ((unsigned long) hardfault_args[1]);
	stacked_r2 = ((unsigned long) hardfault_args[2]);
	stacked_r3 = ((unsigned long) hardfault_args[3]);
	stacked_r12 = ((unsigned long) hardfault_args[4]);
	stacked_lr = ((unsigned long) hardfault_args[5]);
	stacked_pc = ((unsigned long) hardfault_args[6]);
	stacked_psr = ((unsigned long) hardfault_args[7]);
	
	printf ("[Hard fault handler]\r\n");
	printf ("R0 = %x\r\n", stacked_r0);
	printf ("R1 = %x\r\n", stacked_r1);
	printf ("R2 = %x\r\n", stacked_r2);
	printf ("R3 = %x\r\n", stacked_r3);
	printf ("R12 = %x\r\n", stacked_r12);
	printf ("Stacked LR = %x\r\n", stacked_lr);
	printf ("Stacked PC = %x\r\n", stacked_pc);
	printf ("Stacked PSR = %x\r\n", stacked_psr);
	printf ("Current LR = %x\r\n", lr_value);
	
	while(1); // endless loop
}

/***********************************************************************************************************************
* Function Name: HardFault_Handler
* Description  : Assembly wrapper using Embedded Assembler in Keil MDK
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
// Hard Fault handler wrapper in assembly
// It extracts the location of stack frame and passes it to handler
// in C as a pointer. We also extract the LR value as second
// parameter.
__asm void HardFault_Handler(void)
{
		MOVS 	r0, #4
		MOV 	r1, LR
		TST 	r0, r1
		BEQ 	stacking_used_MSP
		MRS 	R0, PSP ; first parameter - stacking was using PSP
		B 		get_LR_and_branch
stacking_used_MSP
		MRS 	R0, MSP ; first parameter - stacking was using MSP
get_LR_and_branch
		MOV 	R1, LR ; second parameter is LR current value
		LDR 	R2,=__cpp(hard_fault_handler_c)
		BX 		R2
}

