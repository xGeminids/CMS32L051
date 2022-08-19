/***********************************************************************************************************************
* Copyright (C) All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* @file    tim4.c
* @brief   This file implements device driver for TM4 module.
* @version 1.0.0
* @date    2019/12/24
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "CMS32L051.h"
#include "userdefine.h"
#include "tim4.h"
#include "gpio.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* For TM40_ch1 pulse measurement */
extern volatile uint32_t g_tau0_ch1_width;

/***********************************************************************************************************************
* Function Name: TM40_IntervalTimer
* @brief  TM40 specified channel operates as interval timer mode.
* @param  ch - specify the channel number
* @param  num - specify the number count clock
* @return None
***********************************************************************************************************************/
void TM40_IntervalTimer(tm4_channel_t ch, uint16_t num)
{
    CGC->PER0 |= CGC_PER0_TM40EN_Msk;    /* enables input clock supply */
    TM40->TPS0 = (0 << TM40_TPS0_PRS03_Pos) | (0 << TM40_TPS0_PRS02_Pos) | (0xf << TM40_TPS0_PRS01_Pos) | (0 << TM40_TPS0_PRS00_Pos); 

    /* stop specify channel */
    TM40_Channel_Stop(ch);  
    
    switch(ch) 
    {
        case TM4_CHANNEL_0: 
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM00_IRQn);
        /* Channel 0 is used as interval timer */
            TM40->TMR00 = (2 << TM40_TMR00_CKS_Pos) | (0 << TM40_TMR00_STS_Pos) | 
                         (0 << TM40_TMR00_CIS_Pos) | (0 << TM40_TMR00_MD_Pos);
            TM40->TDR00 = num - 1;
            TM40->TO0 &= (uint16_t)~TM40_TO0_TO00_Msk;
            TM40->TOE0 &= (uint16_t)~TM40_TOE0_TOE00_Msk;
            break;
        case TM4_CHANNEL_1: 
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM01_IRQn);
        /* Channel 1 is used as interval timer */
            TM40->TMR01 = (2 << TM40_TMR01_CKS_Pos) | (0 << TM40_TMR01_STS_Pos) | 
                         (0 << TM40_TMR01_CIS_Pos) | (0 << TM40_TMR01_MD_Pos);
            TM40->TDR01 = num - 1;
            TM40->TO0 &= (uint16_t)~TM40_TO0_TO01_Msk;
            TM40->TOE0 &= (uint16_t)~TM40_TOE0_TOE01_Msk;
            break;
        case TM4_CHANNEL_2:
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM02_IRQn);
        /* Channel 2 is used as interval timer */
            TM40->TMR02 = (2 << TM40_TMR02_CKS_Pos) | (0 << TM40_TMR02_STS_Pos) | 
                         (0 << TM40_TMR02_CIS_Pos) | (0 << TM40_TMR02_MD_Pos);
            TM40->TDR02 = num - 1;
            TM40->TO0 &= (uint16_t)~TM40_TO0_TO02_Msk;
            TM40->TOE0 &= (uint16_t)~TM40_TOE0_TOE02_Msk;
            break;
        case TM4_CHANNEL_3:
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM03_IRQn);
        /* Channel 3 is used as interval timer */
            TM40->TMR03 = (2 << TM40_TMR03_CKS_Pos) | (0 << TM40_TMR03_STS_Pos) | 
                         (0 << TM40_TMR03_CIS_Pos) | (0 << TM40_TMR03_MD_Pos);
            TM40->TDR03 = num - 1;
            TM40->TO0 &= (uint16_t)~TM40_TO0_TO03_Msk;
            TM40->TOE0 &= (uint16_t)~TM40_TOE0_TOE03_Msk;
            break;
        default: 
            break;
    }
    /* Start specify channel */
    TM40_Channel_Start(ch);

#if 0   
    /* polling INTTMmn interrupt flag */
    switch(ch) 
    {
        case TM4_CHANNEL_0:
            while(INTC_GetPendingIRQ(TM00_IRQn) == 0);
            INTC_ClearPendingIRQ(TM00_IRQn);
            break;
        case TM4_CHANNEL_1:
            while(INTC_GetPendingIRQ(TM01_IRQn) == 0);
            INTC_ClearPendingIRQ(TM01_IRQn);
            break;
        case TM4_CHANNEL_2:
            while(INTC_GetPendingIRQ(TM02_IRQn) == 0);
            INTC_ClearPendingIRQ(TM02_IRQn);
            break;
        case TM4_CHANNEL_3:
            while(INTC_GetPendingIRQ(TM03_IRQn) == 0);
            INTC_ClearPendingIRQ(TM03_IRQn);
            break;
        default: 
            break;
    }

    /* stop specify channel */
    TM40_Channel_Stop(ch);  
