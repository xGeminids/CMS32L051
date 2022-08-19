
#ifndef SPI_FLASH_H
#define SPI_FLASH_H

#include "userdefine.h"
#include "CMS32L051.h"
#include "spi.h"
#include "dma.h"

#define READ_WITH_RECEPTION_ONLY
#define SPI_BUFFER_EMPTY

/**************************************************************************
* SPI Data and Status Register
**************************************************************************/
#define SPI_WRDR 		(SPI->SDRO)
#define SPI_RDDR 		(SPI->SDRI)
#define SPI_BUSY 		(SPI->SPIM & _0001_SPI_UNDER_EXECUTE)
#define SPI_RRDY 		(SPI->SPIM & _0002_SPI_VALID_STORED)
#define SPI_FLASH_CS_CLR() 	NSS_PORT_CLR()
#define SPI_FLASH_CS_SET() 	NSS_PORT_SET()

/**************************************************************************
* SPI NOR Flash Instruction defination
**************************************************************************/
#define BYTE_FIELD(value, byte) ((adr & (0xFF << 8 * byte )) >> 8 * byte)
#define ENABLE_RESET 		(0x66U) 	// Enable Reset Instruction
#define RESET_DEVICE 		(0x99U) 	// Reset Device Instruction
#define READ_STATUS1 		(0x05U) 	// Read Status Register-1 Instruction
#define READ_STATUS2 		(0x35U) 	// Read Status Register-2 Instruction
#define WRITE_STATUS1 		(0x01U) 	// Write Status Register-1 Instruction
#define WRITE_STATUS2 		(0x31U) 	// Write Status Register-2 Instruction
#define WRITE_ENABLE 		(0x06U) 	// Write Enable Instruction
#define WRITE_DISABLE 		(0x04U) 	// Write Disable Instruction
#define READ_DATA 			(0x03U) 	// Read Data Instruction
#define FAST_READ 			(0x0BU) 	// Fast Read Instruction
#define SECTOR_ERASE 		(0x20U) 	// 4KB Sector Erase Instruction
#define BLOCK_ERASE 		(0x52U) 	// 16KB Block Erase Instruction
#define CHIP_ERASE 			(0x60U) 	// Chip Erase Instruction
#define PAGE_WRITE 			(0x02U) 	// Page Program Instruction

/**************************************************************************
* Bit Fields defination
**************************************************************************/
// SPI Nor Flash Status Register-1
#define BUSY_Pos  		(0x00U)
#define BUSY_Msk  		(0x01U)
#define WEL_Pos   		(0x01U)
#define WEL_Msk   		(0x02U)
#define BP_Pos    		(0x04U)
#define BP_Msk    		(0x30U)
#define TB_Pos    		(0x06U)
#define TB_Msk    		(0x40U)
#define BPL_Pos   		(0x07U)
#define BPS_Msk   		(0x80U)
// SPI Nor Flash Status Register-2
#define QE_Pos    		(0x01U)
#define QE_Msk    		(0x02U)

/**************************************************************************
* Typedef defination
**************************************************************************/
typedef enum
{
	STATUS_REG1 = 1,
	STATUS_REG2 = 2
} status_t;

/**************************************************************************
* Global functions
**************************************************************************/
__STATIC_INLINE void LOOP_WAIT(uint32_t n)
{
	while(n--)
	{
		__NOP();
	}
}

__STATIC_INLINE void SPI_INIT(spi_mode_t mode)
{
    CGC->PER1 |= 0x80;  // SPIEN = 1
    //SPI->SPIC = _0001_SPI_SCK_fCLK_1 | (~mode & 0x03) << 3;    
    //SPI->SPIC = _0002_SPI_SCK_fCLK_2 | (~mode & 0x03) << 3;    
    SPI->SPIC = _0003_SPI_SCK_fCLK_3 | (~mode & 0x03) << 3;    
    //SPI->SPIC = 0x80 | _0003_SPI_SCK_fCLK_3 | (~mode & 0x03) << 3;    
    //SPI->SPIC = _0004_SPI_SCK_fCLK_4 | (~mode & 0x03) << 3;    
    SPI->SPIM = _0040_SPI_RECEPTION_TRANSMISSION | _0000_SPI_MSB | _0000_SPI_TRANSFER_END | _0000_SPI_LENGTH_8;
    NSS_PORT_SETTING();
    SCK_PORT_SETTING();
    MOSI_PORT_SETTING();
    MISO_PORT_SETTING();
	SPI->SPIM |= _0080_SPI_START_TRG_ON; 
}

