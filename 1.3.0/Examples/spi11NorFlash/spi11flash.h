
#ifndef SPI11_FLASH_H
#define SPI11_FLASH_H

#include "userdefine.h"
#include "CMS32L051.h"
#include "sci.h"
#include "dma.h"

#define SPI11_BUFFER_EMPTY

/**************************************************************************
* SPI Data and Status Register
**************************************************************************/
#define SPI_WRDR 		(SCI0->SIO11)
#define SPI_RDDR 		(SCI0->SIO11)
#define SPI_BUSY 		(SCI0->SSR03 & _0040_SCI_UNDER_EXECUTE)
#define SPI_RRDY 		(SCI0->SSR03 & _0020_SCI_VALID_STORED)
#define SPI_FLASH_CS_CLR() 	SS11_PORT_CLR()
#define SPI_FLASH_CS_SET() 	SS11_PORT_SET()

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
* Global variables
**************************************************************************/
static volatile uint8_t sdr_prescale;

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

__STATIC_INLINE void SPI_SPEED(uint8_t n) 
{
	SCI0->ST0 |= _0008_SCI_CH3_START_TRG_ON;
	SCI0->SDR03 = n << 9;	// fMCK/(1+n)x2
	sdr_prescale = n;
	SCI0->SS0 |= _0008_SCI_CH3_START_TRG_ON;
}

__STATIC_INLINE void SPI_INIT(spi_mode_t mode)
{
    uint8_t dap, ckp;
    dap = ~mode & 0x01;
    ckp = (~mode & 0x02) >> 1;
    CGC->PER0 |= CGC_PER0_SCI0EN_Msk;
    SCI0->ST0 |= _0008_SCI_CH3_STOP_TRG_ON;
    SCI0->SPS0 &= ~SCI0_SPS0_PRS01_Msk;
    SCI0->SPS0 |= (0 << SCI0_SPS0_PRS01_Pos);
    SCI0->SIR03 = _0004_SCI_SIRMN_FECTMN | _0002_SCI_SIRMN_PECTMN | _0001_SCI_SIRMN_OVCTMN;
    SCI0->SMR03 = _0020_SMRMN_DEFAULT_VALUE | _8000_SCI_CLOCK_SELECT_CK01 | _0000_SCI_CLOCK_MODE_CKS |
                  _0000_SCI_TRIGGER_SOFTWARE | _0000_SCI_MODE_SPI | _0000_SCI_TRANSFER_END;
    SCI0->SCR03 = _0004_SCRMN_DEFAULT_VALUE | _C000_SCI_RECEPTION_TRANSMISSION | dap << 13 | ckp << 12 | _0000_SCI_INTSRE_MASK |
                  _0000_SCI_PARITY_NONE | _0000_SCI_MSB | _0000_SCI_STOP_NONE | _0003_SCI_LENGTH_8;
	SPI_SPEED(3);
    /* Set output enable */
    if ((mode == SPI_MODE_0) || (mode == SPI_MODE_1))
    {
        SCI0->SO0 &= ~_0800_SCI_CH3_CLOCK_OUTPUT_1;
    }
    if ((mode == SPI_MODE_2) || (mode == SPI_MODE_3))
    {
        SCI0->SO0 |= _0800_SCI_CH3_CLOCK_OUTPUT_1;
    }
    SCI0->SOE0 |= _0008_SCI_CH3_OUTPUT_ENABLE;
    SCI0->SS0 |= _0008_SCI_CH3_START_TRG_ON;

    /* Set SSn pin */
    SS11_PORT_SETTING();
    /* Set SCLK11 pin */
    SCLKO11_PORT_SETTING();
    /* Set SDI11 pin */
    SDI11_PORT_SETTING();
    /* Set SDO11 pin */
    SDO11_PORT_SETTING();
}

