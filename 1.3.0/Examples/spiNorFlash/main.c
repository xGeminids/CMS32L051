
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
//#define RTL_SIMULATION

// big and little endian convertion for 16bit number
#define BSWAP_16(x) \
    (uint16_t)((((uint16_t)(x) & 0x00ff) << 8) | \
               (((uint16_t)(x) & 0xff00) >> 8)   \
              )

// big and little endian convertion for 32bit number
#define BSWAP_32(x) \
    (uint32_t)((((uint32_t)(x) & 0xff000000) >> 24) | \
               (((uint32_t)(x) & 0x00ff0000) >> 8)  | \
               (((uint32_t)(x) & 0x0000ff00) << 8)  | \
               (((uint32_t)(x) & 0x000000ff) << 24)   \
			        )

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "math.h"
#include "stdio.h"
#include "CMS32L051.h"
#include "userdefine.h"
#include "sci.h"
#include "spi.h"
#include "spiflash.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
volatile uint32_t g_ticks;
uint8_t rdata[16];
uint8_t wdata[16];
uint32_t err;

void delayMS(uint32_t n)
{
		g_ticks = n;
		while(g_ticks);
}

int main (void)
{
	MD_STATUS status;
	uint32_t msCnt; 	// count value of 1ms
	uint32_t i;
	uint32_t div;
	int8_t n;
	
//-----------------------------------------------------------------------
// Systick setting 
//-----------------------------------------------------------------------   
	g_ticks = 1000; 	// 1000ms
	SystemCoreClockUpdate();
	msCnt = SystemCoreClock / 1000;
	SysTick_Config(msCnt);

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
	printf("SPI Flash - Erase, Write and Fast Read (0Bh)\n");
#endif
//=========================================================================================
// SPI Setting
//=========================================================================================
// For SPI Master and SPI Flash, Please connect the following pins:
// NSS:  P63/NSS  --> CS#
// SCK:  P31/SCK  --> SCK
// MOSI: P74/MOSI --> SI/SIO0	
// MISO: P75/MISO <-- SO/SIO1	
	SPI_INIT(SPI_MODE_0);

//=========================================================================================
// Initial write data
//=========================================================================================
	for(i=0; i<sizeof(wdata); i++)
	{
		wdata[i] = i;
	}

//=========================================================================================
// SPI Flash Erase, Page Program and Fast Read Instruction 
//=========================================================================================
	FlashEraseSector(0x00000000);
	LOOP_WAIT(100);
	FlashProgramPage(0x0000, sizeof(wdata), wdata);
	for(n=3; n>=0; n--)
	{
		div = (uint8_t)pow(2, n);
		printf("SCK = %dM/%d = %dK\n", SystemCoreClock/1000000, div, SystemCoreClock/1000/div);

		SPI_SPEED(n);

		for(i=0; i<sizeof(rdata); i++) {		
			rdata[i] = 0xFF; //initial rdata buf
		}
		FlashFastRead(0x0000, sizeof(rdata), rdata);

//=========================================================================================
// Check Data 
//=========================================================================================
		err = 0;
		for(i=0; i<sizeof(wdata); i++) {		
			if(rdata[i] != wdata[i]) 
			{
				err++;
				printf("exp_value[%2d] = %02x, mrx_value[%2d] = %02x\n", i, wdata[i], i, rdata[i]);
			}	
		}
		if(err) {
			printf("SPI Mode 0 Reveive NG!!\n");
			return(1);
		}
		else {
			printf("SPI Mode 0 Receive OK!!\n");
		}		
	}	
//=========================================================================================
// OK: LED ON
// NG: LED OFF
//=========================================================================================
#if 0
	PORT->PMC7 &= ~(3<<1);  // P71, P72 digital function
	PORT->PM7  &= ~(3<<1);  // P71, P72 output mode
	PORT->P7   |=  (3<<1);  // P71/LED, P72/LED OFF
 	if(err)
 	{
		for(i=0; i<8; i++)
		{
			PORT->P7 ^= (1<<1);
	#ifndef RTL_SIMULATION
			delayMS(250);
	#endif
		}
		PORT->PSET7 = (1<<1); 	// P71/LED OFF
 	}
 	else
 	{
		for(i=0; i<8; i++)
		{
			PORT->P7 ^= (1<<1);
	#ifndef RTL_SIMULATION
			delayMS(250);
	#endif
		}
		PORT->PCLR7 = (1<<1); 	// P71/LED ON
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
