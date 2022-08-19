/***********************************************************************************************************************
* Copyright (C) All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* @file    cmp_user.c
* @brief   This file implements device driver for CMP module.
* @version 1.0.0  
* @date    2019/12/24
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "CMS32L051.h"
#include "cmp.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "userdefine.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
void IRQ24_Handler(void) __attribute__((alias("cmp0_interrupt")));
void IRQ25_Handler(void) __attribute__((alias("cmp1_interrupt")));

/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
volatile uint8_t g_Cmp0IntTaken;
volatile uint8_t g_Cmp1IntTaken;
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: cmp0_interrupt
* @brief  CMP0 interrupt service routine
* @param  None
* @return None
***********************************************************************************************************************/
void cmp0_interrupt(void)
{
    INTC_ClearPendingIRQ(CMP0_IRQn);     /* clear INTCMP0 interrupt flag */
    /* Start user code. Do not edit comment generated here */
    g_Cmp0IntTaken++;
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: cmp1_interrupt
* @brief  CMP1 interrupt service routine
* @param  None
* @return None
***********************************************************************************************************************/
void cmp1_interrupt(void)
{
    INTC_ClearPendingIRQ(CMP1_IRQn);     /* clear INTCMP1 interrupt flag */
    /* Start user code. Do not edit comment generated here */
    g_Cmp1IntTaken++;
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