__STATIC_INLINE void SPI_CPU_WRITE(uint8_t data) 
{
	volatile uint8_t dummy;
	SCI0->SCR03 |= (1<<13); 	// DAP = 1
	SPI_WRDR = data;	
	while(SPI_BUSY);
	dummy = SPI_RDDR;
}

__STATIC_INLINE uint8_t SPI_CPU_READ(void) 	
{
	volatile uint8_t tmp;
	SCI0->SCR03 &= ~(1<<13); 	// DAP = 0
	SPI_WRDR = 0xFFU;
	while(SPI_RRDY == 0);
	tmp = SPI_RDDR;
	return (tmp);
}

__STATIC_INLINE void SPI_DMA_WRITE(uint8_t *const tx_buf, uint16_t tx_num) 
{
	SCI0->SCR03 |= (1<<13); 	// DAP = 1
    SCI0->SIR03 = _0004_SCI_SIRMN_FECTMN | _0002_SCI_SIRMN_PECTMN | _0001_SCI_SIRMN_OVCTMN;
    SCI0->SCR03 &= ~_C000_SCI_RECEPTION_TRANSMISSION;
    SCI0->SCR03 |= _8000_SCI_TRANSMISSION;

 	/* write transfer data with DMA */
 	DMAVEC->VEC[DMA_VECTOR_SPI11] = CTRL_DATA_SPI11;
 	DMAVEC->CTRL[CTRL_DATA_SPI11].DMACR = (0 << CTRL_DMACR_SZ_Pos) | (0 << CTRL_DMACR_CHNE_Pos) |
 	                                      (0 << CTRL_DMACR_DAMOD_Pos) | (1 << CTRL_DMACR_SAMOD_Pos) |
 	                                      (0 << CTRL_DMACR_MODE_Pos);
 	DMAVEC->CTRL[CTRL_DATA_SPI11].DMBLS = 1;
 	DMAVEC->CTRL[CTRL_DATA_SPI11].DMACT = tx_num -1;
 	DMAVEC->CTRL[CTRL_DATA_SPI11].DMRLD = tx_num -1;
 	DMAVEC->CTRL[CTRL_DATA_SPI11].DMSAR = (uint32_t)(tx_buf + 1);
 	DMAVEC->CTRL[CTRL_DATA_SPI11].DMDAR = (uint32_t)&SPI_WRDR;

 	/* init DMA registers */
	INT->IF[SPI11_IRQn].IFL = 0;
 	CGC->PER1 |= CGC_PER1_DMAEN_Msk;
 	DMA->DMABAR = DMAVEC_BASE;
 	DMA->DMAEN1 |= (1 << DMA_VECTOR_SPI11 % 8);
	SPI_WRDR = tx_buf[0];

	/* waiting DMA transfer end */
	while(INT->IF[SPI11_IRQn].IFL == 0x00);
	while(SPI_BUSY);
	INT->IF[SPI11_IRQn].IFL = 0;
    SCI0->SCR03 |=  _C000_SCI_RECEPTION_TRANSMISSION;
}

