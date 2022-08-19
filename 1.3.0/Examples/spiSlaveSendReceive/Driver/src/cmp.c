/***********************************************************************************************************************
* Copyright (C)  All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* @file    cmp.c
* @brief   This file implements device driver for COMP module.
* @version V1.0.0
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
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: CMP_VREF_Init
* @brief  The function initializes the Comparator internal reference voltage.
* @param  None
* @return None
***********************************************************************************************************************/
void CMP_VREF_Init(cmp_channel_t ch, cmp_vref_t vref)
{
    
    if(vref == 0) 
    {
        CMP->CVRCTL = _00_VDD_INTERNAL_REFERVOL | _00_VSS_INTERNAL_REFERVOL;
    }
    if(vref == 1) 
    {
        CMP->CVRCTL = _01_AVREFP_INTERNAL_REFERVOL | _10_AVREFM_INTERNAL_REFERVOL;
    }

    if(ch & 0x01) 
    {
        CMP->CVRCTL = _02_COMP0_REFVOLTAGE_ENABLE;
    }
    if(ch & 0x02) 
    {
        CMP->CVRCTL = _20_COMP1_REFVOLTAGE_ENABLE; 
    }
}

/***********************************************************************************************************************
* Function Name: CMP_VREF_Set_Value
* @brief  This function sets the internal reference voltate [#n] value.
* @param  ch - specify the comp channel number
* @param  reg_value - value of conversion
* @return None
***********************************************************************************************************************/
void CMP_VREF_Set_Value(cmp_channel_t ch, uint8_t regvalue)
{
    if(ch & 0x01) 
    {
        CMP->C0RVM = regvalue;
    }
    if(ch & 0x02) 
    {
        CMP->C1RVM = regvalue; 
    }
}

/***********************************************************************************************************************
* Function Name: CMP_Input_Select
* @brief  This function sets the positive and negative side input of Comparator.
* @param  ch - specify the comp channel number
* @param  positive_side - specify the positive side input of Comparator
* @param  negative_side - specify the negative side input of Comparator
* @return None
***********************************************************************************************************************/
void CMP_Input_Select(cmp_channel_t ch, cmp_side_t positive_side, cmp_side_t negative_side)
{
    if(ch & 0x01)
    {
        CMP->CMPSEL0 = positive_side << 7 | negative_side << 0;
    }
    if(ch & 0x02)
    {
        CMP->CMPSEL1 = positive_side << 6 | negative_side << 0;
    }

}

/***********************************************************************************************************************
* Function Name: CMP_Filter_Setting
* @brief  This function sets the filter and edge detection of Comparator output.
* @param  ch - specify the comp channel number
* @param  fck - specify the clock of digital filter 
* @param  edge - specify the edge detector
* @return None
***********************************************************************************************************************/
void CMP_Filter_Setting(cmp_channel_t ch, cmp_filter_t fck, cmp_edge_t edge)
{
    if(ch & 0x01)
    {
        CMP->COMPFIR = edge << 2 | fck << 0;
    }
    if(ch & 0x02)
    {
        CMP->COMPFIR = edge << 6 | fck << 4;
    }
}