#endif  
}
/***********************************************************************************************************************
* Function Name: TM40_SquareOutput
* @brief  TM40 specified channel operates as square wave output mode.
* @param  ch - specify the channel number
* @param  num - specify the number of count clock 
* @return None
***********************************************************************************************************************/
void TM40_SquareOutput(tm4_channel_t ch, uint16_t num)
{
    CGC->PER0 |= CGC_PER0_TM40EN_Msk;    /* enables input clock supply */
    TM40->TPS0 = (0 << TM40_TPS0_PRS03_Pos) | (0 << TM40_TPS0_PRS02_Pos) | (0 << TM40_TPS0_PRS01_Pos) | (0 << TM40_TPS0_PRS00_Pos); 

    /* stop specify channel */
    TM40_Channel_Stop(ch);  
    
    switch(ch) 
    {
        case TM4_CHANNEL_0: 
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM00_IRQn);
        /* Channel 0 is used as interval timer */
            TM40->TMR00 = (0 << TM40_TMR00_CKS_Pos) | (0 << TM40_TMR00_STS_Pos) | 
                         (0 << TM40_TMR00_CIS_Pos) | (0 << TM40_TMR00_MD_Pos);
            TM40->TDR00 = num - 1;
            TM40->TO0 &= (uint16_t)~TM40_TO0_TO00_Msk;
            TM40->TOE0 |= (uint16_t)TM40_TOE0_TOE00_Msk;
            TO00_PORT_SETTING();
            break;
        case TM4_CHANNEL_1: 
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM01_IRQn);
        /* Channel 1 is used as interval timer */
            TM40->TMR01 = (0 << TM40_TMR01_CKS_Pos) | (0 << TM40_TMR01_STS_Pos) | 
                         (0 << TM40_TMR01_CIS_Pos) | (0 << TM40_TMR01_MD_Pos);
            TM40->TDR01 = num - 1;
            TM40->TOM0 &= (uint16_t)~TM40_TOM0_TOM01_Msk;
            TM40->TOL0 &= (uint16_t)~TM40_TOL0_TOL01_Msk;
            TM40->TO0 &= (uint16_t)~TM40_TO0_TO01_Msk;
            TM40->TOE0 |= (uint16_t)TM40_TOE0_TOE01_Msk;
            TO01_PORT_SETTING();
            break;
        case TM4_CHANNEL_2:
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM02_IRQn);
        /* Channel 2 is used as interval timer */
            TM40->TMR02 = (0 << TM40_TMR02_CKS_Pos) | (0 << TM40_TMR02_STS_Pos) | 
                         (0 << TM40_TMR02_CIS_Pos) | (0 << TM40_TMR02_MD_Pos);
            TM40->TDR02 = num - 1;
            TM40->TOM0 &= (uint16_t)~TM40_TOM0_TOM02_Msk;
            TM40->TOL0 &= (uint16_t)~TM40_TOL0_TOL02_Msk;
            TM40->TO0 &= (uint16_t)~TM40_TO0_TO02_Msk;
            TM40->TOE0 |= (uint16_t)TM40_TOE0_TOE02_Msk;
            TO02_PORT_SETTING();
            break;
        case TM4_CHANNEL_3:
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM03_IRQn);
        /* Channel 3 is used as interval timer */
            TM40->TMR03 = (0 << TM40_TMR03_CKS_Pos) | (0 << TM40_TMR03_STS_Pos) | 
                         (0 << TM40_TMR03_CIS_Pos) | (0 << TM40_TMR03_MD_Pos);
            TM40->TDR03 = num - 1;
            TM40->TOM0 &= (uint16_t)~TM40_TOM0_TOM03_Msk;
            TM40->TOL0 &= (uint16_t)~TM40_TOL0_TOL03_Msk;
            TM40->TO0 &= (uint16_t)~TM40_TO0_TO03_Msk;
            TM40->TOE0 |= (uint16_t)TM40_TOE0_TOE03_Msk;
            TO03_PORT_SETTING();
            break;
        default: 
            break;
    }
    /* Start specify channel */
    TM40_Channel_Start(ch);
}
/***********************************************************************************************************************
* Function Name: TM40_FreqDivider
* @brief  TM40 specified channel operates as frequency divider mode.
* @param  ch - specify the channel number
* @param  edge - specify the input edge of TI
* @param  num - specify the number of divider
* @return None
***********************************************************************************************************************/
void TM40_FreqDivider(tm4_channel_t ch, tm4_pulse_t edge, uint16_t num)
{
    CGC->PER0 |= CGC_PER0_TM40EN_Msk;    /* enables input clock supply */
    TM40->TPS0 = (0 << TM40_TPS0_PRS03_Pos) | (0 << TM40_TPS0_PRS02_Pos) | (0 << TM40_TPS0_PRS01_Pos) | (0 << TM40_TPS0_PRS00_Pos); 

    /* stop specify channel */
    TM40_Channel_Stop(ch);  
    
    if(ch == TM4_CHANNEL_0) 
    {
        /* clear interrupt flag */
        INTC_ClearPendingIRQ(TM00_IRQn);
        /* Channel 0 is used as interval timer */
        TM40->TMR00 = (TM40_TMR00_CCS_Msk) | (0 << TM40_TMR00_STS_Pos) | 
                     (edge << TM40_TMR00_CIS_Pos) | (0 << TM40_TMR00_MD_Pos);
        TM40->TDR00 = num - 1;
        TM40->TO0 &= (uint16_t)~TM40_TO0_TO00_Msk;
        TM40->TOE0 |= (uint16_t)TM40_TOE0_TOE00_Msk;
        TI00_PORT_SETTING();
        TO00_PORT_SETTING();
    }

    /* Start specify channel */
    TM40_Channel_Start(ch);
}
/***********************************************************************************************************************
* Function Name: TM40_EventCount
* @brief  TM40 specified channel operates as external event counter.
* @param  ch - specify the channel number
* @param  intput - specify the input of TI
* @param  edge - specify the input edge of TI
* @param  num - specify the number of specifed edge of TI
* @return None
***********************************************************************************************************************/
void TM40_EventCount(tm4_channel_t ch, tm4_input_t input, tm4_pulse_t edge, uint16_t num)
{
    CGC->PER0 |= CGC_PER0_TM40EN_Msk;    /* enables input clock supply */
    TM40->TPS0 = (0 << TM40_TPS0_PRS03_Pos) | (0 << TM40_TPS0_PRS02_Pos) | (0 << TM40_TPS0_PRS01_Pos) | (10 << TM40_TPS0_PRS00_Pos); 

    /* stop specify channel */
    TM40_Channel_Stop(ch);  
    
    switch(ch) 
    {
        case TM4_CHANNEL_0: 
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM00_IRQn);
        /* Channel 0 is used to external event counter */
            TM40->TMR00 = (TM40_TMR00_CCS_Msk) | (0 << TM40_TMR00_STS_Pos) | 
                          (edge << TM40_TMR00_CIS_Pos) | (6 << TM40_TMR00_MD_Pos);
            TM40->TDR00 = num - 1;
            TM40->TO0 &= (uint16_t)~TM40_TO0_TO00_Msk;
            TM40->TOE0 &= (uint16_t)~TM40_TOE0_TOE00_Msk;
            MISC->TIOS0 |= input;
            if(input == TM4_CH0_INPUT_TI00)
            {
                MISC->NFEN1 |= MISC_NFEN1_TNFEN00_Msk;
                TI00_PORT_SETTING();
            }
            break;
        case TM4_CHANNEL_1: 
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM01_IRQn);
        /* Channel 1 is used to external event counter */
            TM40->TMR01 = (TM40_TMR01_CCS_Msk) | (0 << TM40_TMR01_STS_Pos) | 
                          (edge << TM40_TMR01_CIS_Pos) | (6 << TM40_TMR01_MD_Pos);
            TM40->TDR01 = num - 1;
            TM40->TO0 &= (uint16_t)~TM40_TO0_TO01_Msk;
            TM40->TOE0 &= (uint16_t)~TM40_TOE0_TOE01_Msk;
            MISC->TIOS0 |= input;
            if(input == TM4_CH1_INPUT_TI01)
            {
                MISC->NFEN1 |= MISC_NFEN1_TNFEN01_Msk;
                TI01_PORT_SETTING();
            }
            break;
        case TM4_CHANNEL_2:
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM02_IRQn);
        /* Channel 2 is used to external event counter */
            TM40->TMR02 = (TM40_TMR02_CCS_Msk) | (0 << TM40_TMR02_STS_Pos) | 
                          (edge << TM40_TMR02_CIS_Pos) | (6 << TM40_TMR02_MD_Pos);
            TM40->TDR02 = num - 1;
            TM40->TO0 &= (uint16_t)~TM40_TO0_TO02_Msk;
            TM40->TOE0 &= (uint16_t)~TM40_TOE0_TOE02_Msk;
            MISC->NFEN1 |= MISC_NFEN1_TNFEN02_Msk;
            TI02_PORT_SETTING();
            break;
        case TM4_CHANNEL_3:
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM03_IRQn);
        /* Channel 3 is used to external event counter */
            TM40->TMR03 = (TM40_TMR03_CCS_Msk) | (0 << TM40_TMR03_STS_Pos) | 
                          (edge << TM40_TMR03_CIS_Pos) | (6 << TM40_TMR03_MD_Pos);
            TM40->TDR03 = num - 1;
            TM40->TO0 &= (uint16_t)~TM40_TO0_TO03_Msk;
            TM40->TOE0 &= (uint16_t)~TM40_TOE0_TOE03_Msk;
            MISC->NFEN1 |= MISC_NFEN1_TNFEN03_Msk;
            TI03_PORT_SETTING();
            break;
        default: 
            break;
    }
    /* Start specify channel */
    TM40_Channel_Start(ch);
 