__STATIC_INLINE void SPI_SPEED(uint8_t n) 
{
	SPI->SPIM &= ~_0080_SPI_START_TRG_ON;
	SPI->SPIC &= ~0x07;	// clear lower 3 bits
	SPI->SPIC |= n;	    // fCLK/2**n
	SPI->SPIM |=  _0080_SPI_START_TRG_ON;
}

__STATIC_INLINE void SPI_CPU_WRITE(uint8_t data) 
{
	volatile uint8_t dummy;
	SPI->SPIC |= (1<<3); 	// DAP = 1
	SPI_WRDR = data;	
	while(SPI_BUSY);
	dummy = SPI_RDDR;
}

__STATIC_INLINE uint8_t SPI_CPU_READ(void) 	
{
	volatile uint8_t tmp;
	SPI->SPIC &= ~(1<<3);  // DAP = 0
	SPI_WRDR = 0xFFU;
	while(SPI_RRDY == 0);
	tmp = SPI_RDDR;
	return (tmp);
}

__STATIC_INLINE void SPI_DMA_WRITE(uint8_t *const tx_buf, uint16_t tx_num) 
{
	volatile uint16_t *ptr;
	SPI->SPIM &= ~_0080_SPI_START_TRG_ON ; // Clear INTSPI
	SPI->SPIM |=  _0080_SPI_START_TRG_ON | _0004_SPI_LENGTH_16; // Enable SPI and 16bit transfer mode
	SPI->SPIC |= (1<<3); 	// DAP = 1
 	/* write transfer data with DMA */
 	DMAVEC->VEC[DMA_VECTOR_SPI] = CTRL_DATA_SPI;
 	//DMAVEC->CTRL[CTRL_DATA_SPI].DMACR = (0 << CTRL_DMACR_SZ_Pos) | (0 << CTRL_DMACR_CHNE_Pos) |
 	DMAVEC->CTRL[CTRL_DATA_SPI].DMACR = (1 << CTRL_DMACR_SZ_Pos) | (0 << CTRL_DMACR_CHNE_Pos) |
 	                                    (0 << CTRL_DMACR_DAMOD_Pos) | (1 << CTRL_DMACR_SAMOD_Pos) |
 	                                    (0 << CTRL_DMACR_MODE_Pos);
 	DMAVEC->CTRL[CTRL_DATA_SPI].DMBLS = 1;
 	DMAVEC->CTRL[CTRL_DATA_SPI].DMACT = tx_num/2 -1;
 	DMAVEC->CTRL[CTRL_DATA_SPI].DMRLD = tx_num/2 -1;
 	DMAVEC->CTRL[CTRL_DATA_SPI].DMSAR = (uint32_t)(tx_buf + 2);
 	DMAVEC->CTRL[CTRL_DATA_SPI].DMDAR = (uint32_t)&SPI_WRDR;

 	/* init DMA registers */
	INT->IF[SPI_IRQn].IFL = 0;
 	CGC->PER1 |= CGC_PER1_DMAEN_Msk;
 	DMA->DMABAR = DMAVEC_BASE;
 	DMA->DMAEN1 |= (1 << DMA_VECTOR_SPI % 8);
    ptr = (uint16_t *)tx_buf;
	SPI_WRDR = *ptr;

	/* waiting DMA transfer end */
	while(INT->IF[SPI_IRQn].IFL == 0x00);
	while(SPI_BUSY);
	SPI->SPIM &= ~_0004_SPI_LENGTH_16;  // return 8bit tranfer mode
	INT->IF[SPI_IRQn].IFL = 0;
}

