/***********************************************************************************************************************
* Copyright (C) All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* @file    tim4_user.c
* @brief   This file implements device driver for Timer 4 module.
* @version 1.0.0
* @date    2019/12/24
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "CMS32L051.h"
#include "tim4.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
void IRQ17_Handler(void) __attribute__((alias("tim40_channel0_interrupt")));
void IRQ18_Handler(void) __attribute__((alias("tim40_channel1_interrupt")));
void IRQ19_Handler(void) __attribute__((alias("tim40_channel2_interrupt")));
void IRQ20_Handler(void) __attribute__((alias("tim40_channel3_interrupt")));
void IRQ27_Handler(void) __attribute__((alias("tim41_channel0_interrupt")));
void IRQ28_Handler(void) __attribute__((alias("tim41_channel1_interrupt")));
void IRQ29_Handler(void) __attribute__((alias("tim41_channel2_interrupt")));
void IRQ30_Handler(void) __attribute__((alias("tim41_channel3_interrupt")));
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: tim40_channel0_interrupt
* @brief  This function INTTM00 interrupt service routine.
* @param  None
* @return None
***********************************************************************************************************************/
static void tim40_channel0_interrupt(void)
{
    INTC_ClearPendingIRQ(TM00_IRQn);    /* clear INTTM00 interrupt flag */
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: tim40_channel1_interrupt
* @brief  This function INTTM01 interrupt service routine.
* @param  None
* @return None
***********************************************************************************************************************/
static void tim40_channel1_interrupt(void)
{
    INTC_ClearPendingIRQ(TM01_IRQn);    /* clear INTTM01 interrupt flag */
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: tim40_channel2_interrupt
* @brief  This function INTTM02 interrupt service routine.
* @param  None
* @return None
***********************************************************************************************************************/
static void tim40_channel2_interrupt(void)
{
    INTC_ClearPendingIRQ(TM02_IRQn);    /* clear INTTM02 interrupt flag */
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: tim40_channel3_interrupt
* @brief  This function INTTM03 interrupt service routine.
* @param  None
* @return None
***********************************************************************************************************************/
static void tim40_channel3_interrupt(void)
{
    INTC_ClearPendingIRQ(TM03_IRQn);    /* clear INTTM03 interrupt flag */
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: tim41_channel0_interrupt
* @brief  This function INTTM00 interrupt service routine.
* @param  None
* @return None
***********************************************************************************************************************/
static void tim41_channel0_interrupt(void)
{
    INTC_ClearPendingIRQ(TM10_IRQn);    /* clear INTTM10 interrupt flag */
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: tim41_channel1_interrupt
* @brief  This function INTTM11 interrupt service routine.
* @param  None
* @return None
***********************************************************************************************************************/
static void tim41_channel1_interrupt(void)
{
    INTC_ClearPendingIRQ(TM11_IRQn);    /* clear INTTM11 interrupt flag */
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: tim41_channel2_interrupt
* @brief  This function INTTM12 interrupt service routine.
* @param  None
* @return None
***********************************************************************************************************************/
static void tim41_channel2_interrupt(void)
{
    INTC_ClearPendingIRQ(TM12_IRQn);    /* clear INTTM12 interrupt flag */
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: tim41_channel3_interrupt
* @brief  This function INTTM13 interrupt service routine.
* @param  None
* @return None
***********************************************************************************************************************/
static void tim41_channel3_interrupt(void)
{
    INTC_ClearPendingIRQ(TM13_IRQn);    /* clear INTTM13 interrupt flag */
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
