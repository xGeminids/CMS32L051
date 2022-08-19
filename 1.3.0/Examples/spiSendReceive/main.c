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
#include "CMS32L051.h"
#include "userdefine.h"
#include "sci.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
volatile uint32_t g_ticks;
uint8_t mtx_buf[] = {0xA5, 0x5A, 0xCC, 0x33, 0x3C, 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
uint8_t stx_buf[] = {0x33, 0xCC, 0xC3, 0x5A, 0xA5, 0x08, 0x04, 0x02, 0x01, 0x80, 0x40, 0x20, 0x10};
uint8_t mrx_buf[13];
uint8_t srx_buf[13];

void delayMS(uint32_t n)
{
		g_ticks = n;
		while(g_ticks);
}

int main()
{
	MD_STATUS status;
	uint32_t msCnt; 	// count value of 1ms
	uint32_t i, err;

//-----------------------------------------------------------------------
// Init UART0 for retarget printf/scanf etc. 
//----------------------------------------------------------------------- 
#ifndef RTL_SIMULATION
	SystemCoreClockUpdate();
	status = UART0_Init(SystemCoreClock, 19200);
	if(status == MD_ERROR)
	{
		while(1); // The baud rate cannot get at the current system clock frequency.
	}

	printf("Hello, UART\n");
#endif

//-----------------------------------------------------------------------
// Systick setting 
//-----------------------------------------------------------------------
	g_ticks = 1000; 	// 1000ms
	SystemCoreClockUpdate();
	msCnt = SystemCoreClock / 1000;
	SysTick_Config(msCnt);

//-----------------------------------------------------------------------
// Please connect the following pins:
// SCK:  P130/SCK10 --> P10/SCK11
// MOSI: P02/SDO10 --> P11/SDI11
// MISO: P03/SDI10 <-- P12/SDO11
//-----------------------------------------------------------------------
#ifndef RTL_SIMULATION
	printf("Please connect P130/SCK10 to P10/SCK11\n");
	printf("Please connect P00/SDO10 to P11/SDI11\n");
	printf("Please connect P01/SDI10 to P12/SDO11\n");
	printf("Press any key to continue\n");
	getchar();
#endif
	printf("\n");
	
	/* Pause SPI transfer when program stops during debugging */
	DBG->DBGSTOPCR |= DBG_DBGSTOPCR_FRZEN1_Msk;

//=======================================================================
// SPI10 MODE 0 Master transmission/reception
// SPI11 MODE 0 Slave reception/transmission
//   Mode 0: CPOL = 0, CPHA = 0; i.e. DAP = 1, CKP = 1
//   Mode 1: CPOL = 0, CPHA = 1; i.e. DAP = 0, CKP = 1
//   Mode 2: CPOL = 1, CPHA = 0; i.e. DAP = 1, CKP = 0
//   Mode 3: CPOL = 1, CPHA = 1; i.e. DAP = 0, CKP = 0
//=======================================================================
	SPI10_MasterInit(SPI_MODE_0);
	SPI11_SlaveInit(SPI_MODE_0);

//-----------------------------------------------------------------------
// Master Send and Slave Receive
//-----------------------------------------------------------------------
	SPI11_SlaveReceive(srx_buf, sizeof(srx_buf));
	SPI10_MasterSend(mtx_buf, sizeof(mtx_buf));
#ifdef SPI11_WITH_DMA
	__WFI();
#else
	while(g_spi11_rx_count != 0);
#endif	
	delayMS(1);

//-----------------------------------------------------------------------
// Master Receive and Slave Send
//-----------------------------------------------------------------------
	SPI11_SlaveSend(stx_buf, sizeof(stx_buf));
	SPI10_MasterReceive(mrx_buf, sizeof(mrx_buf));
#ifdef SPI10_WITH_DMA
	__WFI();
#else
	while(g_spi10_rx_count != 0);
#endif	
	delayMS(1);

//-----------------------------------------------------------------------
// Check Data
//-----------------------------------------------------------------------
#ifndef RTL_SIMULATION
	err = 0;
	for(i=0; i<sizeof(srx_buf); i++) {		
		if(mtx_buf[i] != srx_buf[i]) 
		{
			err++; 
			printf("mtx_value[%d] = %02x, srx_value[%d] = %02x\n", i, mtx_buf[i], i, srx_buf[i]);
		}	
		if(stx_buf[i] != mrx_buf[i]) 
		{
			err++; 
			printf("stx_value[%d] = %02x, mrx_value[%d] = %02x\n", i, stx_buf[i], i, mrx_buf[i]);
		}
	}
	if(err) {
		printf("SPI Mode 0 Send OR  Receive test NG!!\n");
	}
	else {
		printf("SPI Mode 0 Send OR  Receive test OK!!\n");
	}
#endif
//-----------------------------------------------------------------------
// Master Send/Receive and Slave Send/Receive
//-----------------------------------------------------------------------
	SPI11_SlaveSendReceive(stx_buf, sizeof(stx_buf), srx_buf);
	SPI10_MasterSendReceive(mtx_buf, sizeof(mtx_buf), mrx_buf);
#ifdef SPI11_WITH_DMA
	__WFI();
#else
	while(g_spi11_rx_count != 0);
#endif	
	delayMS(1);

//-----------------------------------------------------------------------
// Check Data
//-----------------------------------------------------------------------
#ifndef RTL_SIMULATION
	err = 0;
	for(i=0; i<sizeof(srx_buf); i++) {		
		if(mtx_buf[i] != srx_buf[i]) 
		{
			err++; 
			printf("mtx_value[%d] = %02x, srx_value[%d] = %02x\n", i, mtx_buf[i], i, srx_buf[i]);
		}	
		if(stx_buf[i] != mrx_buf[i]) 
		{
			err++; 
			printf("stx_value[%d] = %02x, mrx_value[%d] = %02x\n", i, stx_buf[i], i, mrx_buf[i]);
		}
	}
	if(err) {
		printf("SPI Mode 0 Send AND Receive test NG!!\n");
	}
	else {
		printf("SPI Mode 0 Send AND Receive test OK!!\n");
	}
#endif
//=======================================================================
// SPI10 MODE 3 Master transmission/reception
// SPI11 MODE 3 Slave reception/transmission
//   Mode 0: CPOL = 0, CPHA = 0; i.e. DAP = 1, CKP = 1
//   Mode 1: CPOL = 0, CPHA = 1; i.e. DAP = 0, CKP = 1
//   Mode 2: CPOL = 1, CPHA = 0; i.e. DAP = 1, CKP = 0
//   Mode 3: CPOL = 1, CPHA = 1; i.e. DAP = 0, CKP = 0
//=======================================================================
	SPI10_MasterInit(SPI_MODE_3);
	SPI11_SlaveInit(SPI_MODE_3);
//-----------------------------------------------------------------------
// Master Send and Slave Receive
//-----------------------------------------------------------------------
	SPI11_SlaveReceive(srx_buf, sizeof(srx_buf));
	SPI10_MasterSend(mtx_buf, sizeof(mtx_buf));
#ifdef SPI11_WITH_DMA
	__WFI();
#else
	while(g_spi11_rx_count != 0);
#endif	
	delayMS(1);

//-----------------------------------------------------------------------
// Master Receive and Slave Send
//-----------------------------------------------------------------------
	SPI11_SlaveSend(stx_buf, sizeof(stx_buf));
	SPI10_MasterReceive(mrx_buf, sizeof(mrx_buf));
#ifdef SPI10_WITH_DMA
	__WFI();
#else
	while(g_spi10_rx_count != 0);
#endif	
	delayMS(1);

//-----------------------------------------------------------------------
// Check Data
//-----------------------------------------------------------------------
#ifndef RTL_SIMULATION
	err = 0;
	for(i=0; i<sizeof(srx_buf); i++) {		
		if(mtx_buf[i] != srx_buf[i]) 
		{
			err++; 
			printf("mtx_value[%d] = %02x, srx_value[%d] = %02x\n", i, mtx_buf[i], i, srx_buf[i]);
		}	
		if(stx_buf[i] != mrx_buf[i]) 
		{
			err++; 
			printf("stx_value[%d] = %02x, mrx_value[%d] = %02x\n", i, stx_buf[i], i, mrx_buf[i]);
		}
	}
	if(err) {
		printf("SPI Mode 3 Send OR  Receive test NG!!\n");
	}
	else {
		printf("SPI Mode 3 Send OR  Receive test OK!!\n");
	}
#endif
//-----------------------------------------------------------------------
// Master Send/Receive and Slave Send/Receive
//-----------------------------------------------------------------------
	SPI11_SlaveSendReceive(stx_buf, sizeof(stx_buf), srx_buf);
	SPI10_MasterSendReceive(mtx_buf, sizeof(mtx_buf), mrx_buf);
#ifdef SPI11_WITH_DMA
	__WFI();
#else
	while(g_spi11_rx_count != 0);
#endif	
	delayMS(1);

//-----------------------------------------------------------------------
// Check Data
//-----------------------------------------------------------------------
#ifndef RTL_SIMULATION
	err = 0;
	for(i=0; i<sizeof(srx_buf); i++) {		
		if(mtx_buf[i] != srx_buf[i]) 
		{
			err++; 
			printf("mtx_value[%d] = %02x, srx_value[%d] = %02x\n", i, mtx_buf[i], i, srx_buf[i]);
		}	
		if(stx_buf[i] != mrx_buf[i]) 
		{
			err++; 
			printf("stx_value[%d] = %02x, mrx_value[%d] = %02x\n", i, stx_buf[i], i, mrx_buf[i]);
		}
	}
	if(err) {
		printf("SPI Mode 3 Send AND Receive test NG!!\n");
	}
	else {
		printf("SPI Mode 3 Send AND Receive test OK!!\n");
	}
#endif
//=======================================================================
// SPI10 MODE 1 Master transmission/reception
// SPI11 MODE 1 Slave reception/transmission
//   Mode 0: CPOL = 0, CPHA = 0; i.e. DAP = 1, CKP = 1
//   Mode 1: CPOL = 0, CPHA = 1; i.e. DAP = 0, CKP = 1
//   Mode 2: CPOL = 1, CPHA = 0; i.e. DAP = 1, CKP = 0
//   Mode 3: CPOL = 1, CPHA = 1; i.e. DAP = 0, CKP = 0
//=======================================================================
	SPI10_MasterInit(SPI_MODE_1);
	SPI11_SlaveInit(SPI_MODE_1);

//-----------------------------------------------------------------------
// Master Send and Slave Receive
//-----------------------------------------------------------------------
	SPI11_SlaveReceive(srx_buf, sizeof(srx_buf));
	SPI10_MasterSend(mtx_buf, sizeof(mtx_buf));
#ifdef SPI11_WITH_DMA
	__WFI();
#else
	while(g_spi11_rx_count != 0);
#endif	
	delayMS(1);

//-----------------------------------------------------------------------
// Master Receive and Slave Send
//-----------------------------------------------------------------------
	SPI11_SlaveSend(stx_buf, sizeof(stx_buf));
	SPI10_MasterReceive(mrx_buf, sizeof(mrx_buf));
#ifdef SPI10_WITH_DMA
	__WFI();
#else
	while(g_spi10_rx_count != 0);
#endif	
	delayMS(1);

//-----------------------------------------------------------------------
// Check Data
//-----------------------------------------------------------------------
#ifndef RTL_SIMULATION
	err = 0;
	for(i=0; i<sizeof(srx_buf); i++) {		
		if(mtx_buf[i] != srx_buf[i]) 
		{
			err++; 
			printf("mtx_value[%d] = %02x, srx_value[%d] = %02x\n", i, mtx_buf[i], i, srx_buf[i]);
		}	
		if(stx_buf[i] != mrx_buf[i]) 
		{
			err++; 
			printf("stx_value[%d] = %02x, mrx_value[%d] = %02x\n", i, stx_buf[i], i, mrx_buf[i]);
		}
	}
	if(err) {
		printf("SPI Mode 1 Send OR  Receive test NG!!\n");
	}
	else {
		printf("SPI Mode 1 Send OR  Receive test OK!!\n");
	}
#endif
//-----------------------------------------------------------------------
// Master Send/Receive and Slave Send/Receive
//-----------------------------------------------------------------------
	SPI11_SlaveSendReceive(stx_buf, sizeof(stx_buf), srx_buf);
	SPI10_MasterSendReceive(mtx_buf, sizeof(mtx_buf), mrx_buf);
#ifdef SPI11_WITH_DMA
	__WFI();
#else
	while(g_spi11_rx_count != 0);
#endif	
	delayMS(1);

//-----------------------------------------------------------------------
// Check Data
//-----------------------------------------------------------------------
#ifndef RTL_SIMULATION
	err = 0;
	for(i=0; i<sizeof(srx_buf); i++) {		
		if(mtx_buf[i] != srx_buf[i]) 
		{
			err++; 
			printf("mtx_value[%d] = %02x, srx_value[%d] = %02x\n", i, mtx_buf[i], i, srx_buf[i]);
		}	
		if(stx_buf[i] != mrx_buf[i]) 
		{
			err++; 
			printf("stx_value[%d] = %02x, mrx_value[%d] = %02x\n", i, stx_buf[i], i, mrx_buf[i]);
		}
	}
	if(err) {
		printf("SPI Mode 1 Send AND Receive test NG!!\n");
	}
	else {
		printf("SPI Mode 1 Send AND Receive test OK!!\n");
	}
#endif
//=======================================================================
// SPI10 MODE 2 Master transmission/reception
// SPI11 MODE 2 Slave reception/transmission
//   Mode 0: CPOL = 0, CPHA = 0; i.e. DAP = 1, CKP = 1
//   Mode 1: CPOL = 0, CPHA = 1; i.e. DAP = 0, CKP = 1
//   Mode 2: CPOL = 1, CPHA = 0; i.e. DAP = 1, CKP = 0
//   Mode 3: CPOL = 1, CPHA = 1; i.e. DAP = 0, CKP = 0
//=======================================================================
	SPI10_MasterInit(SPI_MODE_2);
	SPI11_SlaveInit(SPI_MODE_2);

//-----------------------------------------------------------------------
// Master Send and Slave Receive
//-----------------------------------------------------------------------
	SPI11_SlaveReceive(srx_buf, sizeof(srx_buf));
	SPI10_MasterSend(mtx_buf, sizeof(mtx_buf));
#ifdef SPI11_WITH_DMA
	__WFI();
#else
	while(g_spi11_rx_count != 0);
#endif	
	delayMS(1);

//-----------------------------------------------------------------------
// Master Receive and Slave Send
//-----------------------------------------------------------------------
	SPI11_SlaveSend(stx_buf, sizeof(stx_buf));
	SPI10_MasterReceive(mrx_buf, sizeof(mrx_buf));
#ifdef SPI10_WITH_DMA
	__WFI();
#else
	while(g_spi10_rx_count != 0);
#endif	
	delayMS(1);

//-----------------------------------------------------------------------
// Check Data
//-----------------------------------------------------------------------
#ifndef RTL_SIMULATION
	err = 0;
	for(i=0; i<sizeof(srx_buf); i++) {		
		if(mtx_buf[i] != srx_buf[i]) 
		{
			err++; 
			printf("mtx_value[%d] = %02x, srx_value[%d] = %02x\n", i, mtx_buf[i], i, srx_buf[i]);
		}	
		if(stx_buf[i] != mrx_buf[i]) 
		{
			err++; 
			printf("stx_value[%d] = %02x, mrx_value[%d] = %02x\n", i, stx_buf[i], i, mrx_buf[i]);
		}
	}
	if(err) {
		printf("SPI Mode 2 Send OR  Receive test NG!!\n");
	}
	else {
		printf("SPI Mode 2 Send OR  Receive test OK!!\n");
	}
#endif
//-----------------------------------------------------------------------
// Master Send/Receive and Slave Send/Receive
//-----------------------------------------------------------------------
	SPI11_SlaveSendReceive(stx_buf, sizeof(stx_buf), srx_buf);
	SPI10_MasterSendReceive(mtx_buf, sizeof(mtx_buf), mrx_buf);
#ifdef SPI11_WITH_DMA
	__WFI();
#else
	while(g_spi11_rx_count != 0);
#endif	
	delayMS(1);

//-----------------------------------------------------------------------
// Check Data
//-----------------------------------------------------------------------
#ifndef RTL_SIMULATION
	err = 0;
	for(i=0; i<sizeof(srx_buf); i++) {		
		if(mtx_buf[i] != srx_buf[i]) 
		{
			err++; 
			printf("mtx_value[%d] = %02x, srx_value[%d] = %02x\n", i, mtx_buf[i], i, srx_buf[i]);
		}	
		if(stx_buf[i] != mrx_buf[i]) 
		{
			err++; 
			printf("stx_value[%d] = %02x, mrx_value[%d] = %02x\n", i, stx_buf[i], i, mrx_buf[i]);
		}
	}
	if(err) {
		printf("SPI Mode 2 Send AND Receive test NG!!\n");
	}
	else {
		printf("SPI Mode 2 Send AND Receive test OK!!\n");
	}
#endif
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