__STATIC_INLINE void SPI_DMA_READ(uint8_t *const rx_buf, uint16_t rx_num) 	
{
	volatile uint32_t tmp;

#ifdef READ_WITH_RECEPTION_ONLY
	SPI->SPIM &= ~_0040_SPI_TRANSMISSION; // only reception mode
#elif defined SPI_BUFFER_EMPTY
	SPI->SPIM |= _0008_SPI_BUFFER_EMPTY; 
#endif

	SPI->SPIM |=  _0004_SPI_LENGTH_16;    // 16bit transfer mode
	SPI->SPIC &= ~(1<<3);  // DAP = 0

#ifdef READ_WITH_RECEPTION_ONLY
	/* read receive data with DMA */
	DMAVEC->VEC[DMA_VECTOR_SPI] = CTRL_DATA_SPI;
	//DMAVEC->CTRL[CTRL_DATA_SPI].DMACR = (0 << CTRL_DMACR_SZ_Pos) | (0 << CTRL_DMACR_CHNE_Pos) |
	DMAVEC->CTRL[CTRL_DATA_SPI].DMACR = (1 << CTRL_DMACR_SZ_Pos) | (0 << CTRL_DMACR_CHNE_Pos) |
	                                    (1 << CTRL_DMACR_DAMOD_Pos) | (0 << CTRL_DMACR_SAMOD_Pos) |
	                                    (0 << CTRL_DMACR_MODE_Pos);
	DMAVEC->CTRL[CTRL_DATA_SPI].DMBLS = 1;
	DMAVEC->CTRL[CTRL_DATA_SPI].DMACT = rx_num/2;
	DMAVEC->CTRL[CTRL_DATA_SPI].DMRLD = rx_num/2;
	DMAVEC->CTRL[CTRL_DATA_SPI].DMSAR = (uint32_t)&SPI_RDDR;
	DMAVEC->CTRL[CTRL_DATA_SPI].DMDAR = (uint32_t)rx_buf;

#else 
	/* read receive data with DMA */
	DMAVEC->VEC[DMA_VECTOR_SPI] = CTRL_DATA_SPI;
	//DMAVEC->CTRL[CTRL_DATA_SPI].DMACR = (0 << CTRL_DMACR_SZ_Pos) | (1 << CTRL_DMACR_CHNE_Pos) |
	DMAVEC->CTRL[CTRL_DATA_SPI].DMACR = (1 << CTRL_DMACR_SZ_Pos) | (1 << CTRL_DMACR_CHNE_Pos) |
	                                    (1 << CTRL_DMACR_DAMOD_Pos) | (0 << CTRL_DMACR_SAMOD_Pos) |
	                                    (0 << CTRL_DMACR_MODE_Pos);
	DMAVEC->CTRL[CTRL_DATA_SPI].DMBLS = 1;
	DMAVEC->CTRL[CTRL_DATA_SPI].DMACT = rx_num/2;
	DMAVEC->CTRL[CTRL_DATA_SPI].DMRLD = rx_num/2;
	DMAVEC->CTRL[CTRL_DATA_SPI].DMSAR = (uint32_t)&SPI_RDDR;
	DMAVEC->CTRL[CTRL_DATA_SPI].DMDAR = (uint32_t)rx_buf;

    /* dummy write to trigger next transfer */
 	DMAVEC->CTRL[CTRL_DATA_SPI + 1].DMACR = (0 << CTRL_DMACR_SZ_Pos) | (0 << CTRL_DMACR_CHNE_Pos) |
 	                                        (0 << CTRL_DMACR_DAMOD_Pos) | (0 << CTRL_DMACR_SAMOD_Pos) |
 	                                        (0 << CTRL_DMACR_MODE_Pos);
 	DMAVEC->CTRL[CTRL_DATA_SPI + 1].DMBLS = 1;
 	DMAVEC->CTRL[CTRL_DATA_SPI + 1].DMACT = rx_num/2;
 	DMAVEC->CTRL[CTRL_DATA_SPI + 1].DMRLD = rx_num/2;
 	DMAVEC->CTRL[CTRL_DATA_SPI + 1].DMSAR = (uint32_t)&tmp;
 	DMAVEC->CTRL[CTRL_DATA_SPI + 1].DMDAR = (uint32_t)&SPI_WRDR;
#endif

	/* init DMA registers */
	INT->IF[SPI_IRQn].IFL = 0;
	CGC->PER1 |= CGC_PER1_DMAEN_Msk;
	DMA->DMABAR = DMAVEC_BASE;

#ifdef READ_WITH_RECEPTION_ONLY
	DMA->DMAEN1 |= (1 << DMA_VECTOR_SPI % 8);
	/* dummy read to trigger transfer start */
	tmp = SPI_RDDR ;
#elif defined SPI_BUFFER_EMPTY
	if((SPI->SPIC & 0x07U) > 1)
	{
		/* dummy write to trigger transfer start */
		SPI_WRDR = 0xFFFFU;
		while(SPI_BUSY == 0);
		INT->IF[SPI_IRQn].IFL = 0;
		SPI_WRDR = 0xFFFFU;
		DMA->DMAEN1 |= (1 << DMA_VECTOR_SPI % 8);
	}
	else
	{
		/* dummy write to trigger transfer start */
		SPI_WRDR = 0xFFFFU;
		SPI_WRDR = 0xFFFFU;
		DMA->DMAEN1 |= (1 << DMA_VECTOR_SPI % 8);
		INT->IF[SPI_IRQn].IFL = 0;
	}
#else
	/* dummy write to trigger transfer start */
	DMA->DMAEN1 |= (1 << DMA_VECTOR_SPI % 8);
	SPI_WRDR = 0xFFU;
#endif

	/* waiting DMA transfer end */
	while(INT->IF[SPI_IRQn].IFL == 0x00);
	SPI->SPIM &= ~(_0080_SPI_START_TRG_ON | _0004_SPI_LENGTH_16);
	SPI->SPIM |=  _0080_SPI_START_TRG_ON | _0040_SPI_TRANSMISSION; // transmission and reception mode
//	while((SPI->SPIM & _0002_SPI_VALID_STORED) == 0);
	INT->IF[SPI_IRQn].IFL = 0;

	return (0);
}