__STATIC_INLINE void SPI_DMA_READ(uint8_t *const rx_buf, uint16_t rx_num) 	
{
	volatile uint8_t dummy = 0xFFU;
	SCI0->SCR03 &= ~(1<<13); 	// DAP = 0
    SCI0->SIR03 = _0004_SCI_SIRMN_FECTMN | _0002_SCI_SIRMN_PECTMN | _0001_SCI_SIRMN_OVCTMN;
    SCI0->SCR03 &= ~_C000_SCI_RECEPTION_TRANSMISSION;
    SCI0->SCR03 |= _4000_SCI_RECEPTION;

#ifdef SPI11_BUFFER_EMPTY
	SCI0->SMR03 |= _0001_SCI_BUFFER_EMPTY; 
	if(sdr_prescale == 0) // SCK = fclk/2
	{
		DMAVEC->VEC[DMA_VECTOR_SPI11] = CTRL_DATA_SPI11;
		DMAVEC->CTRL[CTRL_DATA_SPI11].DMACR = (0 << CTRL_DMACR_SZ_Pos) | (1 << CTRL_DMACR_CHNE_Pos) |
		                                      (0 << CTRL_DMACR_DAMOD_Pos) | (0 << CTRL_DMACR_SAMOD_Pos) |
		                                      (0 << CTRL_DMACR_MODE_Pos);
		DMAVEC->CTRL[CTRL_DATA_SPI11].DMBLS = 1;
		DMAVEC->CTRL[CTRL_DATA_SPI11].DMACT = rx_num;
		DMAVEC->CTRL[CTRL_DATA_SPI11].DMRLD = rx_num;
		DMAVEC->CTRL[CTRL_DATA_SPI11].DMSAR = (uint32_t)&dummy;
		DMAVEC->CTRL[CTRL_DATA_SPI11].DMDAR = (uint32_t)&dummy;

		/* read receive data with DMA */
		DMAVEC->CTRL[CTRL_DATA_SPI11 + 1].DMACR = (0 << CTRL_DMACR_SZ_Pos) | (1 << CTRL_DMACR_CHNE_Pos) |
		                                          (1 << CTRL_DMACR_DAMOD_Pos) | (0 << CTRL_DMACR_SAMOD_Pos) |
		                                          (0 << CTRL_DMACR_MODE_Pos);
		DMAVEC->CTRL[CTRL_DATA_SPI11 + 1].DMBLS = 1;
		DMAVEC->CTRL[CTRL_DATA_SPI11 + 1].DMACT = rx_num;
		DMAVEC->CTRL[CTRL_DATA_SPI11 + 1].DMRLD = rx_num;
		DMAVEC->CTRL[CTRL_DATA_SPI11 + 1].DMSAR = (uint32_t)&SPI_RDDR;
		DMAVEC->CTRL[CTRL_DATA_SPI11 + 1].DMDAR = (uint32_t)rx_buf;

 		/* dummy write to trigger next transfer */
 		DMAVEC->CTRL[CTRL_DATA_SPI11 + 2].DMACR = (0 << CTRL_DMACR_SZ_Pos) | (0 << CTRL_DMACR_CHNE_Pos) |
 			                                      (0 << CTRL_DMACR_DAMOD_Pos) | (0 << CTRL_DMACR_SAMOD_Pos) |
 			                                      (0 << CTRL_DMACR_MODE_Pos);
 		DMAVEC->CTRL[CTRL_DATA_SPI11 + 2].DMBLS = 1;
 		DMAVEC->CTRL[CTRL_DATA_SPI11 + 2].DMACT = rx_num;
 		DMAVEC->CTRL[CTRL_DATA_SPI11 + 2].DMRLD = rx_num;
 		DMAVEC->CTRL[CTRL_DATA_SPI11 + 2].DMSAR = (uint32_t)&dummy;
 		DMAVEC->CTRL[CTRL_DATA_SPI11 + 2].DMDAR = (uint32_t)&SPI_WRDR;

		/* init DMA registers */
		INT->IF[SPI11_IRQn].IFL = 0;
		CGC->PER1 |= CGC_PER1_DMAEN_Msk;
		DMA->DMABAR = DMAVEC_BASE;
		DMA->DMAEN1 |= (1 << DMA_VECTOR_SPI11 % 8);

		/* dummy write to trigger transfer start */
		SPI_WRDR = 0xFFU;
	}
	else 
	{
		/* read receive data with DMA */
		DMAVEC->VEC[DMA_VECTOR_SPI11] = CTRL_DATA_SPI11;
		DMAVEC->CTRL[CTRL_DATA_SPI11].DMACR = (0 << CTRL_DMACR_SZ_Pos) | (1 << CTRL_DMACR_CHNE_Pos) |
		                                      (1 << CTRL_DMACR_DAMOD_Pos) | (0 << CTRL_DMACR_SAMOD_Pos) |
		                                      (0 << CTRL_DMACR_MODE_Pos);
		DMAVEC->CTRL[CTRL_DATA_SPI11].DMBLS = 1;
		DMAVEC->CTRL[CTRL_DATA_SPI11].DMACT = rx_num;
		DMAVEC->CTRL[CTRL_DATA_SPI11].DMRLD = rx_num;
		DMAVEC->CTRL[CTRL_DATA_SPI11].DMSAR = (uint32_t)&SPI_RDDR;
		DMAVEC->CTRL[CTRL_DATA_SPI11].DMDAR = (uint32_t)rx_buf;

 		/* dummy write to trigger next transfer */
 		DMAVEC->CTRL[CTRL_DATA_SPI11 + 1].DMACR = (0 << CTRL_DMACR_SZ_Pos) | (0 << CTRL_DMACR_CHNE_Pos) |
 			                                          (0 << CTRL_DMACR_DAMOD_Pos) | (0 << CTRL_DMACR_SAMOD_Pos) |
 			                                          (0 << CTRL_DMACR_MODE_Pos);
 		DMAVEC->CTRL[CTRL_DATA_SPI11 + 1].DMBLS = 1;
 		DMAVEC->CTRL[CTRL_DATA_SPI11 + 1].DMACT = rx_num;
 		DMAVEC->CTRL[CTRL_DATA_SPI11 + 1].DMRLD = rx_num;
 		DMAVEC->CTRL[CTRL_DATA_SPI11 + 1].DMSAR = (uint32_t)&dummy;
 		DMAVEC->CTRL[CTRL_DATA_SPI11 + 1].DMDAR = (uint32_t)&SPI_WRDR;

		/* init DMA registers */
		INT->IF[SPI11_IRQn].IFL = 0;
		CGC->PER1 |= CGC_PER1_DMAEN_Msk;
		DMA->DMABAR = DMAVEC_BASE;

		/* dummy write to trigger transfer start */
		SPI_WRDR = 0xFFU;
		DMA->DMAEN1 |= (1 << DMA_VECTOR_SPI11 % 8);
		SPI_WRDR = 0xFFU;
		INT->IF[SPI11_IRQn].IFL = 0;
	}

#else

	/* read receive data with DMA */
	DMAVEC->VEC[DMA_VECTOR_SPI11] = CTRL_DATA_SPI11;
	DMAVEC->CTRL[CTRL_DATA_SPI11].DMACR = (0 << CTRL_DMACR_SZ_Pos) | (1 << CTRL_DMACR_CHNE_Pos) |
	                                      (1 << CTRL_DMACR_DAMOD_Pos) | (0 << CTRL_DMACR_SAMOD_Pos) |
	                                      (0 << CTRL_DMACR_MODE_Pos);
	DMAVEC->CTRL[CTRL_DATA_SPI11].DMBLS = 1;
	DMAVEC->CTRL[CTRL_DATA_SPI11].DMACT = rx_num;
	DMAVEC->CTRL[CTRL_DATA_SPI11].DMRLD = rx_num;
	DMAVEC->CTRL[CTRL_DATA_SPI11].DMSAR = (uint32_t)&SPI_RDDR;
	DMAVEC->CTRL[CTRL_DATA_SPI11].DMDAR = (uint32_t)rx_buf;

    /* dummy write to trigger next transfer */
 	DMAVEC->CTRL[CTRL_DATA_SPI11 + 1].DMACR = (0 << CTRL_DMACR_SZ_Pos) | (0 << CTRL_DMACR_CHNE_Pos) |
 	                                          (0 << CTRL_DMACR_DAMOD_Pos) | (0 << CTRL_DMACR_SAMOD_Pos) |
 	                                          (0 << CTRL_DMACR_MODE_Pos);
 	DMAVEC->CTRL[CTRL_DATA_SPI11 + 1].DMBLS = 1;
 	DMAVEC->CTRL[CTRL_DATA_SPI11 + 1].DMACT = rx_num;
 	DMAVEC->CTRL[CTRL_DATA_SPI11 + 1].DMRLD = rx_num;
 	DMAVEC->CTRL[CTRL_DATA_SPI11 + 1].DMSAR = (uint32_t)&dummy;
 	DMAVEC->CTRL[CTRL_DATA_SPI11 + 1].DMDAR = (uint32_t)&SPI_WRDR;

	/* init DMA registers */
	INT->IF[SPI11_IRQn].IFL = 0;
	CGC->PER1 |= CGC_PER1_DMAEN_Msk;
	DMA->DMABAR = DMAVEC_BASE;

	/* dummy write to trigger transfer start */
	DMA->DMAEN1 |= (1 << DMA_VECTOR_SPI11 % 8);
	SPI_WRDR = 0xFFU;
#endif

	/* waiting DMA transfer end */
	while(INT->IF[SPI11_IRQn].IFL == 0x00);
	while(SPI_BUSY);
	SCI0->SMR03 &= ~_0001_SCI_BUFFER_EMPTY; 
	SCI0->SCR03 |=  _C000_SCI_RECEPTION_TRANSMISSION;
	INT->IF[SPI11_IRQn].IFL = 0;

}

