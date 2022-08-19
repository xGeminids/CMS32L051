
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
#define DATA_PRINT
#define FLASH_BASE  0x00000000
#define SECTOR_SIZE 512
#define BLOCK_SIZE  16384 	 //  16K Byte
//#define FLASH_SIZE  0x20000  // 128K Byte
//#define FLASH_SIZE  0x10000  // 64K Byte
#define FLASH_SIZE  0x08000  // 32K Byte

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
#include "stdio.h"
#include "CMS32L051.h"
#include "userdefine.h"
#include "sci.h"
#include "spiflash.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
uint32_t g_ticks;
uint8_t rdata[64][2][16];
uint8_t wdata[64][2][16];
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
	uint32_t i, j;
	uint32_t *w_ptr;
	uint8_t  *b_ptr;
	
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
  printf("SPI Flash - Erase, Write and Fast Read (0Bh)\n\n");
#endif

//=========================================================================================
// SPI Setting
//=========================================================================================
// for SPI11 Master and SPI Flash
// NSS:  P62/SS11  --> CS#
// SCK:  P10/SCK11 --> SCK 
// MOSI: P12/SDO11 --> SI/SIO0	
// MISO: P11/SDI11 <-- SO/SIO1
  SPI_INIT(SPI_MODE_0);

//=========================================================================================
// Initial write data
//=========================================================================================
  for(i=0; i<sizeof(wdata)/sizeof(wdata[0]); i++) {
    w_ptr = (uint32_t *)&wdata[i];
    *w_ptr++ = BSWAP_32(i);
    *w_ptr++ = 0x55555555;
    *w_ptr++ = 0x04030201;
    *w_ptr++ = 0x08070605;
    *w_ptr++ = BSWAP_32(i);
    *w_ptr++ = 0xAAAAAAAA;
    *w_ptr++ = 0x04030201;
    *w_ptr++ = 0x08070605;
  }

//=========================================================================================
// Chip Erase
//=========================================================================================
  printf("INFO: Chip Erase...\n");
  FlashEraseChip();

//=========================================================================================
// Check Chip Erase
//=========================================================================================
  printf("INFO: Check Chip Erase...\n");
  for(i=0; i<FLASH_SIZE/BLOCK_SIZE; i++)
  {
	  // Read the first 16 bytes of a Block Section
    b_ptr= (uint8_t *)&rdata[(i + 0) * BLOCK_SIZE / SECTOR_SIZE - 0][0][0];
    FlashFastRead((FLASH_BASE + BLOCK_SIZE * (i + 0) - 0x00), 16, b_ptr);

	  // Read the last 16 bytes of a Block Section
    b_ptr= (uint8_t *)&rdata[(i + 1) * BLOCK_SIZE / SECTOR_SIZE - 1][1][0];
    FlashFastRead((FLASH_BASE + BLOCK_SIZE * (i + 1) - 0x10), 16, b_ptr);
  }

  // Check data section
  for(i=0; i<FLASH_SIZE/BLOCK_SIZE; i++)
  {
	  for(j=0; j<16; j++)
	  {
      if(rdata[(i + 0) * BLOCK_SIZE / SECTOR_SIZE - 0][0][j] != 0xFF ) err++;
      if(rdata[(i + 1) * BLOCK_SIZE / SECTOR_SIZE - 1][1][j] != 0xFF ) err++;
	  }
  }

#ifdef DATA_PRINT
  printf("         00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15\n");
  // print data section
  for(i=0; i<FLASH_SIZE/BLOCK_SIZE; i++)
  {
    for(j=0; j<16; j++)
    {
      if(j == 0x00) printf("0x%05X:", (BLOCK_SIZE * (i + 0) - 0x00));
      printf(" %02X", rdata[(i + 0) * BLOCK_SIZE / SECTOR_SIZE - 0][0][j]);
      if(j % 16 == 15) printf(" #Sector %03d\n", (i + 0) * BLOCK_SIZE / SECTOR_SIZE - 0);
    }
    for(j=0; j<16; j++)
    {
      if(j == 0x00) printf("0x%05X:", (BLOCK_SIZE * (i + 1) - 0x10));
      printf(" %02X", rdata[(i + 1) * BLOCK_SIZE / SECTOR_SIZE - 1][1][j]);
      if(j % 16 == 15) printf(" #Sector %03d\n", (i + 1) * BLOCK_SIZE / SECTOR_SIZE - 1);
    }
  }
  if(err)
  {
    printf("INFO: Chip Erase Done. Result is NG!!\n\n");
  }
  else
  {
    printf("INFO: Chip Erase Done. Result is OK!!\n\n");
  }
#endif
//=========================================================================================
// Program each sector
//=========================================================================================
  printf("INFO: Page Program...\n");
  for(i=0; i<FLASH_SIZE/SECTOR_SIZE; i++) {
  	FlashProgramPage(SECTOR_SIZE * (i + 0) - 0x00, 16, (uint8_t *)&wdata[i][0][0]);
  	FlashProgramPage(SECTOR_SIZE * (i + 1) - 0x10, 16, (uint8_t *)&wdata[i][1][0]);
  }