/***********************************************************************************************************************
* Function Name: CMP_Init
* @brief  This function initilize the CMP. 
* @param  ch - specify the comp channel number
* @param  positive_side - specify the input signal of positive side of comparator 
* @param  negative_side - specify the input signal of negative side of comparator 
* @return None
***********************************************************************************************************************/
void CMP_Init(cmp_channel_t ch, cmp_side_t positive_side, cmp_side_t negative_side)
{
    CGC->PER1 |= CGC_PER1_PGACMPEN_Msk;    /* enables input clock supply */
    if(ch & 0x01) 
    {
        if(negative_side == CMP_NSIDE_IVREF) {
            CMP_VREF_Set_Value(CMP_CHANNEL_0, 0x10);
            CMP_VREF_Init     (CMP_CHANNEL_0, CMP_VDD_VSS_REFERENCE_VOLTAGE);
        }
        CMP_Input_Select  (CMP_CHANNEL_0, positive_side, negative_side);
        CMP_Filter_Setting(CMP_CHANNEL_0, CMP_FILTER_FCLK_8, CMP_EDGE_BOTH);
        CMP->COMPOCR = _02_COMP0_OUTPUT_ENABLE | _01_COMP0_INTERRPUT_ENABLE;
        CMP->COMPMDR = _01_COMP0_OPERATION_ENABLED;
        CMP0_PORT_SETTING();
    }
    if(ch & 0x02) 
    {
        if(negative_side == CMP_NSIDE_IVREF) {
            CMP_VREF_Set_Value(CMP_CHANNEL_1, 0x10);
            CMP_VREF_Init     (CMP_CHANNEL_1, CMP_VDD_VSS_REFERENCE_VOLTAGE);
        }
        //CMP_Input_Select  (CMP_CHANNEL_1, CMP_PSIDE_VCIN12, CMP_NSIDE_IVREF);
        CMP_Input_Select  (CMP_CHANNEL_1, positive_side, negative_side);
        CMP_Filter_Setting(CMP_CHANNEL_1, CMP_FILTER_FCLK_8, CMP_EDGE_BOTH);
        CMP->COMPOCR = _20_COMP1_OUTPUT_ENABLE | _10_COMP1_INTERRPUT_ENABLE;
        CMP->COMPMDR = _10_COMP1_OPERATION_ENABLED;
        CMP1_PORT_SETTING();
    }
}

/***********************************************************************************************************************
* Function Name: CMP_Start
* @brief  This function starts the Comparator.
* @param  ch - specify the channel
* @return None
***********************************************************************************************************************/
void CMP_Start(cmp_channel_t ch)
{
    if(ch & 0x01) 
    {
        INTC_ClearPendingIRQ(CMP0_IRQn);    /* clear INTCMP0 interrupt flag */
        INTC_EnableIRQ(CMP0_IRQn);          /* enable INTCMP0 interrupt */
        NVIC_EnableIRQ(CMP0_IRQn);          /* enable INTCMP0 interrupt */
        CMP->COMPMDR = _01_COMP0_OPERATION_ENABLED;
    }

    if(ch & 0x02) 
    {
        INTC_ClearPendingIRQ(CMP1_IRQn);    /* clear INTCMP1 interrupt flag */
        INTC_EnableIRQ(CMP1_IRQn);          /* enable INTCMP1 interrupt */
        NVIC_EnableIRQ(CMP1_IRQn);          /* enable INTCMP1 interrupt */
        CMP->COMPMDR = _10_COMP1_OPERATION_ENABLED;
    }
}
/***********************************************************************************************************************
* Function Name: CMP_Stop
* @brief  This function stops the Comparator.
* @param  ch - specify the channel
* @return None
***********************************************************************************************************************/
void CMP_Stop(cmp_channel_t ch)
{
    if(ch & 0x01) 
    {
        CMP->COMPMDR &= ~_01_COMP0_OPERATION_ENABLED;
        INTC_DisableIRQ(CMP0_IRQn);         /* disable INTCMP0 interrupt */
        NVIC_DisableIRQ(CMP0_IRQn);         /* disable INTCMP0 interrupt */
        INTC_ClearPendingIRQ(CMP0_IRQn);    /* clear INTCMP0 interrupt flag */
    }

    if(ch & 0x02) 
    {
        CMP->COMPMDR &= ~_10_COMP1_OPERATION_ENABLED;
        INTC_DisableIRQ(CMP1_IRQn);         /* disable INTCMP1 interrupt */
        NVIC_DisableIRQ(CMP1_IRQn);         /* disable INTCMP1 interrupt */
        INTC_ClearPendingIRQ(CMP1_IRQn);    /* clear INTCMP1 interrupt flag */
    }
}
/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
