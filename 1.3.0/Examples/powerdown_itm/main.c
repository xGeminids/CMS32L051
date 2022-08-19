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
#include <string.h>
#include "CMS32L051.h"
#include "userdefine.h"
#include "sci.h"
#include "clk.h"
#include "rtc.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
volatile uint32_t g_ticks;

int main()
{
	MD_STATUS status;
	uint32_t i;
	
//-----------------------------------------------------------------------
// Init UART0 for retarget printf/scanf etc. 
//----------------------------------------------------------------------- 
#if 1
	SystemCoreClockUpdate();
	status = UART0_Init(SystemCoreClock, 19200);
	if(status == MD_ERROR)
	{
		while(1);
	}
	
	printf("Hello, UART\n");
#endif
//-----------------------------------------------------------------------
// PCLBUZ0 output 
//----------------------------------------------------------------------- 
	//PCBZ_Init(PCBZ_CHANNEL_0, PCBZ_FMAIN_3);
	CLKBUZ0_PORT_SETTING(); // P140/CLKBUZ0
	PCBZ->CKS0  = 0x83;    	// FMAIN/2^3

//-----------------------------------------------------------------------
// PCLBUZ1 output 
//-----------------------------------------------------------------------
	//PCBZ_Init(PCBZ_CHANNEL_1, PCBZ_FSUB_0);
	CLKBUZ1_PORT_SETTING(); // P15/CLKBUZ1
	PCBZ->CKS1  = 0x88;    	// FSUB


//-----------------------------------------------------------------------
// 15bits IntervalTimer (Operation Clock: fIL)
//-----------------------------------------------------------------------
//	printf("IntervalTimer operates with fIL\n");
//	IT_Init(RTC_FIL, 1000);
//	IT_Start();

//	for(i=0; i<8; i++)
//	{	
//		__STOP();  	/* Deep Sleep */
//		printf("Wake up from deep sleep by 15bit IntervalTimer interrupt: No.%02d\n", g_ItIntTaken);
//	}
//	
//	IT_Stop();

//-----------------------------------------------------------------------
// 15bits IntervalTimer (Operation Clock: fSUB)
//-----------------------------------------------------------------------	
	printf("IntervalTimer operates with fSUB\n");
	/* SubOSC ON */
	CLK_SubOsc_Setting(OSC_EXCLK, OSC_LOW_POWER);

	//CLK_SubOsc_Setting(OSC_OSCILLATOR, OSC_LOW_POWER);
	IT_Init(RTC_FIL, 100);
	IT_Start();

	for(i=0; i<8; i++)
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
				
        SystemCoreClockUpdate();
        status = UART0_Init(SystemCoreClock, 19200);
        if(status == MD_ERROR)
        {
            while(1);
        }
        printf("Wake up from power-down deep sleep by 15bit IntervalTimer interrupt: No.%02d\n", g_ItIntTaken);
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

