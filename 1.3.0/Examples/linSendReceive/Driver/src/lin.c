/***********************************************************************************************************************
* Copyright (C) All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* @file    lin.h
* @brief   This file implements device driver for LIN Send and Receive.
* @version 1.0.0
* @date    2019/12/24
***********************************************************************************************************************/
/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "lin.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: LOOP_WAIT
* @brief  This is a loop wait function.
* @param  n - the wait number 
* @return None
***********************************************************************************************************************/
__STATIC_INLINE void LOOP_WAIT(uint32_t n)
{
    while(n--)
    {
        __NOP();
    }
}

/***********************************************************************************************************************
* Function Name: UART_CPU_WRITE
* @brief  This function implements a UART transmission with polling style.
* @param  data - the data to send
* @return None
***********************************************************************************************************************/
__STATIC_INLINE void UART_CPU_WRITE(uint8_t data)
{
    INTC_DisableIRQ(ST0_IRQn);  /* disable interrupt */
    UART_TXDR = data;
    while(UART_BUSY);
}

/***********************************************************************************************************************
* Function Name: UART_CPU_READ
* @brief  This function implements a UART reception with polling style.
* @param  None
* @return Received data
***********************************************************************************************************************/
__STATIC_INLINE uint8_t UART_CPU_READ(void)
{
    volatile uint8_t tmp;

    INTC_DisableIRQ(ST0_IRQn);  /* disable interrupt */
    while(UART_RRDY);
    tmp = UART_RXDR;
    return (tmp);
}

/***********************************************************************************************************************
* Function Name: LIN_SetConfig
* @brief  This function sets the configure of LIN.
* @param  None
* @return None
***********************************************************************************************************************/
void LIN_SetConfig(void)
{
    MISC->ISC = 0x03; /* RXD0 is used as TI03 and INTP0 input */

    CGC->PER0 |= CGC_PER0_TM40EN_Msk;  /* enables input clock supply */
    TM40->TPS0 = _0000_TM4_CKM3_fCLK_8 | _0000_TM4_CKM2_fCLK_1 | _0000_TM4_CKM1_fCLK_0 | _0000_TM4_CKM0_fCLK_0;

    /* INTP0 setting */
    INTC_DisableIRQ(INTP0_IRQn);  /* disable INTP0 interrupt */
    INTC_ClearPendingIRQ(INTP0_IRQn);  /* clear INTP0 interrupt flag */
    INTM->EGN0 |=  (1 << 0);
    INTM->EGP0 &= ~(1 << 0);
    INTC_EnableIRQ(INTP0_IRQn);  /* enable INTP0 interrupt */

    /* UART0 setting */
    SystemCoreClockUpdate();
    UART0_Init(SystemCoreClock, LIN_BAUDRATE);
}

/***********************************************************************************************************************
* Function Name: LIN_SetBaudRate
* @brief  This function sets the baud rate of LIN reception.
* @param  baudrate - the setting value of the baudrate 
* @return None
***********************************************************************************************************************/
void LIN_SetBaudRate(uint16_t baudrate)
{
    UART_RXSP;  /* Rx stop */
    UART_RXBR = baudrate;
    UART_RXST;  /* Rx start */
}

/***********************************************************************************************************************
* Function Name: LIN_SendWakeUp
* @brief  This function sends the wakeup field of LIN.
* @param  None
* @return None
***********************************************************************************************************************/
void LIN_SendWakeUp(void)
{
    UART_CPU_WRITE(0x80);  /* send wakeup field */
}

/***********************************************************************************************************************
* Function Name: LIN_SendBreak
* @brief  This function sends the break field of LIN.
* @param  None
* @return None
***********************************************************************************************************************/
void LIN_SendBreak(void)
{
    uint16_t baudrate, tmp;

    /* change baudrate */
    UART_TXSP;  /* Tx stop */
    baudrate = UART_TXBR >> 9;
    tmp = baudrate * 13 / 9;
    UART_TXBR = tmp << 9;
    UART_TXST;  /* Tx start */

    /* send break field */
    UART_CPU_WRITE(0x00);

    /* restore baudrate */
    UART_TXSP;  /* Tx stop */
    UART_TXBR = baudrate << 9;
    UART_TXST;  /* Tx start */
}

/***********************************************************************************************************************
* Function Name: LIN_SendSync
* @brief  This function sends the Sync field of LIN.
* @param  None
* @return None
***********************************************************************************************************************/
void LIN_SendSync(void)
{
    UART_CPU_WRITE(0x55);  /* send sync field */
}

/***********************************************************************************************************************
* Function Name: LIN_ReceiveBreak
* @brief  This function implements measure the break field of LIN.
* @param  None
* @return None
***********************************************************************************************************************/
void LIN_ReceiveBreak(void)
{
    TM40->TT0 |= _0008_TM4_CH3_STOP_TRG_ON;
    INTC_ClearPendingIRQ(TM03_IRQn);  /* clear interrupt flag */

    /* Channel 3 is used to measure input pulse width */
    TM40->TMR03 = _0000_TM4_CLOCK_SELECT_CKM0 | _0000_TM4_CLOCK_MODE_CKS | _0200_TM4_TRIGGER_TIMN_BOTH |
                  _0080_TM4_TIMN_EDGE_BOTH_LOW | _000C_TM4_MODE_HIGHLOW_MEASURE;

    INTC_EnableIRQ(TM03_IRQn);  /* enable interrupt */
    TM40->TS0 |= _0008_TM4_CH3_START_TRG_ON;

    g_inttm03Taken = 0;
}