__STATIC_INLINE void FlashSoftReset(void)
{
	/* Enable Reset Instruction */
	SPI_FLASH_CS_CLR(); 			/* chip select active */
	SPI_CPU_WRITE(ENABLE_RESET);
	SPI_FLASH_CS_SET();			/* chip select inactive */

	LOOP_WAIT(10);

	/* Reset Device Instruction */
	SPI_FLASH_CS_CLR(); 			/* chip select active */
	SPI_CPU_WRITE(RESET_DEVICE);
	SPI_FLASH_CS_SET();			/* chip select inactive */
}

__STATIC_INLINE uint8_t FlashReadStatus(status_t name)
{
	uint8_t inst;
	uint8_t tmp;

	SPI_FLASH_CS_CLR(); 			/* chip select active */
	if(name == STATUS_REG1)
		inst = READ_STATUS1;
	else
		inst = READ_STATUS2;
	SPI_CPU_WRITE(inst);
	tmp = SPI_CPU_READ();  		/* get Status Register value */
	SPI_FLASH_CS_SET();			/* chip select inactive */

	return tmp;
}

__STATIC_INLINE void FlashWriteEnable(void)
{
	/* Write Enable Instruction */
	SPI_FLASH_CS_CLR(); 			/* chip select active */
	SPI_CPU_WRITE(WRITE_ENABLE);
	SPI_FLASH_CS_SET();			/* chip select inactive */
}

__STATIC_INLINE void FlashWriteStatus(status_t name, uint8_t data)
{
	uint8_t inst;

	/* Write enable instruction */
	FlashWriteEnable(); 	

	LOOP_WAIT(10);

	/* Write Status Register Instruction */
	SPI_FLASH_CS_CLR(); 			/* chip select active */
	if(name == STATUS_REG1)
		inst = WRITE_STATUS1;
	else
		inst = WRITE_STATUS2;
	SPI_CPU_WRITE(inst);
	SPI_CPU_WRITE(data);
	SPI_FLASH_CS_SET();			/* chip select inactive */
}

__STATIC_INLINE void FlashEraseSector(uint32_t adr)
{
	/* Write enable instruction */
	FlashWriteEnable(); 	

	LOOP_WAIT(10);

	/* Sector Erase Instruction */
	SPI_FLASH_CS_CLR(); 			/* chip select active */
	SPI_CPU_WRITE(SECTOR_ERASE);
	SPI_CPU_WRITE(BYTE_FIELD(adr, 2)); // Address [23:16] 
	SPI_CPU_WRITE(BYTE_FIELD(adr, 1)); // Address [15:08] 
	SPI_CPU_WRITE(BYTE_FIELD(adr, 0)); // Address [07:00] 
	SPI_FLASH_CS_SET();			/* chip select inactive */

	LOOP_WAIT(10);

	/* polling busy flag */
	SPI_FLASH_CS_CLR(); 			/* chip select active */
	SPI_CPU_WRITE(READ_STATUS1);
	while(SPI_CPU_READ() & BUSY_Msk);  
	SPI_FLASH_CS_SET();			/* chip select inactive */
}

__STATIC_INLINE void FlashEraseBlock(uint32_t adr)
{
	/* Write enable instruction */
	FlashWriteEnable(); 	

	LOOP_WAIT(10);

	/* Block Erase Instruction */
	SPI_FLASH_CS_CLR(); 			/* chip select active */
	SPI_CPU_WRITE(BLOCK_ERASE);
	SPI_CPU_WRITE(BYTE_FIELD(adr, 2)); // Address [23:16] 
	SPI_CPU_WRITE(BYTE_FIELD(adr, 1)); // Address [15:08] 
	SPI_CPU_WRITE(BYTE_FIELD(adr, 0)); // Address [07:00] 
	SPI_FLASH_CS_SET();			/* chip select inactive */

	LOOP_WAIT(10);

	/* polling busy flag */
	SPI_FLASH_CS_CLR(); 			/* chip select active */
	SPI_CPU_WRITE(READ_STATUS1);
	while(SPI_CPU_READ() & BUSY_Msk);  
	SPI_FLASH_CS_SET();			/* chip select inactive */
}