//=========================================================================================
// Check Write Data
//=========================================================================================
  printf("INFO: Check Write Data...\n");
  for(i=0; i<FLASH_SIZE/BLOCK_SIZE; i++)
  {
	  // Read the first 16 bytes of a Block Section
    b_ptr= (uint8_t *)&rdata[(i + 0) * BLOCK_SIZE / SECTOR_SIZE - 0][0][0];
    FlashFastRead((FLASH_BASE + BLOCK_SIZE * (i + 0) - 0x00), 16, b_ptr);

	  // Read the last 16 bytes of a Block Section
    b_ptr= (uint8_t *)&rdata[(i + 1) * BLOCK_SIZE / SECTOR_SIZE - 1][1][0];
    FlashFastRead((FLASH_BASE + BLOCK_SIZE * (i + 1) - 0x10), 16, b_ptr);
  }

  // Check data section
  for(i=0; i<FLASH_SIZE/BLOCK_SIZE; i++)
  {
	  for(j=0; j<16; j++)
	  {
      if(rdata[(i + 0) * BLOCK_SIZE / SECTOR_SIZE - 0][0][j] != wdata[(i + 0) * BLOCK_SIZE / SECTOR_SIZE - 0][0][j] ) err++;
      if(rdata[(i + 1) * BLOCK_SIZE / SECTOR_SIZE - 1][1][j] != wdata[(i + 1) * BLOCK_SIZE / SECTOR_SIZE - 1][1][j] ) err++;
	  }
  }

#ifdef DATA_PRINT
  printf("         00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15\n");
  // print data section
  for(i=0; i<FLASH_SIZE/BLOCK_SIZE; i++)
  {
    for(j=0; j<16; j++)
    {
      if(j == 0x00) printf("0x%05X:", (BLOCK_SIZE * (i + 0) - 0x00));
      printf(" %02X", rdata[(i + 0) * BLOCK_SIZE / SECTOR_SIZE - 0][0][j]);
      if(j % 16 == 15) printf(" #Sector %03d\n", (i + 0) * BLOCK_SIZE / SECTOR_SIZE - 0);
    }
    for(j=0; j<16; j++)
    {
      if(j == 0x00) printf("0x%05X:", (BLOCK_SIZE * (i + 1) - 0x10));
      printf(" %02X", rdata[(i + 1) * BLOCK_SIZE / SECTOR_SIZE - 1][1][j]);
      if(j % 16 == 15) printf(" #Sector %03d\n", (i + 1) * BLOCK_SIZE / SECTOR_SIZE - 1);
    }
  }
  if(err)
  {
    printf("INFO: Page Program Done. Result is NG!!\n\n");
  }
  else
  {
    printf("INFO: Page Program Done. Result is OK!!\n\n");
  }
#endif

//=========================================================================================
// Sector Erase
//=========================================================================================
  printf("INFO: Sector 0 and 31 Erase...\n");
  FlashEraseSector(0x00000); 	// sector 000
  FlashEraseSector(0x03FF0); 	// sector 031

//=========================================================================================
// Check Chip Erase
//=========================================================================================
  printf("INFO: Check Sector Erase...\n");
  for(i=0; i<FLASH_SIZE/BLOCK_SIZE; i++)
  {
	  // Read the first 16 bytes of a Block Section
    b_ptr= (uint8_t *)&rdata[(i + 0) * BLOCK_SIZE / SECTOR_SIZE - 0][0][0];
    FlashFastRead((FLASH_BASE + BLOCK_SIZE * (i + 0) - 0x00), 16, b_ptr);

	  // Read the last 16 bytes of a Block Section
    b_ptr= (uint8_t *)&rdata[(i + 1) * BLOCK_SIZE / SECTOR_SIZE - 1][1][0];
    FlashFastRead((FLASH_BASE + BLOCK_SIZE * (i + 1) - 0x10), 16, b_ptr);
  }

  // Check data section
  for(i=0; i<1; i++)
  {
	  for(j=0; j<16; j++)
	  {
      if(rdata[(i + 0) * BLOCK_SIZE / SECTOR_SIZE - 0][0][j] != 0xFF ) err++;
      if(rdata[(i + 1) * BLOCK_SIZE / SECTOR_SIZE - 1][1][j] != 0xFF ) err++;
	  }
  }
  for(i=1; i<FLASH_SIZE/BLOCK_SIZE; i++)
  {
	  for(j=0; j<16; j++)
	  {
      if(rdata[(i + 0) * BLOCK_SIZE / SECTOR_SIZE - 0][0][j] != wdata[(i + 0) * BLOCK_SIZE / SECTOR_SIZE - 0][0][j] ) err++;
      if(rdata[(i + 1) * BLOCK_SIZE / SECTOR_SIZE - 1][1][j] != wdata[(i + 1) * BLOCK_SIZE / SECTOR_SIZE - 1][1][j] ) err++;
	  }
  }