__STATIC_INLINE void FlashSoftReset(void)
{
	/* Enable Reset Instruction */
	SPI_FLASH_CS_CLR(); 			/* chip select active */
	SPI_CPU_WRITE(ENABLE_RESET);
	SPI_FLASH_CS_SET();			/* chip select inactive */

	LOOP_WAIT(16);

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

	LOOP_WAIT(16);

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

	LOOP_WAIT(16);

	/* Sector Erase Instruction */
	SPI_FLASH_CS_CLR(); 			/* chip select active */
	SPI_CPU_WRITE(SECTOR_ERASE);
	SPI_CPU_WRITE(BYTE_FIELD(adr, 2)); // Address [23:16] 
	SPI_CPU_WRITE(BYTE_FIELD(adr, 1)); // Address [15:08] 
	SPI_CPU_WRITE(BYTE_FIELD(adr, 0)); // Address [07:00] 
	SPI_FLASH_CS_SET();			/* chip select inactive */

	LOOP_WAIT(16);

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

	LOOP_WAIT(16);

	/* Block Erase Instruction */
	SPI_FLASH_CS_CLR(); 			/* chip select active */
	SPI_CPU_WRITE(BLOCK_ERASE);
	SPI_CPU_WRITE(BYTE_FIELD(adr, 2)); // Address [23:16] 
	SPI_CPU_WRITE(BYTE_FIELD(adr, 1)); // Address [15:08] 
	SPI_CPU_WRITE(BYTE_FIELD(adr, 0)); // Address [07:00] 
	SPI_FLASH_CS_SET();			/* chip select inactive */

	LOOP_WAIT(16);

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

	LOOP_WAIT(16);

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

	LOOP_WAIT(16);

	/* Page Program Instruction */
	SPI_FLASH_CS_CLR(); 			/* chip select active */
	SPI_CPU_WRITE(PAGE_WRITE);
	SPI_CPU_WRITE(BYTE_FIELD(adr, 2)); // Address [23:16] 
	SPI_CPU_WRITE(BYTE_FIELD(adr, 1)); // Address [15:08] 
	SPI_CPU_WRITE(BYTE_FIELD(adr, 0)); // Address [07:00] 
#ifdef SPI11_WITH_DMA
	SPI_DMA_WRITE(buf, sz);
#else
	while(sz--)
	{
		SPI_CPU_WRITE(*buf++); // Write Data Byte
	}
#endif
	SPI_FLASH_CS_SET();			/* chip select inactive */

	LOOP_WAIT(16);

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
#ifdef SPI11_WITH_DMA
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
#ifdef SPI11_WITH_DMA
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