#if 0		
    /* polling INTTMmn interrupt flag */
    switch(ch) 
    {
        case TM4_CHANNEL_0:
            while(INTC_GetPendingIRQ(TM00_IRQn) == 0);
            INTC_ClearPendingIRQ(TM00_IRQn);
            break;
        case TM4_CHANNEL_1:
            while(INTC_GetPendingIRQ(TM01_IRQn) == 0);
            INTC_ClearPendingIRQ(TM01_IRQn);
            break;
        case TM4_CHANNEL_2:
            while(INTC_GetPendingIRQ(TM02_IRQn) == 0);
            INTC_ClearPendingIRQ(TM02_IRQn);
            break;
        case TM4_CHANNEL_3:
            while(INTC_GetPendingIRQ(TM03_IRQn) == 0);
            INTC_ClearPendingIRQ(TM03_IRQn);
            break;
        default: 
            break;
    }

    /* stop specify channel */
    TM40_Channel_Stop(ch);  
 #endif   
}
/***********************************************************************************************************************
* Function Name: TM40_Get_PulseInterval
* @brief  This function measures the puluse interval with polling mode for TM4 module.
* @param  ch - specify the channel number
* @param  intput - specify the input of TI
* @param  edge - specify the input edge of TI
* @return width - return the pulse interval at specified edge
***********************************************************************************************************************/
uint32_t TM40_Get_PulseInterval(tm4_channel_t ch, tm4_input_t input, tm4_pulse_t edge)
{
    uint32_t width;

    CGC->PER0 |= CGC_PER0_TM40EN_Msk;    /* enables input clock supply */
    TM40->TPS0 = (0 << TM40_TPS0_PRS03_Pos) | (0 << TM40_TPS0_PRS02_Pos) | (0 << TM40_TPS0_PRS01_Pos) | (0 << TM40_TPS0_PRS00_Pos); 

    /* stop specify channel */
    TM40_Channel_Stop(ch);  
    
    switch(ch) 
    {
        case TM4_CHANNEL_0:
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM00_IRQn);
        /* Channel 0 is used to measure input pulse interval */
            TM40->TMR00 = (0 << TM40_TMR00_CKS_Pos) | (1 << TM40_TMR00_STS_Pos) | 
                          (edge << TM40_TMR00_CIS_Pos) | (4 << TM40_TMR00_MD_Pos);
            TM40->TO0 &= (uint16_t)~TM40_TO0_TO00_Msk;
            TM40->TOE0 &= (uint16_t)~TM40_TOE0_TOE00_Msk;
            MISC->TIOS0 |= input;
            if(input == TM4_CH0_INPUT_TI00)
            {
                MISC->NFEN1 |= MISC_NFEN1_TNFEN00_Msk;
                TI00_PORT_SETTING();
            }
            break;
        case TM4_CHANNEL_1:
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM01_IRQn);
        /* Channel 1 is used to measure input pulse interval */
            TM40->TMR01 = (0 << TM40_TMR01_CKS_Pos) | (1 << TM40_TMR01_STS_Pos) | 
                          (edge << TM40_TMR01_CIS_Pos) | (4 << TM40_TMR01_MD_Pos);
            TM40->TO0 &= (uint16_t)~TM40_TO0_TO01_Msk;
            TM40->TOE0 &= (uint16_t)~TM40_TOE0_TOE01_Msk;
            if(input == TM4_CH1_INPUT_TI01)
            {
                MISC->NFEN1 |= MISC_NFEN1_TNFEN01_Msk;
                TI01_PORT_SETTING();
            }
            break;
        case TM4_CHANNEL_2:
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM02_IRQn);
        /* Channel 2 is used to measure input pulse interval */
            TM40->TMR02 = (0 << TM40_TMR02_CKS_Pos) | (1 << TM40_TMR02_STS_Pos) | 
                          (edge << TM40_TMR02_CIS_Pos) | (4 << TM40_TMR02_MD_Pos);
            TM40->TO0 &= (uint16_t)~TM40_TO0_TO02_Msk;
            TM40->TOE0 &= (uint16_t)~TM40_TOE0_TOE02_Msk;
            MISC->NFEN1 |= MISC_NFEN1_TNFEN02_Msk;
            TI02_PORT_SETTING();
            break;
        case TM4_CHANNEL_3:
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM03_IRQn);
        /* Channel 3 is used to measure input pulse interval */
            TM40->TMR03 = (0 << TM40_TMR03_CKS_Pos) | (1 << TM40_TMR03_STS_Pos) | 
                          (edge << TM40_TMR03_CIS_Pos) | (4 << TM40_TMR03_MD_Pos);
            TM40->TO0 &= (uint16_t)~TM40_TO0_TO03_Msk;
            TM40->TOE0 &= (uint16_t)~TM40_TOE0_TOE03_Msk;
            MISC->NFEN1 |= MISC_NFEN1_TNFEN03_Msk;
            TI03_PORT_SETTING();
            break;
        default: 
            break;
    }
    /* Start specify channel */
    TM40_Channel_Start(ch);
    
    /* polling INTTMmn interrupt flag */
    switch(ch) 
    {
        case TM4_CHANNEL_0:
            while(INTC_GetPendingIRQ(TM00_IRQn) == 0);
            INTC_ClearPendingIRQ(TM00_IRQn);
            while(INTC_GetPendingIRQ(TM00_IRQn) == 0);
            INTC_ClearPendingIRQ(TM00_IRQn);
            if (1U == (TM40->TSR00 & TM40_TSR00_OVF_Msk)) {   /* overflow occurs */
                width = (uint32_t)(TM40->TDR00 + 1) + 0x10000U;  // read data after 2nd interrupt
            }
            else {
                width = (uint32_t)(TM40->TDR00 + 1);  // read data after 2nd interrupt
            }
            break;
        case TM4_CHANNEL_1:
            while(INTC_GetPendingIRQ(TM01_IRQn) == 0);
            INTC_ClearPendingIRQ(TM01_IRQn);
            while(INTC_GetPendingIRQ(TM01_IRQn) == 0);
            INTC_ClearPendingIRQ(TM01_IRQn);
            if (1U == (TM40->TSR01 & TM40_TSR01_OVF_Msk)) {   /* overflow occurs */
                width = (uint32_t)(TM40->TDR01 + 1) + 0x10000U;  // read data after 2nd interrupt
            }
            else {
                width = (uint32_t)(TM40->TDR01 + 1);  // read data after 2nd interrupt
            }
            break;
        case TM4_CHANNEL_2:
            while(INTC_GetPendingIRQ(TM02_IRQn) == 0);
            INTC_ClearPendingIRQ(TM02_IRQn);
            while(INTC_GetPendingIRQ(TM02_IRQn) == 0);
            INTC_ClearPendingIRQ(TM02_IRQn);
            if (1U == (TM40->TSR02 & TM40_TSR02_OVF_Msk)) {   /* overflow occurs */
                width = (uint32_t)(TM40->TDR02 + 1) + 0x10000U;  // read data after 2nd interrupt
            }
            else {
                width = (uint32_t)(TM40->TDR02 + 1);  // read data after 2nd interrupt
            }
            break;
        case TM4_CHANNEL_3:
            while(INTC_GetPendingIRQ(TM03_IRQn) == 0);
            INTC_ClearPendingIRQ(TM03_IRQn);
            while(INTC_GetPendingIRQ(TM03_IRQn) == 0);
            INTC_ClearPendingIRQ(TM03_IRQn);
            if (1U == (TM40->TSR03 & TM40_TSR03_OVF_Msk)) {   /* overflow occurs */
                width = (uint32_t)(TM40->TDR03 + 1) + 0x10000U;  // read data after 2nd interrupt
            }
            else {
                width = (uint32_t)(TM40->TDR03 + 1);  // read data after 2nd interrupt
            }
            break;
        default: 
            break;
    }

    /* stop specify channel */
    TM40_Channel_Stop(ch);  
    
    return (width); 
}
/***********************************************************************************************************************
* Function Name: TM40_Get_PulseWidth
* @brief  This function initializes the TM4 module.
* @param  ch - specify the channel number
* @param  intput - specify the input of TI
* @param  level - specify the input levle of TI
* @return width - return the pulse width at specified level
***********************************************************************************************************************/
uint32_t TM40_Get_PulseWidth(tm4_channel_t ch, tm4_input_t input, tm4_pulse_t level)
{
    uint32_t width;

    CGC->PER0 |= CGC_PER0_TM40EN_Msk;    /* enables input clock supply */
    TM40->TPS0 = (0 << TM40_TPS0_PRS03_Pos) | (0 << TM40_TPS0_PRS02_Pos) | (0 << TM40_TPS0_PRS01_Pos) | (0 << TM40_TPS0_PRS00_Pos); 

    /* stop specify channel */
    TM40_Channel_Stop(ch);  
    
    switch(ch) 
    {
        case TM4_CHANNEL_0: 
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM00_IRQn);
        /* Channel 0 is used to measure input pulse width */
            TM40->TMR00 = (0 << TM40_TMR00_CKS_Pos) | (2 << TM40_TMR00_STS_Pos) | 
                         (level << TM40_TMR00_CIS_Pos) | (0xC << TM40_TMR00_MD_Pos);
            TM40->TO0 &= (uint16_t)~TM40_TO0_TO00_Msk;
            TM40->TOE0 &= (uint16_t)~TM40_TOE0_TOE00_Msk;
            MISC->TIOS0 |= input;
            if(input == TM4_CH0_INPUT_TI00)
            {
                MISC->NFEN1 |= MISC_NFEN1_TNFEN00_Msk;
                TI00_PORT_SETTING();
            }
            break;
        case TM4_CHANNEL_1: 
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM01_IRQn);
        /* Channel 1 is used to measure input pulse width */
            MISC->TIOS0 |= input;
            TM40->TMR01 = (0 << TM40_TMR01_CKS_Pos) | (2 << TM40_TMR01_STS_Pos) | 
                         (level << TM40_TMR01_CIS_Pos) | (0xC << TM40_TMR01_MD_Pos);
            TM40->TO0 &= (uint16_t)~TM40_TO0_TO01_Msk;
            TM40->TOE0 &= (uint16_t)~TM40_TOE0_TOE01_Msk;
            if(input == TM4_CH1_INPUT_TI01)
            {
                MISC->NFEN1 |= MISC_NFEN1_TNFEN01_Msk;
                TI01_PORT_SETTING();
            }
            break;
        case TM4_CHANNEL_2:
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM02_IRQn);
        /* Channel 2 is used to measure input pulse width */
            TM40->TMR02 = (0 << TM40_TMR02_CKS_Pos) | (2 << TM40_TMR02_STS_Pos) | 
                         (level << TM40_TMR02_CIS_Pos) | (0xC << TM40_TMR02_MD_Pos);
            TM40->TO0 &= (uint16_t)~TM40_TO0_TO02_Msk;
            TM40->TOE0 &= (uint16_t)~TM40_TOE0_TOE02_Msk;
            MISC->NFEN1 |= MISC_NFEN1_TNFEN02_Msk;
            TI02_PORT_SETTING();
            break;
        case TM4_CHANNEL_3:
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM03_IRQn);
        /* Channel 3 is used to measure input pulse width */
            TM40->TMR03 = (0 << TM40_TMR03_CKS_Pos) | (2 << TM40_TMR03_STS_Pos) | 
                         (level << TM40_TMR03_CIS_Pos) | (0xC << TM40_TMR03_MD_Pos);
            TM40->TO0 &= (uint16_t)~TM40_TO0_TO03_Msk;
            TM40->TOE0 &= (uint16_t)~TM40_TOE0_TOE03_Msk;
            MISC->NFEN1 |= MISC_NFEN1_TNFEN03_Msk;
            TI03_PORT_SETTING();
            break;
        default: 
            break;
    }

    /* Start specify channel */
    TM40_Channel_Start(ch);
    
    /* polling INTTMmn interrupt flag */
    switch(ch) 
    {
        case TM4_CHANNEL_0:
            while(INTC_GetPendingIRQ(TM00_IRQn) == 0);
            INTC_ClearPendingIRQ(TM00_IRQn);
            if (1U == (TM40->TSR00 & TM40_TSR00_OVF_Msk)) {   /* overflow occurs */
                width = (uint32_t)(TM40->TDR00 + 1) + 0x10000U;  // read data after interrupt
            }
            else {
                width = (uint32_t)(TM40->TDR00 + 1);  // read data after interrupt
            }
            break;
        case TM4_CHANNEL_1:
            while(INTC_GetPendingIRQ(TM01_IRQn) == 0);
            INTC_ClearPendingIRQ(TM01_IRQn);
            if (1U == (TM40->TSR01 & TM40_TSR01_OVF_Msk)) {   /* overflow occurs */
                width = (uint32_t)(TM40->TDR01 + 1) + 0x10000U;  // read data after interrupt
            }
            else {
                width = (uint32_t)(TM40->TDR01 + 1);  // read data after interrupt
            }
            break;
        case TM4_CHANNEL_2:
            while(INTC_GetPendingIRQ(TM02_IRQn) == 0);
            INTC_ClearPendingIRQ(TM02_IRQn);
            if (1U == (TM40->TSR02 & TM40_TSR02_OVF_Msk)) {   /* overflow occurs */
                width = (uint32_t)(TM40->TDR02 + 1) + 0x10000U;  // read data after interrupt
            }
            else {
                width = (uint32_t)(TM40->TDR02 + 1);  // read data after interrupt
            }
            break;
        case TM4_CHANNEL_3:
            while(INTC_GetPendingIRQ(TM03_IRQn) == 0);
            INTC_ClearPendingIRQ(TM03_IRQn);
            if (1U == (TM40->TSR03 & TM40_TSR03_OVF_Msk)) {   /* overflow occurs */
                width = (uint32_t)(TM40->TDR03 + 1) + 0x10000U;  // read data after interrupt
            }
            else {
                width = (uint32_t)(TM40->TDR03 + 1);  // read data after interrupt
            }
            break;
        default: 
            break;
    }

    /* stop specify channel */
    TM40_Channel_Stop(ch);  
    
    return (width); 
}
/***********************************************************************************************************************
* Function Name: TM40_DelayCounter
* @brief  This function initializes the TM4 module.
* @param  ch - specify the channel number
* @param  intput - specify the input of TI
* @param  edge - specify the input edge of TI
* @param  delay - specify the delay number of TI
* @return None
***********************************************************************************************************************/
void TM40_DelayCounter(tm4_channel_t ch, tm4_input_t input, tm4_pulse_t edge, uint16_t delay)
{
    CGC->PER0 |= CGC_PER0_TM40EN_Msk;    /* enables input clock supply */
    TM40->TPS0 = (0 << TM40_TPS0_PRS03_Pos) | (0 << TM40_TPS0_PRS02_Pos) | (0 << TM40_TPS0_PRS01_Pos) | (0 << TM40_TPS0_PRS00_Pos); 

    /* stop specify channel */
    TM40_Channel_Stop(ch);  
    
    switch(ch) 
    {
        case TM4_CHANNEL_0: 
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM00_IRQn);
        /* Channel 0 is used to measure input pulse width */
            TM40->TMR00 = (0 << TM40_TMR00_CKS_Pos) | (1 << TM40_TMR00_STS_Pos) | 
                         (edge << TM40_TMR00_CIS_Pos) | (9 << TM40_TMR00_MD_Pos);
            TM40->TDR00 = delay - 1;
            TM40->TO0 &= (uint16_t)~TM40_TO0_TO00_Msk;
            TM40->TOE0 &= (uint16_t)~TM40_TOE0_TOE00_Msk;
            MISC->TIOS0 |= input;
            if(input == TM4_CH0_INPUT_TI00)
            {
                MISC->NFEN1 |= MISC_NFEN1_TNFEN00_Msk;
                TI00_PORT_SETTING();
            }
            break;
        case TM4_CHANNEL_1: 
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM01_IRQn);
        /* Channel 1 is used to measure input pulse width */
            TM40->TMR01 = (0 << TM40_TMR01_CKS_Pos) | (1 << TM40_TMR01_STS_Pos) | 
                         (edge << TM40_TMR01_CIS_Pos) | (9 << TM40_TMR01_MD_Pos);
            TM40->TDR01 = delay - 1;
            TM40->TO0 &= (uint16_t)~TM40_TO0_TO01_Msk;
            TM40->TOE0 &= (uint16_t)~TM40_TOE0_TOE01_Msk;
            MISC->TIOS0 |= input;
            if(input == TM4_CH1_INPUT_TI01)
            {
                MISC->NFEN1 |= MISC_NFEN1_TNFEN01_Msk;
                TI01_PORT_SETTING();
            }
            break;
        case TM4_CHANNEL_2:
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM02_IRQn);
        /* Channel 2 is used to measure input pulse width */
            TM40->TMR02 = (0 << TM40_TMR02_CKS_Pos) | (1 << TM40_TMR02_STS_Pos) | 
                         (edge << TM40_TMR02_CIS_Pos) | (9 << TM40_TMR02_MD_Pos);
            TM40->TDR02 = delay - 1;
            TM40->TO0 &= (uint16_t)~TM40_TO0_TO02_Msk;
            TM40->TOE0 &= (uint16_t)~TM40_TOE0_TOE02_Msk;
            MISC->NFEN1 |= MISC_NFEN1_TNFEN02_Msk;
            TI02_PORT_SETTING();
            break;
        case TM4_CHANNEL_3:
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM03_IRQn);
        /* Channel 3 is used to measure input pulse width */
            TM40->TMR03 = (0 << TM40_TMR03_CKS_Pos) | (1 << TM40_TMR03_STS_Pos) | 
                         (edge << TM40_TMR03_CIS_Pos) | (9 << TM40_TMR03_MD_Pos);
            TM40->TDR03 = delay - 1;
            TM40->TO0 &= (uint16_t)~TM40_TO0_TO03_Msk;
            TM40->TOE0 &= (uint16_t)~TM40_TOE0_TOE03_Msk;
            MISC->NFEN1 |= MISC_NFEN1_TNFEN03_Msk;
            TI03_PORT_SETTING();
            break;
        default: 
            break;
    }

    /* Start specify channel */
    TM40_Channel_Start(ch);   
}
/***********************************************************************************************************************
* Function Name: TM40_PWM_Generate
* @brief  This function initializes the TM4 module as PWM function.
* @param  period - the period of PWM waveform
* @param  duty - the duty factor of PWM waveform
* @return None
***********************************************************************************************************************/
void TM40_PWM_Generate(uint16_t period, uint16_t duty)
{
    CGC->PER0 |= CGC_PER0_TM40EN_Msk;    /* enables input clock supply */
    TM40->TPS0 = (0 << TM40_TPS0_PRS03_Pos) | (0 << TM40_TPS0_PRS02_Pos) | (0xf << TM40_TPS0_PRS01_Pos) | (0 << TM40_TPS0_PRS00_Pos); 
    /* master channel */
    TM40->TMR00 = (2 << TM40_TMR00_CKS_Pos) | (0 << TM40_TMR00_STS_Pos) | 
                 (0 << TM40_TMR00_CIS_Pos) | (0x1 << TM40_TMR00_MD_Pos);
    TM40->TDR00 = period - 1;
    TM40->TO0 &= (uint16_t)~TM40_TO0_TO00_Msk;
    TM40->TOE0 &= (uint16_t)~TM40_TOE0_TOE00_Msk;
    /* slave channel */
    TM40->TMR01 = (2 << TM40_TMR01_CKS_Pos) | (4 << TM40_TMR01_STS_Pos) | 
                 (0 << TM40_TMR01_CIS_Pos) | (9 << TM40_TMR01_MD_Pos);
    TM40->TDR01 = duty;
    TM40->TOM0 |= TM40_TOM0_TOM01_Msk;                /* slave channel output mode */
    TM40->TOL0 &= (uint16_t)~TM40_TOL0_TOL01_Msk;     /* positive logic output (active-high) */
    TM40->TO0 &= (uint16_t)~TM40_TO0_TO01_Msk;        /* TOmp default output level */
    TM40->TOE0 |= (uint16_t)TM40_TOE0_TOE01_Msk;      /* Enable TOmp output by counting operation */
    TO01_PORT_SETTING();
    /* slave channel */
    TM40->TMR02 = (2 << TM40_TMR02_CKS_Pos) | (4 << TM40_TMR02_STS_Pos) | 
                 (0 << TM40_TMR02_CIS_Pos) | (9 << TM40_TMR02_MD_Pos);
    TM40->TDR02 = duty;
    TM40->TOM0 |= TM40_TOM0_TOM02_Msk;                /* slave channel output mode */
    TM40->TOL0 &= (uint16_t)~TM40_TOL0_TOL02_Msk;     /* positive logic output (active-high) */
    TM40->TO0 &= (uint16_t)~TM40_TO0_TO02_Msk;        /* TOmp default output level */
    TM40->TOE0 |= (uint16_t)TM40_TOE0_TOE02_Msk;      /* Enable TOmp output by counting operation */
    TO02_PORT_SETTING();
    /* slave channel */
    TM40->TMR03 = (2 << TM40_TMR03_CKS_Pos) | (4 << TM40_TMR03_STS_Pos) | 
                 (0 << TM40_TMR03_CIS_Pos) | (9 << TM40_TMR03_MD_Pos);
    TM40->TDR03 = duty;
    TM40->TOM0 |= TM40_TOM0_TOM03_Msk;                /* slave channel output mode */
    TM40->TOL0 &= (uint16_t)~TM40_TOL0_TOL03_Msk;     /* positive logic output (active-high) */
    TM40->TO0 &= (uint16_t)~TM40_TO0_TO03_Msk;        /* TOmp default output level */
    TM40->TOE0 |= (uint16_t)TM40_TOE0_TOE03_Msk;      /* Enable TOmp output by counting operation */
    TO03_PORT_SETTING();

    /* Start specify channel */
    TM40_Channel_Start(TM4_CHANNEL_0 | TM4_CHANNEL_1 | TM4_CHANNEL_2 | TM4_CHANNEL_3);
}
/***********************************************************************************************************************
* Function Name: TM40_Channel_Start
* @brief  This function starts TM40 channel counter.
* @param  ch - specify the channel number
* @return None
***********************************************************************************************************************/
void TM40_Channel_Start(tm4_channel_t ch)
{
    TM40->TS0 |= ch;
}
/***********************************************************************************************************************
* Function Name: TM4_Channel1_Stop
* @brief  This function stops TM40 channel counter.
* @param  ch - specify the channel number
* @return None
***********************************************************************************************************************/
void TM40_Channel_Stop(tm4_channel_t ch)
{
    TM40->TT0 |= ch;
}
/***********************************************************************************************************************
* Function Name: TM41_IntervalTimer
* @brief  TM41 specified channel operates as interval timer mode.
* @param  ch - specify the channel number
* @param  num - specify the number count clock
* @return None
***********************************************************************************************************************/
void TM41_IntervalTimer(tm4_channel_t ch, uint16_t num)
{
    CGC->PER0 |= CGC_PER0_TM41EN_Msk;    /* enables input clock supply */
    TM41->TPS1 = (0 << TM41_TPS1_PRS13_Pos) | (0 << TM41_TPS1_PRS12_Pos) | (0xf << TM41_TPS1_PRS11_Pos) | (0 << TM41_TPS1_PRS10_Pos); 

    /* stop specify channel */
    TM41_Channel_Stop(ch);  
    
    switch(ch) 
    {
        case TM4_CHANNEL_0: 
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM10_IRQn);
        /* Channel 0 is used as interval timer */
            TM41->TMR10 = (2 << TM41_TMR10_CKS_Pos) | (0 << TM41_TMR10_STS_Pos) | 
                         (0 << TM41_TMR10_CIS_Pos) | (0 << TM41_TMR10_MD_Pos);
            TM41->TDR10 = num - 1;
            TM41->TO1 &= (uint16_t)~TM41_TO1_TO10_Msk;
            TM41->TOE1 &= (uint16_t)~TM41_TOE1_TOE10_Msk;
            break;
        case TM4_CHANNEL_1: 
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM11_IRQn);
        /* Channel 1 is used as interval timer */
            TM41->TMR11 = (2 << TM41_TMR11_CKS_Pos) | (0 << TM41_TMR11_STS_Pos) | 
                         (0 << TM41_TMR11_CIS_Pos) | (0 << TM41_TMR11_MD_Pos);
            TM41->TDR11 = num - 1;
            TM41->TO1 &= (uint16_t)~TM41_TO1_TO11_Msk;
            TM41->TOE1 &= (uint16_t)~TM41_TOE1_TOE11_Msk;
            break;
        case TM4_CHANNEL_2:
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM12_IRQn);
        /* Channel 2 is used as interval timer */
            TM41->TMR12 = (2 << TM41_TMR12_CKS_Pos) | (0 << TM41_TMR12_STS_Pos) | 
                         (0 << TM41_TMR12_CIS_Pos) | (0 << TM41_TMR12_MD_Pos);
            TM41->TDR12 = num - 1;
            TM41->TO1 &= (uint16_t)~TM41_TO1_TO12_Msk;
            TM41->TOE1 &= (uint16_t)~TM41_TOE1_TOE12_Msk;
            break;
        case TM4_CHANNEL_3:
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM13_IRQn);
        /* Channel 3 is used as interval timer */
            TM41->TMR13 = (2 << TM41_TMR13_CKS_Pos) | (0 << TM41_TMR13_STS_Pos) | 
                         (0 << TM41_TMR13_CIS_Pos) | (0 << TM41_TMR13_MD_Pos);
            TM41->TDR13 = num - 1;
            TM41->TO1 &= (uint16_t)~TM41_TO1_TO13_Msk;
            TM41->TOE1 &= (uint16_t)~TM41_TOE1_TOE13_Msk;
            break;
        default: 
            break;
    }
    /* Start specify channel */
    TM41_Channel_Start(ch);