#ifdef DATA_PRINT
  printf("         00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15\n");
  // print data section
  for(i=0; i<FLASH_SIZE/BLOCK_SIZE; i++)
  {
    for(j=0; j<16; j++)
    {
      if(j == 0x00) printf("0x%05X:", (BLOCK_SIZE * (i + 0) - 0x00));
      printf(" %02X", rdata[(i + 0) * BLOCK_SIZE / SECTOR_SIZE - 0][0][j]);
      if(j % 16 == 15) printf(" #Sector %03d\n", (i + 0) * BLOCK_SIZE / SECTOR_SIZE - 0);
    }
    for(j=0; j<16; j++)
    {
      if(j == 0x00) printf("0x%05X:", (BLOCK_SIZE * (i + 1) - 0x10));
      printf(" %02X", rdata[(i + 1) * BLOCK_SIZE / SECTOR_SIZE - 1][1][j]);
      if(j % 16 == 15) printf(" #Sector %03d\n", (i + 1) * BLOCK_SIZE / SECTOR_SIZE - 1);
    }
  }
  if(err)
  {
    printf("INFO: Sector Erase Done. Result is NG!!\n\n");
  }
  else
  {
    printf("INFO: Sector Erase Done. Result is OK!!\n\n");
  }
#endif

//=========================================================================================
// Block Erase
//=========================================================================================
  printf("INFO: Block 1 Erase...\n");
  FlashEraseBlock(0x04000); 	// Block 1: Sector 32 ~ 63

//=========================================================================================
// Check Block Erase
//=========================================================================================
  printf("INFO: Check Block Erase...\n");
  for(i=0; i<FLASH_SIZE/BLOCK_SIZE; i++)
  {
	  // Read the first 16 bytes of a Block Section
    b_ptr= (uint8_t *)&rdata[(i + 0) * BLOCK_SIZE / SECTOR_SIZE - 0][0][0];
    FlashFastRead((FLASH_BASE + BLOCK_SIZE * (i + 0) - 0x00), 16, b_ptr);

	  // Read the last 16 bytes of a Block Section
    b_ptr= (uint8_t *)&rdata[(i + 1) * BLOCK_SIZE / SECTOR_SIZE - 1][1][0];
    FlashFastRead((FLASH_BASE + BLOCK_SIZE * (i + 1) - 0x10), 16, b_ptr);
  }

  // Check data section
  for(i=0; i<2; i++)
  {
	  for(j=0; j<16; j++)
	  {
      if(rdata[(i + 0) * BLOCK_SIZE / SECTOR_SIZE - 0][0][j] != 0xFF ) err++;
      if(rdata[(i + 1) * BLOCK_SIZE / SECTOR_SIZE - 1][1][j] != 0xFF ) err++;
	  }
  }
  for(i=2; i<FLASH_SIZE/BLOCK_SIZE; i++)
  {
	  for(j=0; j<16; j++)
	  {
      if(rdata[(i + 0) * BLOCK_SIZE / SECTOR_SIZE - 0][0][j] != wdata[(i + 0) * BLOCK_SIZE / SECTOR_SIZE - 0][0][j] ) err++;
      if(rdata[(i + 1) * BLOCK_SIZE / SECTOR_SIZE - 1][1][j] != wdata[(i + 1) * BLOCK_SIZE / SECTOR_SIZE - 1][1][j] ) err++;
	  }
  }

# ifdef DATA_PRINT
  printf("         00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15\n");
  // print data section
  for(i=0; i<FLASH_SIZE/BLOCK_SIZE; i++)
  {
    for(j=0; j<16; j++)
    {
      if(j == 0x00) printf("0x%05X:", (BLOCK_SIZE * (i + 0) - 0x00));
      printf(" %02X", rdata[(i + 0) * BLOCK_SIZE / SECTOR_SIZE - 0][0][j]);
      if(j % 16 == 15) printf(" #Sector %03d\n", (i + 0) * BLOCK_SIZE / SECTOR_SIZE - 0);
    }
    for(j=0; j<16; j++)
    {
      if(j == 0x00) printf("0x%05X:", (BLOCK_SIZE * (i + 1) - 0x10));
      printf(" %02X", rdata[(i + 1) * BLOCK_SIZE / SECTOR_SIZE - 1][1][j]);
      if(j % 16 == 15) printf(" #Sector %03d\n", (i + 1) * BLOCK_SIZE / SECTOR_SIZE - 1);
    }
  }
  if(err)
  {
    printf("INFO: Block Erase Done. Result is NG!!\n\n");
  }
  else
  {
    printf("INFO: Block Erase Done. Result is OK!!\n\n");
  }
#endif

//=========================================================================================
// OK: LED ON
// NG: LED OFF
//=========================================================================================
	PORT->PMC7 &= ~(3<<1);  // P71, P72 digital function
	PORT->PM7  &= ~(3<<1);  // P71, P72 output mode
	PORT->P7   |=  (3<<1);  // P71/LED, P72/LED OFF
  if(err)
  {
		for(i=0; i<8; i++)
		{
			PORT->P7 ^= (1<<1);
			delayMS(250);
		}
		PORT->PSET7 = (1<<1); 	// P71/LED OFF
  }
  else
  {
		for(i=0; i<8; i++)
		{
			PORT->P7 ^= (1<<1);
			delayMS(250);
		}
		PORT->PCLR7 = (1<<1); 	// P71/LED ON
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
