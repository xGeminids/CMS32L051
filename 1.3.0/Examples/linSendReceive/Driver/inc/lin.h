/***********************************************************************************************************************
* Copyright (C) All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* @file    lin.h
* @brief   This file implements device driver for LIN Send and Receive.
* @version 1.0.0
* @date    2019/12/24
***********************************************************************************************************************/
#ifndef LIN_H
#define LIN_H

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "userdefine.h"
#include "CMS32L051.h"
#include "sci.h"
#include "tim4.h"
#include "intp.h"

/**************************************************************************
* UART Data and Status Register
**************************************************************************/
#define UART_TXDR        (SCI0->TXD0)   /* transmission data register */
#define UART_RXDR        (SCI0->RXD0)   /* reception data register */
#define UART_TXBR        (SCI0->SDR00)  /* transmission baudrate */
#define UART_RXBR        (SCI0->SDR01)  /* reception baudrate */
#define UART_PRS         (SCI0->SPS0 & 0x0FU)  /* prescale value */
#define UART_BUSY        (SCI0->SSR00 & (_0040_SCI_UNDER_EXECUTE | _0020_SCI_VALID_STORED))  /* busy */
#define UART_RRDY        (SCI0->SSR01 & _0020_SCI_VALID_STORED)    /* read ready */
#define UART_TXST        (SCI0->SS0 |= _0001_SCI_CH0_START_TRG_ON) /* transmission start */
#define UART_TXSP        (SCI0->ST0 |= _0001_SCI_CH0_STOP_TRG_ON)  /* transmission stop */
#define UART_RXST        (SCI0->SS0 |= _0002_SCI_CH1_START_TRG_ON) /* reception start */
#define UART_RXSP        (SCI0->ST0 |= _0002_SCI_CH1_STOP_TRG_ON)  /* reception stop */
    
#define LIN_BAUDRATE     (19200)        /* baudrate: 19200bps */
#define RESPONSE_TIME    (1000)

//#define POLLING_STYLE

/**************************************************************************
* Typedef defination
**************************************************************************/
typedef struct
{
    union {
        uint8_t PID;
        struct {
            uint8_t ID    : 6;
            uint8_t P0    : 1;  /*  odd parity: P0 =  (ID0 ^ ID1 ^ ID2 ^ ID4) */
            uint8_t P1    : 1;  /* even parity: P1 = ~(ID1 ^ ID3 ^ ID4 ^ ID5) */
        } PID_b;
    };
    uint8_t Data[8];            /* data field: 1 ~ 8 bytes */
    uint8_t ChkSum;             /* check sum field */
} lin_frame_t;

/**************************************************************************
* Global functions
**************************************************************************/
void LIN_SetConfig(void);
void LIN_SetBaudRate(uint16_t baudrate);
void LIN_SendWakeUp(void);
void LIN_SendBreak(void);
void LIN_SendSync(void);
void LIN_ReceiveBreak(void);
int16_t LIN_GetBreakWidth(void);
void LIN_ReceiveSync(void);
uint16_t LIN_GetBaudRate(void);
#ifdef POLLING_STYLE  /* polling-based */
void LIN_Send(lin_frame_t *pFrame);
void LIN_Receive(lin_fram_t *pFrame);
#else  /* interrupt-based */
void LIN_Send(uint8_t * tx_buf, uint8_t tx_num);
void LIN_Receive(uint8_t * rx_buf, uint8_t rx_num);
#endif

#endif