#if 0   
    /* polling INTTMmn interrupt flag */
    switch(ch) 
    {
        case TM4_CHANNEL_0:
            while(INTC_GetPendingIRQ(TM10_IRQn) == 0);
            INTC_ClearPendingIRQ(TM10_IRQn);
            break;
        case TM4_CHANNEL_1:
            while(INTC_GetPendingIRQ(TM11_IRQn) == 0);
            INTC_ClearPendingIRQ(TM11_IRQn);
            break;
        case TM4_CHANNEL_2:
            while(INTC_GetPendingIRQ(TM12_IRQn) == 0);
            INTC_ClearPendingIRQ(TM12_IRQn);
            break;
        case TM4_CHANNEL_3:
            while(INTC_GetPendingIRQ(TM13_IRQn) == 0);
            INTC_ClearPendingIRQ(TM13_IRQn);
            break;
        default: 
            break;
    }

    /* stop specify channel */
    TM41_Channel_Stop(ch);  
#endif  
}
/***********************************************************************************************************************
* Function Name: TM41_SquareOutput
* @brief  TM41 specified channel operates as square wave output mode.
* @param  ch - specify the channel number
* @param  num - specify the number of count clock 
* @return None
***********************************************************************************************************************/
void TM41_SquareOutput(tm4_channel_t ch, uint16_t num)
{
    CGC->PER0 |= CGC_PER0_TM41EN_Msk;    /* enables input clock supply */
    TM41->TPS1 = (0 << TM41_TPS1_PRS13_Pos) | (0 << TM41_TPS1_PRS12_Pos) | (0 << TM41_TPS1_PRS11_Pos) | (0 << TM41_TPS1_PRS10_Pos); 

    /* stop specify channel */
    TM41_Channel_Stop(ch);  
    
    switch(ch) 
    {
        case TM4_CHANNEL_0: 
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM10_IRQn);
        /* Channel 0 is used as interval timer */
            TM41->TMR10 = (0 << TM41_TMR10_CKS_Pos) | (0 << TM41_TMR10_STS_Pos) | 
                         (0 << TM41_TMR10_CIS_Pos) | (0 << TM41_TMR10_MD_Pos);
            TM41->TDR10 = num - 1;
            TM41->TO1 &= (uint16_t)~TM41_TO1_TO10_Msk;
            TM41->TOE1 |= (uint16_t)TM41_TOE1_TOE10_Msk;
            TO10_PORT_SETTING();
            break;
        case TM4_CHANNEL_1: 
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM11_IRQn);
        /* Channel 1 is used as interval timer */
            TM41->TMR11 = (0 << TM41_TMR11_CKS_Pos) | (0 << TM41_TMR11_STS_Pos) | 
                         (0 << TM41_TMR11_CIS_Pos) | (0 << TM41_TMR11_MD_Pos);
            TM41->TDR11 = num - 1;
            TM41->TOM1 &= (uint16_t)~TM41_TOM1_TOM11_Msk;
            TM41->TOL1 &= (uint16_t)~TM41_TOL1_TOL11_Msk;
            TM41->TO1 &= (uint16_t)~TM41_TO1_TO11_Msk;
            TM41->TOE1 |= (uint16_t)TM41_TOE1_TOE11_Msk;
            TO11_PORT_SETTING();
            break;
        case TM4_CHANNEL_2:
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM12_IRQn);
        /* Channel 2 is used as interval timer */
            TM41->TMR12 = (0 << TM41_TMR12_CKS_Pos) | (0 << TM41_TMR12_STS_Pos) | 
                         (0 << TM41_TMR12_CIS_Pos) | (0 << TM41_TMR12_MD_Pos);
            TM41->TDR12 = num - 1;
            TM41->TOM1 &= (uint16_t)~TM41_TOM1_TOM12_Msk;
            TM41->TOL1 &= (uint16_t)~TM41_TOL1_TOL12_Msk;
            TM41->TO1 &= (uint16_t)~TM41_TO1_TO12_Msk;
            TM41->TOE1 |= (uint16_t)TM41_TOE1_TOE12_Msk;
            TO12_PORT_SETTING();
            break;
        case TM4_CHANNEL_3:
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM13_IRQn);
        /* Channel 3 is used as interval timer */
            TM41->TMR13 = (0 << TM41_TMR13_CKS_Pos) | (0 << TM41_TMR13_STS_Pos) | 
                         (0 << TM41_TMR13_CIS_Pos) | (0 << TM41_TMR13_MD_Pos);
            TM41->TDR13 = num - 1;
            TM41->TOM1 &= (uint16_t)~TM41_TOM1_TOM13_Msk;
            TM41->TOL1 &= (uint16_t)~TM41_TOL1_TOL13_Msk;
            TM41->TO1 &= (uint16_t)~TM41_TO1_TO13_Msk;
            TM41->TOE1 |= (uint16_t)TM41_TOE1_TOE13_Msk;
            TO13_PORT_SETTING();
            break;
        default: 
            break;
    }
    /* Start specify channel */
    TM41_Channel_Start(ch);
}
/***********************************************************************************************************************
* Function Name: TM41_FreqDivider
* @brief  TM41 specified channel operates as frequency divider mode.
* @param  ch - specify the channel number
* @param  edge - specify the input edge of TI
* @param  num - specify the number of divider
* @return None
***********************************************************************************************************************/
void TM41_FreqDivider(tm4_channel_t ch, tm4_pulse_t edge, uint16_t num)
{
    CGC->PER0 |= CGC_PER0_TM41EN_Msk;    /* enables input clock supply */
    TM41->TPS1 = (0 << TM41_TPS1_PRS13_Pos) | (0 << TM41_TPS1_PRS12_Pos) | (0 << TM41_TPS1_PRS11_Pos) | (0 << TM41_TPS1_PRS10_Pos); 

    /* stop specify channel */
    TM41_Channel_Stop(ch);  
    
    if(ch == TM4_CHANNEL_0) 
    {
        /* clear interrupt flag */
        INTC_ClearPendingIRQ(TM10_IRQn);
        /* Channel 0 is used as interval timer */
        TM41->TMR10 = (TM41_TMR10_CCS_Msk) | (0 << TM41_TMR10_STS_Pos) | 
                     (edge << TM41_TMR10_CIS_Pos) | (0 << TM41_TMR10_MD_Pos);
        TM41->TDR10 = num;
        TM41->TO1 &= (uint16_t)~TM41_TO1_TO10_Msk;
        TM41->TOE1 |= (uint16_t)TM41_TOE1_TOE10_Msk;
        TI10_PORT_SETTING();
        TO10_PORT_SETTING();
    }
    if(ch == TM4_CHANNEL_1) 
    {
        /* clear interrupt flag */
        INTC_ClearPendingIRQ(TM11_IRQn);
        /* Channel 1 is used as interval timer */
        TM41->TMR11 = (TM41_TMR11_CCS_Msk) | (0 << TM41_TMR11_STS_Pos) | 
                     (edge << TM41_TMR11_CIS_Pos) | (0 << TM41_TMR11_MD_Pos);
        TM41->TDR11 = num;
        TM41->TO1 &= (uint16_t)~TM41_TO1_TO11_Msk;
        TM41->TOE1 |= (uint16_t)TM41_TOE1_TOE11_Msk;
        TI11_PORT_SETTING();
        TO11_PORT_SETTING();
    }
    if(ch == TM4_CHANNEL_2) 
    {
        /* clear interrupt flag */
        INTC_ClearPendingIRQ(TM12_IRQn);
        /* Channel 1 is used as interval timer */
        TM41->TMR12 = (TM41_TMR12_CCS_Msk) | (0 << TM41_TMR12_STS_Pos) | 
                     (edge << TM41_TMR12_CIS_Pos) | (0 << TM41_TMR12_MD_Pos);
        TM41->TDR12 = num;
        TM41->TO1 &= (uint16_t)~TM41_TO1_TO12_Msk;
        TM41->TOE1 |= (uint16_t)TM41_TOE1_TOE12_Msk;
        TI12_PORT_SETTING();
        TO12_PORT_SETTING();
    }
    if(ch == TM4_CHANNEL_3) 
    {
        /* clear interrupt flag */
        INTC_ClearPendingIRQ(TM13_IRQn);
        /* Channel 1 is used as interval timer */
        TM41->TMR13 = (TM41_TMR13_CCS_Msk) | (0 << TM41_TMR13_STS_Pos) | 
                     (edge << TM41_TMR13_CIS_Pos) | (0 << TM41_TMR13_MD_Pos);
        TM41->TDR13 = num;
        TM41->TO1 &= (uint16_t)~TM41_TO1_TO13_Msk;
        TM41->TOE1 |= (uint16_t)TM41_TOE1_TOE13_Msk;
        TI13_PORT_SETTING();
        TO13_PORT_SETTING();
    }

    /* Start specify channel */
    TM41_Channel_Start(ch);

    return(0);      // return OK
}
/***********************************************************************************************************************
* Function Name: TM41_EventCount
* @brief  TM41 specified channel operates as external event counter.
* @param  ch - specify the channel number
* @param  edge - specify the input edge of TI
* @param  num - specify the number of specifed edge of TI
* @return None
***********************************************************************************************************************/
void TM41_EventCount(tm4_channel_t ch, tm4_pulse_t edge, uint16_t num)
{
    CGC->PER0 |= CGC_PER0_TM41EN_Msk;    /* enables input clock supply */
    TM41->TPS1 = (0 << TM41_TPS1_PRS13_Pos) | (0 << TM41_TPS1_PRS12_Pos) | (0 << TM41_TPS1_PRS11_Pos) | (10 << TM41_TPS1_PRS10_Pos); 

    /* stop specify channel */
    TM41_Channel_Stop(ch);  
    
    switch(ch) 
    {
        case TM4_CHANNEL_0: 
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM10_IRQn);
        /* Channel 0 is used to external event counter */
            TM41->TMR10 = (TM41_TMR10_CCS_Msk) | (0 << TM41_TMR10_STS_Pos) | 
                         (edge << TM41_TMR10_CIS_Pos) | (6 << TM41_TMR10_MD_Pos);
            TM41->TDR10 = num - 1;
            TM41->TO1 &= (uint16_t)~TM41_TO1_TO10_Msk;
            TM41->TOE1 &= (uint16_t)~TM41_TOE1_TOE10_Msk;
            MISC->NFEN2 |= MISC_NFEN2_TNFEN10_Msk;
            TI10_PORT_SETTING();
            break;
        case TM4_CHANNEL_1: 
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM11_IRQn);
        /* Channel 1 is used to external event counter */
            TM41->TMR11 = (TM41_TMR11_CCS_Msk) | (0 << TM41_TMR11_STS_Pos) | 
                         (edge << TM41_TMR11_CIS_Pos) | (6 << TM41_TMR11_MD_Pos);
            TM41->TDR11 = num - 1;
            TM41->TO1 &= (uint16_t)~TM41_TO1_TO11_Msk;
            TM41->TOE1 &= (uint16_t)~TM41_TOE1_TOE11_Msk;
            MISC->NFEN2 |= MISC_NFEN2_TNFEN11_Msk;
            TI11_PORT_SETTING();
            break;
        case TM4_CHANNEL_2:
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM12_IRQn);
        /* Channel 2 is used to external event counter */
            TM41->TMR12 = (TM41_TMR12_CCS_Msk) | (0 << TM41_TMR12_STS_Pos) | 
                         (edge << TM41_TMR12_CIS_Pos) | (6 << TM41_TMR12_MD_Pos);
            TM41->TDR12 = num - 1;
            TM41->TO1 &= (uint16_t)~TM41_TO1_TO12_Msk;
            TM41->TOE1 &= (uint16_t)~TM41_TOE1_TOE12_Msk;
            MISC->NFEN2 |= MISC_NFEN2_TNFEN12_Msk;
            TI12_PORT_SETTING();
            break;
        case TM4_CHANNEL_3:
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM13_IRQn);
        /* Channel 3 is used to external event counter */
            TM41->TMR13 = (TM41_TMR13_CCS_Msk) | (0 << TM41_TMR13_STS_Pos) | 
                         (edge << TM41_TMR13_CIS_Pos) | (6 << TM41_TMR13_MD_Pos);
            TM41->TDR13 = num - 1;
            TM41->TO1 &= (uint16_t)~TM41_TO1_TO13_Msk;
            TM41->TOE1 &= (uint16_t)~TM41_TOE1_TOE13_Msk;
            MISC->NFEN2 |= MISC_NFEN2_TNFEN13_Msk;
            TI13_PORT_SETTING();
            break;
        default: 
            break;
    }
    /* Start specify channel */
    TM41_Channel_Start(ch);
    
