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
#include "dma.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
volatile uint32_t g_ticks;
uint8_t User_tx_buf[20] = { 'H', 'e', 'l', 'l', 'o', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'}; 
uint8_t User_rx_buf[100];

void delayMS(uint32_t n)
{
    g_ticks = n;
    while(g_ticks);
}

int main()
{
	MD_STATUS status;
	uint32_t msCnt;                     // count value of 1ms
	
//-----------------------------------------------------------------------
// Systick setting 
//-----------------------------------------------------------------------   
	SystemCoreClockUpdate();
	msCnt = SystemCoreClock / 1000;
	SysTick_Config(1*msCnt); 
	delayMS(200);                       // delay 200ms
	
	SystemCoreClockUpdate();

	PORT->PMC7 &= ~(3<<1); // P71, P72 Digital function
	PORT->P7    = 0x02U;  
	PORT->PM7  &= ~(3<<1); // P71, P72 output mode
    
	status = UART0_Init(SystemCoreClock, 115200);
	if(status == MD_ERROR)
	{
		while(1); // The baud rate cannot get at the current system clock frequency.
	}
    
	status = UART1_Init(SystemCoreClock, 115200);
	if(status == MD_ERROR)
	{
		while(1); // The baud rate cannot get at the current system clock frequency.
	}
    
    status = UART2_Init(SystemCoreClock, 115200);
	if(status == MD_ERROR)
	{
		while(1); // The baud rate cannot get at the current system clock frequency.
	}
//    printf("UART send and receive use dma \n\r");

//    UART0_SendChar(0x55);
    UART2_Send(User_tx_buf, 12);
    delayMS(100);
    UART2_Send(User_tx_buf, 12);
//    delayMS(100);
//    UART0_Receive(User_rx_buf, 5);

	while(1)
	{
        
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
//	unsigned int stacked_r0;
//	unsigned int stacked_r1;
//	unsigned int stacked_r2;
//	unsigned int stacked_r3;
//	unsigned int stacked_r12;
//	unsigned int stacked_lr;
//	unsigned int stacked_pc;
//	unsigned int stacked_psr;
//	
//	stacked_r0 = ((unsigned long) hardfault_args[0]);
//	stacked_r1 = ((unsigned long) hardfault_args[1]);
//	stacked_r2 = ((unsigned long) hardfault_args[2]);
//	stacked_r3 = ((unsigned long) hardfault_args[3]);
//	stacked_r12 = ((unsigned long) hardfault_args[4]);
//	stacked_lr = ((unsigned long) hardfault_args[5]);
//	stacked_pc = ((unsigned long) hardfault_args[6]);
//	stacked_psr = ((unsigned long) hardfault_args[7]);

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