/***********************************************************************************************************************
* Function Name: LIN_GetBreakWidth
* @brief  This function gets the width of the break field of LIN.
* @param  None
* @return None
***********************************************************************************************************************/
int16_t LIN_GetBreakWidth(void)
{
    uint32_t width = 0;

    while(g_inttm03Taken < 1);
    width = g_tm40_ch3_width;

    return (width);
}

/***********************************************************************************************************************
* Function Name: LIN_GetBreakWidth
* @brief  This function implements measure the sync field of LIN.
* @param  None
* @return None
***********************************************************************************************************************/
void LIN_ReceiveSync(void)
{
    TM40->TT0 |= _0008_TM4_CH3_STOP_TRG_ON;
    INTC_ClearPendingIRQ(TM03_IRQn);  /* clear interrupt flag */

    /* Channel 3 is used to measure input pulse interval at falling edge */
    TM40->TMR03 = _0000_TM4_CLOCK_SELECT_CKM0 | _0000_TM4_CLOCK_MODE_CKS | _0100_TM4_TRIGGER_TIMN_VALID |
                  _0000_TM4_TIMN_EDGE_FALLING | _0004_TM4_MODE_CAPTURE | _0000_TM4_START_INT_UNUSED;

    INTC_EnableIRQ(TM03_IRQn);  /* enable interrupt */
    TM40->TS0 |= _0008_TM4_CH3_START_TRG_ON;

    g_inttm03Taken = 0;
}

/***********************************************************************************************************************
* Function Name: LIN_GetBreakWidth
* @brief  This function gets the baud rate by measure the sync field of LIN.
* @param  None
* @return the setting value of baudrate
***********************************************************************************************************************/
uint16_t LIN_GetBaudRate(void)
{
    uint32_t period = 0;
    uint8_t  tmp;

    while(g_inttm03Taken < 2);
    period = g_tm40_ch3_width;
    /* calculate baudrate value */
    tmp = period / sps_tbl[UART_PRS] / 4 - 1;

    return (tmp << 9);
}

#ifdef POLLING_STYLE  /* polling-based */
/***********************************************************************************************************************
* Function Name: LIN_Send
* @brief  This function sends the LIN Frame with polling method.
* @param  pFrame - the pointer of LIN Frame
* @return None
***********************************************************************************************************************/
void LIN_Send(lin_frame_t *pFrame)
{
    uint8_t i;
    uint8_t *ptr = (uint8_t *)pFrame;

    INTC_DisableIRQ(ST0_IRQn);  /* disable interrupt */

    UART_CPU_WRITE(0x80);  /* send wakeup field */
    LOOP_WAIT(RESPONSE_TIME);
    LIN_SendBreak();       /* send break field */
    LOOP_WAIT(RESPONSE_TIME);
    UART_CPU_WRITE(0x55);  /* send sync field */
    LOOP_WAIT(RESPONSE_TIME);
    for (i=0; i<sizeof(lin_frame_t); i++)
    {
        UART_CPU_WRITE(ptr[i]); 
    }
}

/***********************************************************************************************************************
* Function Name: LIN_Receive
* @brief  This function receives the LIN Frame with polling method.
* @param  pFrame - the pointer of LIN Frame
* @return None
***********************************************************************************************************************/
void LIN_Receive(lin_fram_t *pFrame)
{
    uint8_t i;
    uint8_t *ptr = (uint8_t *)pFrame;

    INTC_DisableIRQ(SR0_IRQn);  /* disable interrupt */
    for (i=0; i<sizeof(lin_frame_t); i++)
    {
        *ptr++ = UART_CPU_READ();
    }
}

#else  /* interrupt-based */
/***********************************************************************************************************************
* Function Name: LIN_Send
* @brief  This function sends the LIN Frame with interrupt method.
* @param  tx_buf - the pointer of LIN Frame
* @param  tx_num - the length of LIN Frame
* @return None
***********************************************************************************************************************/
void LIN_Send(uint8_t * tx_buf, uint8_t tx_num)
{
    INTC_EnableIRQ(ST0_IRQn);  /* enable interrupt */
    gp_uart0_tx_address = tx_buf;
    g_uart0_tx_count = tx_num;
    UART_TXDR = *gp_uart0_tx_address;
    gp_uart0_tx_address++;
    g_uart0_tx_count--;
}

/***********************************************************************************************************************
* Function Name: LIN_Receive
* @brief  This function receives the LIN Frame with interrupt method.
* @param  rx_buf - the pointer of LIN Frame
* @param  rx_num - the length of LIN Frame
* @return None
***********************************************************************************************************************/
void LIN_Receive(uint8_t * rx_buf, uint8_t rx_num)
{
    INTC_EnableIRQ(SR0_IRQn);  /* enable interrupt */
    g_uart0_rx_count = 0U;
    g_uart0_rx_length = rx_num;
    gp_uart0_rx_address = rx_buf;
}

#endif

