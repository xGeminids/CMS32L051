/***********************************************************************************************************************
* Copyright (C) All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* @file    adc_user.c
* @brief   This file implements device driver for ADC module.
* @version 1.0.0 
* @date    2019/12/24
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "CMS32L051.h"
#include "adc.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "userdefine.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
void IRQ21_Handler(void) __attribute__((alias("adc_interrupt")));

/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
volatile uint16_t * gp_adc_buf;          /* adc 12 bit result buffer address */
volatile uint32_t   g_AdcIntTaken;      /* adc interrupt flag */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: adc_interrupt
* @brief  ADC interrupt service routine
* @param  None
* @return None
***********************************************************************************************************************/
void adc_interrupt(void)
{
    INTC_ClearPendingIRQ(ADC_IRQn);     /* clear INTAD interrupt flag */
    /* Start user code. Do not edit comment generated here */
    DMA->DMAEN0 = 1<<5;  	/* A/D transfer end */
    g_AdcIntTaken++;
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