__STATIC_INLINE void FlashEraseChip(void)
{
	/* Write enable instruction */
	FlashWriteEnable(); 	

	/* Block Erase Instruction */
	SPI_FLASH_CS_CLR(); 			/* chip select active */
	SPI_CPU_WRITE(CHIP_ERASE);
	SPI_FLASH_CS_SET();			/* chip select inactive */

	LOOP_WAIT(10);

	/* polling busy flag */
	SPI_FLASH_CS_CLR(); 			/* chip select active */
	SPI_CPU_WRITE(READ_STATUS1);
	while(SPI_CPU_READ() & BUSY_Msk);  
	SPI_FLASH_CS_SET();			/* chip select inactive */
}

__STATIC_INLINE void FlashProgramPage(uint32_t adr, uint32_t sz, uint8_t *buf)
{
	/* Write enable instruction */
	FlashWriteEnable(); 	

	LOOP_WAIT(10);

	/* Page Program Instruction */
	SPI_FLASH_CS_CLR(); 			/* chip select active */
	SPI_CPU_WRITE(PAGE_WRITE);
	SPI_CPU_WRITE(BYTE_FIELD(adr, 2)); // Address [23:16] 
	SPI_CPU_WRITE(BYTE_FIELD(adr, 1)); // Address [15:08] 
	SPI_CPU_WRITE(BYTE_FIELD(adr, 0)); // Address [07:00] 

#ifdef SPI_WITH_DMA
	SPI_DMA_WRITE(buf, sz);
#else
	while(sz--)
	{
		SPI_CPU_WRITE(*buf++); // Write Data Byte
	}
#endif
	SPI_FLASH_CS_SET();			/* chip select inactive */

	LOOP_WAIT(10);

	/* polling busy flag */
	SPI_FLASH_CS_CLR(); 			/* chip select active */
	SPI_CPU_WRITE(READ_STATUS1);
	while(SPI_CPU_READ() & BUSY_Msk);  
	SPI_FLASH_CS_SET();			/* chip select inactive */
}

__STATIC_INLINE void FlashReadData(uint32_t adr, uint32_t sz, uint8_t *buf)
{
	volatile uint8_t tmp;
	/* Fast Read Instruction */
	SPI_FLASH_CS_CLR(); 			/* chip select active */
	SPI_CPU_WRITE(READ_DATA);
	SPI_CPU_WRITE(BYTE_FIELD(adr, 2)); // Address [23:16] 
	SPI_CPU_WRITE(BYTE_FIELD(adr, 1)); // Address [15:08] 
	SPI_CPU_WRITE(BYTE_FIELD(adr, 0)); // Address [07:00] 
#ifdef SPI_WITH_DMA
	SPI_DMA_READ(buf, sz);
#else
	while(sz--)
	{
		tmp = SPI_CPU_READ(); // Read Data Byte 
		*buf++ = tmp;
	}
#endif
	SPI_FLASH_CS_SET();			/* chip select inactive */
}

__STATIC_INLINE void FlashFastRead(uint32_t adr, uint32_t sz, uint8_t *buf)
{
	volatile uint8_t tmp;
	/* Fast Read Instruction */
	SPI_FLASH_CS_CLR(); 			/* chip select active */
	SPI_CPU_WRITE(FAST_READ);
	SPI_CPU_WRITE(BYTE_FIELD(adr, 2)); // Address [23:16] 
	SPI_CPU_WRITE(BYTE_FIELD(adr, 1)); // Address [15:08] 
	SPI_CPU_WRITE(BYTE_FIELD(adr, 0)); // Address [07:00] 
	SPI_CPU_WRITE(0xFFU); 		/* 8 cycles dummy clocks */
#ifdef SPI_WITH_DMA
	SPI_DMA_READ(buf, sz);
#else
	while(sz--)
	{
		tmp = SPI_CPU_READ(); // Read Data Byte 
		*buf++ = tmp;
	}
#endif
	SPI_FLASH_CS_SET();			/* chip select inactive */
}
#endif