#if 0
    /* polling INTTMmn interrupt flag */
    switch(ch) 
    {
        case TM4_CHANNEL_0:
            while(INTC_GetPendingIRQ(TM10_IRQn) == 0);
            INTC_ClearPendingIRQ(TM10_IRQn);
            break;
        case TM4_CHANNEL_1:
            while(INTC_GetPendingIRQ(TM11_IRQn) == 0);
            INTC_ClearPendingIRQ(TM11_IRQn);
            break;
        case TM4_CHANNEL_2:
            while(INTC_GetPendingIRQ(TM12_IRQn) == 0);
            INTC_ClearPendingIRQ(TM12_IRQn);
            break;
        case TM4_CHANNEL_3:
            while(INTC_GetPendingIRQ(TM13_IRQn) == 0);
            INTC_ClearPendingIRQ(TM13_IRQn);
            break;
        default: 
            break;
    }

    /* stop specify channel */
    TM41_Channel_Stop(ch);  
#endif
    
    return (0); 
}
/***********************************************************************************************************************
* Function Name: TM41_Get_PulseInterval
* @brief  This function measures the puluse interval with polling mode for TM4 module.
* @param  ch - specify the channel number
* @param  edge - specify the input edge of TI
* @return width - return the pulse interval at specified edge
***********************************************************************************************************************/
uint32_t TM41_Get_PulseInterval(tm4_channel_t ch, tm4_pulse_t edge)
{
    uint32_t width;

    CGC->PER0 |= CGC_PER0_TM41EN_Msk;    /* enables input clock supply */
    TM41->TPS1 = (0 << TM41_TPS1_PRS13_Pos) | (0 << TM41_TPS1_PRS12_Pos) | (0 << TM41_TPS1_PRS11_Pos) | (0 << TM41_TPS1_PRS10_Pos); 

    /* stop specify channel */
    TM41_Channel_Stop(ch);  
    
    switch(ch) 
    {
        case TM4_CHANNEL_0:
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM10_IRQn);
        /* Channel 0 is used to measure input pulse interval */
            TM41->TMR10 = (0 << TM41_TMR10_CKS_Pos) | (1 << TM41_TMR10_STS_Pos) | 
                         (edge << TM41_TMR10_CIS_Pos) | (4 << TM41_TMR10_MD_Pos);
            TM41->TO1 &= (uint16_t)~TM41_TO1_TO10_Msk;
            TM41->TOE1 &= (uint16_t)~TM41_TOE1_TOE10_Msk;
            MISC->NFEN2 |= MISC_NFEN2_TNFEN10_Msk;
            TI10_PORT_SETTING();
            break;
        case TM4_CHANNEL_1:
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM11_IRQn);
        /* Channel 1 is used to measure input pulse interval */
            TM41->TMR11 = (0 << TM41_TMR11_CKS_Pos) | (1 << TM41_TMR11_STS_Pos) | 
                         (edge << TM41_TMR11_CIS_Pos) | (4 << TM41_TMR11_MD_Pos);
            TM41->TO1 &= (uint16_t)~TM41_TO1_TO11_Msk;
            TM41->TOE1 &= (uint16_t)~TM41_TOE1_TOE11_Msk;
            MISC->NFEN2 |= MISC_NFEN2_TNFEN11_Msk;
            TI11_PORT_SETTING();
            break;
        case TM4_CHANNEL_2:
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM12_IRQn);
        /* Channel 2 is used to measure input pulse interval */
            TM41->TMR12 = (0 << TM41_TMR12_CKS_Pos) | (1 << TM41_TMR12_STS_Pos) | 
                         (edge << TM41_TMR12_CIS_Pos) | (4 << TM41_TMR12_MD_Pos);
            TM41->TO1 &= (uint16_t)~TM41_TO1_TO12_Msk;
            TM41->TOE1 &= (uint16_t)~TM41_TOE1_TOE12_Msk;
            MISC->NFEN2 |= MISC_NFEN2_TNFEN12_Msk;
            TI12_PORT_SETTING();
            break;
        case TM4_CHANNEL_3:
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM13_IRQn);
        /* Channel 3 is used to measure input pulse interval */
            TM41->TMR13 = (0 << TM41_TMR13_CKS_Pos) | (1 << TM41_TMR13_STS_Pos) | 
                         (edge << TM41_TMR13_CIS_Pos) | (4 << TM41_TMR13_MD_Pos);
            TM41->TO1 &= (uint16_t)~TM41_TO1_TO13_Msk;
            TM41->TOE1 &= (uint16_t)~TM41_TOE1_TOE13_Msk;
            MISC->NFEN2 |= MISC_NFEN2_TNFEN13_Msk;
            TI13_PORT_SETTING();
            break;
        default: 
            break;
    }
    /* Start specify channel */
    TM41_Channel_Start(ch);
    
    /* polling INTTMmn interrupt flag */
    switch(ch) 
    {
        case TM4_CHANNEL_0:
            while(INTC_GetPendingIRQ(TM10_IRQn) == 0);
            INTC_ClearPendingIRQ(TM10_IRQn);
            while(INTC_GetPendingIRQ(TM10_IRQn) == 0);
            INTC_ClearPendingIRQ(TM10_IRQn);
            if (1U == (TM41->TSR10 & TM41_TSR10_OVF_Msk)) {   /* overflow occurs */
                width = (uint32_t)(TM41->TDR10 + 1) + 0x10000U;  // read data after 2nd interrupt
            }
            else {
                width = (uint32_t)(TM41->TDR10 + 1);  // read data after 2nd interrupt
            }
            break;
        case TM4_CHANNEL_1:
            while(INTC_GetPendingIRQ(TM11_IRQn) == 0);
            INTC_ClearPendingIRQ(TM11_IRQn);
            while(INTC_GetPendingIRQ(TM11_IRQn) == 0);
            INTC_ClearPendingIRQ(TM11_IRQn);
            if (1U == (TM41->TSR11 & TM41_TSR11_OVF_Msk)) {   /* overflow occurs */
                width = (uint32_t)(TM41->TDR11 + 1) + 0x10000U;  // read data after 2nd interrupt
            }
            else {
                width = (uint32_t)(TM41->TDR11 + 1);  // read data after 2nd interrupt
            }
            break;
        case TM4_CHANNEL_2:
            while(INTC_GetPendingIRQ(TM12_IRQn) == 0);
            INTC_ClearPendingIRQ(TM12_IRQn);
            while(INTC_GetPendingIRQ(TM12_IRQn) == 0);
            INTC_ClearPendingIRQ(TM12_IRQn);
            if (1U == (TM41->TSR12 & TM41_TSR12_OVF_Msk)) {   /* overflow occurs */
                width = (uint32_t)(TM41->TDR12 + 1) + 0x10000U;  // read data after 2nd interrupt
            }
            else {
                width = (uint32_t)(TM41->TDR12 + 1);  // read data after 2nd interrupt
            }
            break;
        case TM4_CHANNEL_3:
            while(INTC_GetPendingIRQ(TM13_IRQn) == 0);
            INTC_ClearPendingIRQ(TM13_IRQn);
            while(INTC_GetPendingIRQ(TM13_IRQn) == 0);
            INTC_ClearPendingIRQ(TM13_IRQn);
            if (1U == (TM41->TSR13 & TM41_TSR13_OVF_Msk)) {   /* overflow occurs */
                width = (uint32_t)(TM41->TDR13 + 1) + 0x10000U;  // read data after 2nd interrupt
            }
            else {
                width = (uint32_t)(TM41->TDR13 + 1);  // read data after 2nd interrupt
            }
            break;
        default: 
            break;
    }

    /* stop specify channel */
    TM41_Channel_Stop(ch);  
    
    return (width); 
}
/***********************************************************************************************************************
* Function Name: TM41_Get_PulseWidth
* @brief  This function initializes the TM4 module.
* @param  ch - specify the channel number
* @param  level - specify the input levle of TI
* @return width - return the pulse width at specified level
***********************************************************************************************************************/
uint32_t TM41_Get_PulseWidth(tm4_channel_t ch, tm4_pulse_t level)
{
    uint32_t width;

    CGC->PER0 |= CGC_PER0_TM41EN_Msk;    /* enables input clock supply */
    TM41->TPS1 = (0 << TM41_TPS1_PRS13_Pos) | (0 << TM41_TPS1_PRS12_Pos) | (0 << TM41_TPS1_PRS11_Pos) | (0 << TM41_TPS1_PRS10_Pos); 

    /* stop specify channel */
    TM41_Channel_Stop(ch);  
    
    switch(ch) 
    {
        case TM4_CHANNEL_0: 
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM10_IRQn);
        /* Channel 0 is used to measure input pulse width */
            TM41->TMR10 = (0 << TM41_TMR10_CKS_Pos) | (2 << TM41_TMR10_STS_Pos) | 
                         (level << TM41_TMR10_CIS_Pos) | (0xC << TM41_TMR10_MD_Pos);
            TM41->TO1 &= (uint16_t)~TM41_TO1_TO10_Msk;
            TM41->TOE1 &= (uint16_t)~TM41_TOE1_TOE10_Msk;
            MISC->NFEN2 |= MISC_NFEN2_TNFEN10_Msk;
            TI10_PORT_SETTING();
            break;
        case TM4_CHANNEL_1: 
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM11_IRQn);
        /* Channel 1 is used to measure input pulse width */
            TM41->TMR11 = (0 << TM41_TMR11_CKS_Pos) | (2 << TM41_TMR11_STS_Pos) | 
                         (level << TM41_TMR11_CIS_Pos) | (0xC << TM41_TMR11_MD_Pos);
            TM41->TO1 &= (uint16_t)~TM41_TO1_TO11_Msk;
            TM41->TOE1 &= (uint16_t)~TM41_TOE1_TOE11_Msk;
            MISC->NFEN2 |= MISC_NFEN2_TNFEN11_Msk;
            TI11_PORT_SETTING();
            break;
        case TM4_CHANNEL_2:
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM12_IRQn);
        /* Channel 2 is used to measure input pulse width */
            TM41->TMR12 = (0 << TM41_TMR12_CKS_Pos) | (2 << TM41_TMR12_STS_Pos) | 
                         (level << TM41_TMR12_CIS_Pos) | (0xC << TM41_TMR12_MD_Pos);
            TM41->TO1 &= (uint16_t)~TM41_TO1_TO12_Msk;
            TM41->TOE1 &= (uint16_t)~TM41_TOE1_TOE12_Msk;
            MISC->NFEN2 |= MISC_NFEN2_TNFEN12_Msk;
            TI12_PORT_SETTING();
            break;
        case TM4_CHANNEL_3:
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM13_IRQn);
        /* Channel 3 is used to measure input pulse width */
            TM41->TMR13 = (0 << TM41_TMR13_CKS_Pos) | (2 << TM41_TMR13_STS_Pos) | 
                         (level << TM41_TMR13_CIS_Pos) | (0xC << TM41_TMR13_MD_Pos);
            TM41->TO1 &= (uint16_t)~TM41_TO1_TO13_Msk;
            TM41->TOE1 &= (uint16_t)~TM41_TOE1_TOE13_Msk;
            MISC->NFEN2 |= MISC_NFEN2_TNFEN13_Msk;
            TI13_PORT_SETTING();
            break;
        default: 
            break;
    }

    /* Start specify channel */
    TM41_Channel_Start(ch);
    
    /* polling INTTMmn interrupt flag */
    switch(ch) 
    {
        case TM4_CHANNEL_0:
            while(INTC_GetPendingIRQ(TM10_IRQn) == 0);
            INTC_ClearPendingIRQ(TM10_IRQn);
            if (1U == (TM41->TSR10 & TM41_TSR10_OVF_Msk)) {   /* overflow occurs */
                width = (uint32_t)(TM41->TDR10 + 1) + 0x10000U;  // read data after interrupt
            }
            else {
                width = (uint32_t)(TM41->TDR10 + 1);  // read data after interrupt
            }
            break;
        case TM4_CHANNEL_1:
            while(INTC_GetPendingIRQ(TM11_IRQn) == 0);
            INTC_ClearPendingIRQ(TM11_IRQn);
            if (1U == (TM41->TSR11 & TM41_TSR11_OVF_Msk)) {   /* overflow occurs */
                width = (uint32_t)(TM41->TDR11 + 1) + 0x10000U;  // read data after interrupt
            }
            else {
                width = (uint32_t)(TM41->TDR11 + 1);  // read data after interrupt
            }
            break;
        case TM4_CHANNEL_2:
            while(INTC_GetPendingIRQ(TM12_IRQn) == 0);
            INTC_ClearPendingIRQ(TM12_IRQn);
            if (1U == (TM41->TSR12 & TM41_TSR12_OVF_Msk)) {   /* overflow occurs */
                width = (uint32_t)(TM41->TDR12 + 1) + 0x10000U;  // read data after interrupt
            }
            else {
                width = (uint32_t)(TM41->TDR12 + 1);  // read data after interrupt
            }
            break;
        case TM4_CHANNEL_3:
            while(INTC_GetPendingIRQ(TM13_IRQn) == 0);
            INTC_ClearPendingIRQ(TM13_IRQn);
            if (1U == (TM41->TSR13 & TM41_TSR13_OVF_Msk)) {   /* overflow occurs */
                width = (uint32_t)(TM41->TDR13 + 1) + 0x10000U;  // read data after interrupt
            }
            else {
                width = (uint32_t)(TM41->TDR13 + 1);  // read data after interrupt
            }
            break;
        default: 
            break;
    }

    /* stop specify channel */
    TM41_Channel_Stop(ch);  
    
    return (width); 
}
/***********************************************************************************************************************
* Function Name: TM41_DelayCounter
* @brief  This function initializes the TM4 module.
* @param  ch - specify the channel number
* @param  edge - specify the input edge of TI
* @param  delay - specify the delay number of TI
* @return None
***********************************************************************************************************************/
void TM41_DelayCounter(tm4_channel_t ch, tm4_pulse_t edge, uint16_t delay)
{
    CGC->PER0 |= CGC_PER0_TM41EN_Msk;    /* enables input clock supply */
    TM41->TPS1 = (0 << TM41_TPS1_PRS13_Pos) | (0 << TM41_TPS1_PRS12_Pos) | (0 << TM41_TPS1_PRS11_Pos) | (0 << TM41_TPS1_PRS10_Pos); 

    /* stop specify channel */
    TM41_Channel_Stop(ch);  
    
    switch(ch) 
    {
        case TM4_CHANNEL_0: 
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM10_IRQn);
        /* Channel 0 is used to measure input pulse width */
            TM41->TMR10 = (0 << TM41_TMR10_CKS_Pos) | (1 << TM41_TMR10_STS_Pos) | 
                         (edge << TM41_TMR10_CIS_Pos) | (9 << TM41_TMR10_MD_Pos);
            TM41->TDR10 = delay - 1;
            TM41->TO1 &= (uint16_t)~TM41_TO1_TO10_Msk;
            TM41->TOE1 &= (uint16_t)~TM41_TOE1_TOE10_Msk;
            MISC->NFEN2 |= MISC_NFEN2_TNFEN10_Msk;
            TI10_PORT_SETTING();
            break;
        case TM4_CHANNEL_1: 
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM11_IRQn);
        /* Channel 1 is used to measure input pulse width */
            TM41->TMR11 = (0 << TM41_TMR11_CKS_Pos) | (1 << TM41_TMR11_STS_Pos) | 
                         (edge << TM41_TMR11_CIS_Pos) | (9 << TM41_TMR11_MD_Pos);
            TM41->TDR11 = delay - 1;
            TM41->TO1 &= (uint16_t)~TM41_TO1_TO11_Msk;
            TM41->TOE1 &= (uint16_t)~TM41_TOE1_TOE11_Msk;
            MISC->NFEN2 |= MISC_NFEN2_TNFEN11_Msk;
            TI11_PORT_SETTING();
            break;
        case TM4_CHANNEL_2:
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM12_IRQn);
        /* Channel 2 is used to measure input pulse width */
            TM41->TMR12 = (0 << TM41_TMR12_CKS_Pos) | (1 << TM41_TMR12_STS_Pos) | 
                         (edge << TM41_TMR12_CIS_Pos) | (9 << TM41_TMR12_MD_Pos);
            TM41->TDR12 = delay - 1;
            TM41->TO1 &= (uint16_t)~TM41_TO1_TO12_Msk;
            TM41->TOE1 &= (uint16_t)~TM41_TOE1_TOE12_Msk;
            MISC->NFEN2 |= MISC_NFEN2_TNFEN12_Msk;
            TI12_PORT_SETTING();
            break;
        case TM4_CHANNEL_3:
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM13_IRQn);
        /* Channel 3 is used to measure input pulse width */
            TM41->TMR13 = (0 << TM41_TMR13_CKS_Pos) | (1 << TM41_TMR13_STS_Pos) | 
                         (edge << TM41_TMR13_CIS_Pos) | (9 << TM41_TMR13_MD_Pos);
            TM41->TDR13 = delay - 1;
            TM41->TO1 &= (uint16_t)~TM41_TO1_TO13_Msk;
            TM41->TOE1 &= (uint16_t)~TM41_TOE1_TOE13_Msk;
            MISC->NFEN2 |= MISC_NFEN2_TNFEN13_Msk;
            TI13_PORT_SETTING();
            break;
        default: 
            break;
    }

    /* Start specify channel */
    TM41_Channel_Start(ch);   
}
/***********************************************************************************************************************
* Function Name: TM41_PWM_Generate
* @brief  This function initializes the TM4 module as PWM function.
* @param  period - the period of PWM waveform
* @param  duty - the duty factor of PWM waveform
* @return None
***********************************************************************************************************************/
void TM41_PWM_Generate(uint16_t period, uint16_t duty)
{
    CGC->PER0 |= CGC_PER0_TM41EN_Msk;    /* enables input clock supply */
    TM41->TPS1 = (0 << TM41_TPS1_PRS13_Pos) | (0 << TM41_TPS1_PRS12_Pos) | (0xf << TM41_TPS1_PRS11_Pos) | (0 << TM41_TPS1_PRS10_Pos); 
    /* master channel */
    TM41->TMR10 = (2 << TM41_TMR10_CKS_Pos) | (0 << TM41_TMR10_STS_Pos) | 
                 (0 << TM41_TMR10_CIS_Pos) | (0x1 << TM41_TMR10_MD_Pos);
    TM41->TDR10 = period - 1;
    TM41->TO1 &= (uint16_t)~TM41_TO1_TO10_Msk;
    TM41->TOE1 &= (uint16_t)~TM41_TOE1_TOE10_Msk;
    /* slave channel */
    TM41->TMR11 = (2 << TM41_TMR11_CKS_Pos) | (4 << TM41_TMR11_STS_Pos) | 
                 (0 << TM41_TMR11_CIS_Pos) | (9 << TM41_TMR11_MD_Pos);
    TM41->TDR11 = duty;
    TM41->TOM1 |= TM41_TOM1_TOM11_Msk;                /* slave channel output mode */
    TM41->TOL1 &= (uint16_t)~TM41_TOL1_TOL11_Msk;     /* positive logic output (active-high) */
    TM41->TO1 &= (uint16_t)~TM41_TO1_TO11_Msk;        /* TOmp default output level */
    TM41->TOE1 |= (uint16_t)TM41_TOE1_TOE11_Msk;      /* Enable TOmp output by counting operation */
    TO11_PORT_SETTING();

    /* slave channel */
    TM41->TMR12 = (2 << TM41_TMR12_CKS_Pos) | (4 << TM41_TMR12_STS_Pos) | 
                 (0 << TM41_TMR12_CIS_Pos) | (9 << TM41_TMR12_MD_Pos);
    TM41->TDR12 = duty;
    TM41->TOM1 |= TM41_TOM1_TOM12_Msk;                /* slave channel output mode */
    TM41->TOL1 &= (uint16_t)~TM41_TOL1_TOL12_Msk;     /* positive logic output (active-high) */
    TM41->TO1 &= (uint16_t)~TM41_TO1_TO12_Msk;        /* TOmp default output level */
    TM41->TOE1 |= (uint16_t)TM41_TOE1_TOE12_Msk;      /* Enable TOmp output by counting operation */
    TO12_PORT_SETTING();

    /* slave channel */
    TM41->TMR13 = (2 << TM41_TMR13_CKS_Pos) | (4 << TM41_TMR13_STS_Pos) | 
                 (0 << TM41_TMR13_CIS_Pos) | (9 << TM41_TMR13_MD_Pos);
    TM41->TDR13 = duty;
    TM41->TOM1 |= TM41_TOM1_TOM13_Msk;                /* slave channel output mode */
    TM41->TOL1 &= (uint16_t)~TM41_TOL1_TOL13_Msk;     /* positive logic output (active-high) */
    TM41->TO1 &= (uint16_t)~TM41_TO1_TO13_Msk;        /* TOmp default output level */
    TM41->TOE1 |= (uint16_t)TM41_TOE1_TOE13_Msk;      /* Enable TOmp output by counting operation */
    TO13_PORT_SETTING();

    /* Start specify channel */
    TM41_Channel_Start(TM4_CHANNEL_0 | TM4_CHANNEL_1 | TM4_CHANNEL_2 | TM4_CHANNEL_3);
}
/***********************************************************************************************************************
* Function Name: TM41_Channel_Start
* @brief  This function starts TM41 channel counter.
* @param  ch - specify the channel number
* @return None
***********************************************************************************************************************/
void TM41_Channel_Start(tm4_channel_t ch)
{
    TM41->TS1 |= ch;
}
/***********************************************************************************************************************
* Function Name: TM4_Channel1_Stop
* @brief  This function stops TM41 channel counter.
* @param  ch - specify the channel number
* @return None
***********************************************************************************************************************/
void TM41_Channel_Stop(tm4_channel_t ch)
{
    TM41->TT1 |= ch;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
