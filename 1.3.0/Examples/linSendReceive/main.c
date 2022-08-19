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
//#define RTL_SIMULATION

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include <stdio.h>
#include "lin.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
volatile uint32_t g_ticks;
uint32_t bit_width;
lin_frame_t txFrame, rxFrame;

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
	uint32_t width, baudrate;

//-----------------------------------------------------------------------
// Init UART0 for retarget printf/scanf etc. 
//----------------------------------------------------------------------- 
//#ifndef RTL_SIMULATION
//	SystemCoreClockUpdate();
//	status = UART0_Init(SystemCoreClock, 19200);
//	if(status == MD_ERROR)
//	{
//		while(1); // The baud rate cannot get at the current system clock frequency.
//	}

//	printf("Hello, UART\n");
//#endif

//-----------------------------------------------------------------------
// Systick setting 
//-----------------------------------------------------------------------
	g_ticks = 1000; 	// 1000ms
	SystemCoreClockUpdate();
	msCnt = SystemCoreClock / 1000;
	SysTick_Config(msCnt);

//-----------------------------------------------------------------------
// LIN Send and Receive 
// Please connect P51/TXD0 to P50/RXD0 on board
//-----------------------------------------------------------------------
	bit_width = SystemCoreClock / LIN_BAUDRATE;
	txFrame.PID = (uint8_t)0xF0;
	txFrame.Data[0] = (uint8_t)0x00;
	txFrame.Data[1] = (uint8_t)0x01;
	txFrame.Data[2] = (uint8_t)0x02;
	txFrame.Data[3] = (uint8_t)0x03;
	txFrame.Data[4] = (uint8_t)0x10;
	txFrame.Data[5] = (uint8_t)0x20;
	txFrame.Data[6] = (uint8_t)0x30;
	txFrame.Data[7] = (uint8_t)0x40;
	for(i=0; i<8; i++)
	{
			txFrame.ChkSum  ^= txFrame.Data[i];
	}
	LIN_SetConfig();
	while (1)
	{
		LIN_SendWakeUp();  /* send wakeup field */
		if(g_intp0Taken)
		{
			g_intp0Taken = 0;
			LIN_ReceiveBreak();
			LIN_SendBreak();  /* send break field */
			width = LIN_GetBreakWidth();
			if(width > 11 * bit_width)
			{
				LIN_ReceiveSync();
				LIN_SendSync();  /* send sync field */
				baudrate = LIN_GetBaudRate();
				LIN_SetBaudRate(baudrate);
				LIN_Receive((uint8_t *)&rxFrame, sizeof(rxFrame));
			}
			LIN_Send((uint8_t *)&txFrame, sizeof(txFrame));
		}
		delayMS(500);
//		printf("\n");
//		printf("   PID = 0x%02X\n", rxFrame.PID);
//		printf("  Data = ");
//		for(i=0; i<8; i++)
//		{
//				printf("0x%02X, ", rxFrame.Data[i]);
//		}
//		printf("\n");
//		printf("ChkSum = 0x%02X\n", rxFrame.ChkSum);
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

