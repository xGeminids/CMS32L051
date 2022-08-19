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
// MainOSC and SubOSC enable
//-----------------------------------------------------------------------
	CLK_Osc_Setting(OSC_OSCILLATOR, OSC_OSCILLATOR);
	//CLK_SubOsc_Setting(OSC_OSCILLATOR, OSC_LOW_POWER);
	
//-----------------------------------------------------------------------
// P72/PCLBUZ0 output 
// P15/PCLBUZ1 output
//-----------------------------------------------------------------------  
	CLKBUZ0_PORT_SETTING();
	PCBZ->CKS0  = 0x83;    	// FMAIN/2^3
	CLKBUZ1_PORT_SETTING();
	PCBZ->CKS1  = 0x88;    	// FSUB	

#if 1	
//-----------------------------------------------------------------------
// Select MainOSC as system clock
//-----------------------------------------------------------------------
	printf("Select MainOSC as system clock\n");
	CLK_Fclk_Select(MAINCLK_FMX);
	while((CGC->CKC & CGC_CKC_MCS_Msk) == 0);

//-----------------------------------------------------------------------
// Select HOCO as system clock
//-----------------------------------------------------------------------
	CLK_Fclk_Select(MAINCLK_FIH);
	while(CGC->CKC & CGC_CKC_MCS_Msk);	
	printf("Select HOCO as system clock\n");
#endif	
//-----------------------------------------------------------------------
// Select SubOSC as system clock
//-----------------------------------------------------------------------
	printf("Select SubOSC as system clock\n");
	CLK_Fclk_Select(MAINCLK_FSUB);
	while((CGC->CKC & CGC_CKC_CLS_Msk) == 0);		
	
//-----------------------------------------------------------------------
// Select HOCO as system clock
//-----------------------------------------------------------------------
	CLK_Fclk_Select(MAINCLK_FIH);
	while(CGC->CKC & CGC_CKC_CLS_Msk);	
	printf("Select HOCO as system clock\n");
	
//-----------------------------------------------------------------------
// Select LOCO as system clock
//-----------------------------------------------------------------------
	printf("Select LOCO as system clock\n");
	CLK_Fclk_Select(MAINCLK_FIL);
	while((CGC->CKC & CGC_CKC_CLS_Msk) == 0);			
	
//-----------------------------------------------------------------------
// Select HOCO as system clock
//-----------------------------------------------------------------------
	CLK_Fclk_Select(MAINCLK_FIH);
	while(CGC->CKC & CGC_CKC_CLS_Msk);
	printf("Select HOCO as system clock\n");

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

