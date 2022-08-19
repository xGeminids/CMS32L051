/***********************************************************************************************************************
* Copyright (C) All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* @file    cmp.h
* @brief   This file implements device driver for COMP module.
* @version 1.0.0
* @date    2019/12/24
***********************************************************************************************************************/
#ifndef COMP_H
#define COMP_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/*
    Comparator Mode Setting Register (COMPMDR) 
*/
/* Comparator 1 monitor flag bit (C1MON) */
#define _00_COMP1_FLAG_REFERENCE_0                    (0x00U) /* IVCMP1 less than comparator 1 reference voltage or comparator 1 stopped */
#define _80_COMP1_FLAG_REFERENCE_1                    (0x80U) /* IVCMP1 more than comparator 1 reference voltage */
/* Comparator 1 operation enable bit (C1ENB) */
#define _00_COMP1_OPERATION_DISABLED                  (0x00U) /* comparator 1 operation disabled */
#define _10_COMP1_OPERATION_ENABLED                   (0x10U) /* comparator 1 operation enabled */
/* Comparator 0 monitor flag bit (C0MON) */
#define _00_COMP0_FLAG_REFERENCE_0                    (0x00U) /* IVCMP0 less than comparator 0 reference voltage or comparator 0 stopped */
#define _08_COMP0_FLAG_REFERENCE_1                    (0x08U) /* IVCMP0 more than comparator 0 reference voltage */
/* Comparator 0 operation enable bit (C0ENB) */
#define _00_COMP0_OPERATION_DISABLED                  (0x00U) /* comparator 0 operation disabled */
#define _01_COMP0_OPERATION_ENABLED                   (0x01U) /* comparator 0 operation enabled */

/*
    Comparator Filter Control Register (COMPFIR) 
*/
/* Comparator 1 edge detectioin selection (C1EDG) */
#define _00_COMP1_ONE_EDGE_INTERRUPT                  (0x00U) /* interrupt request by comparator 1 one-edge detection */
#define _80_COMP1_BOTH_EDGES_INTERRUPT                (0x80U) /* interrupt request by comparator 1 both-edge detection */
/* Comparator 1 edges polarity selection (C1EPO) */
#define _00_COMP1_RISING_INTERRUPT                    (0x00U) /* interrupt request at comparator 1 rising edge */
#define _40_COMP1_FALLING_INTERRUPT                   (0x40U) /* interrupt request at comparator 1 falling edge */
/* Comparator 1 Filter Selection (C1FCK) */
#define _00_COMP1_FILTER_NONE                         (0x00U) /* disable filter */
#define _10_COMP1_FILTER_FCLK                         (0x10U) /* sampling at fCLK */
#define _20_COMP1_FILTER_FCLK_8                       (0x20U) /* sampling at fCLK/2^3 */
#define _30_COMP1_FILTER_FCLK_32                      (0x30U) /* sampling at fCLK/2^5 */
/* Comparator 0 edge detectioin selection (C0EDG) */
#define _00_COMP0_ONE_EDGE_INTERRUPT                  (0x00U) /* interrupt request by comparator 0 one-edge detection */
#define _08_COMP0_BOTH_EDGES_INTERRUPT                (0x08U) /* interrupt request by comparator 0 both-edge detection */
/* Comparator 0 edges polarity selection (C0EPO) */
#define _00_COMP0_RISING_INTERRUPT                    (0x00U) /* interrupt request at comparator 0 rising edge */
#define _04_COMP0_FALLING_INTERRUPT                   (0x04U) /* interrupt request at comparator 0 falling edge */
/* Comparator 0 Filter Selection (C0FCK) */
#define _00_COMP0_FILTER_NONE                         (0x00U) /* disable filter */
#define _01_COMP0_FILTER_FCLK                         (0x01U) /* sampling at fCLK */
#define _02_COMP0_FILTER_FCLK_8                       (0x02U) /* sampling at fCLK/2^3 */
#define _03_COMP0_FILTER_FCLK_32                      (0x03U) /* sampling at fCLK/2^5 */

/*
    Comparator Output Control Register (COMPOCR) 
*/
/* TIMER WINDOW output mode control bit of comparator 1 (C1OTWMD) */
#define _00_COMP1_OUTPUT_NORMAL                       (0x00U) /* comparator 1 normal output mode */
#define _80_COMP1_OUTPUT_TIMERWIN                     (0x80U) /* comparator 1 TIMER WINDOW output mode */
/* VCOUT1 output polarity selection (C1OP) */
#define _00_COMP1_OUTPUT_NORMAL_TO_VCOUT1             (0x00U) /* comparator 1 output is output to VCOUT1 */
#define _40_COMP1_OUTPUT_INVERTED_TO_VCOUT1           (0x40U) /* inverted comparator 1 output is output to VCOUT1 */
/* Comparator 1 interrupt request enable (C1OE) */
#define _00_COMP1_OUTPUT_DISABLE                      (0x00U) /* comparator 1 VCOUT1 pin output disabled */
#define _20_COMP1_OUTPUT_ENABLE                       (0x20U) /* comparator 1 VCOUT1 pin output enabled */
/* Comparator 1 interrupt request selection (C1IE) */
#define _00_COMP1_INTERRUPT_DISABLE                   (0x00U) /* comparator 1 interrupt request disabled */
#define _10_COMP1_INTERRPUT_ENABLE                    (0x10U) /* comparator 1 interrupt request enabled */
/* VCOUT0 output polarity selection (C0OP) */
#define _00_COMP0_OUTPUT_NORMAL_TO_VCOUT0             (0x00U) /* comparator 0 output is output to VCOUT0 */
#define _04_COMP0_OUTPUT_INVERTED_TO_VCOUT0           (0x04U) /* inverted comparator 0 output is output to VCOUT0 */
/* VCOUT0 pin output enable (C0OE) */
#define _00_COMP0_OUTPUT_DISABLE                      (0x00U) /* comparator 0 VCOUT0 pin output disabled */
#define _02_COMP0_OUTPUT_ENABLE                       (0x02U) /* comparator 0 VCOUT0 pin output enabled */
/* Comparator 0 interrupt request selection (C0IE) */
#define _00_COMP0_INTERRUPT_DISABLE                   (0x00U) /* comparator 0 interrupt request disabled */
#define _01_COMP0_INTERRPUT_ENABLE                    (0x01U) /* comparator 0 interrupt request enabled */

/*
    Comparator internal reference voltage control register (CVRCTL) 
*/
/* Control bit for internal reference voltage 1 (CVRE1) */
#define _00_COMP1_REFVOLTAGE_DISABLE                  (0x00U) /* internal reference voltage 1 operation stopped */
#define _20_COMP1_REFVOLTAGE_ENABLE                   (0x20U) /* internal reference voltage 1 operation enabled */
/* Ground selection bit for internal reference voltage (CVRVS1) */
#define _00_VSS_INTERNAL_REFERVOL                     (0x00U) /* VSS selected as ground for internal reference voltage */
#define _10_AVREFM_INTERNAL_REFERVOL                  (0x10U) /* AVREFM selected as ground for internal reference voltage */
/* Control bit for internal reference voltage 0 (CVRE0) */
#define _00_COMP0_REFVOLTAGE_DISABLE                  (0x00U) /* internal reference voltage 0 operation stopped */
#define _02_COMP0_REFVOLTAGE_ENABLE                   (0x02U) /* internal reference voltage 0 operation enabled */
/* Power supply of internal reference voltage (CVRVS0) */
#define _00_VDD_INTERNAL_REFERVOL                     (0x00U) /* VDD is selected as power supply of internal reference voltage  */
#define _01_AVREFP_INTERNAL_REFERVOL                  (0x01U) /* AVREFP is selected as power supply of internal reference voltage */

/*
    Comparator 0 Input Signal Selection Control Register (CMPSEL0) 
*/
/* Positive pin input signal selection of Comparator 0 (CMP0SEL) */
#define _00_COM0_IVCMP0_SELECTED                      (0x00U) /* AIN5V0 (IVCMP0 pin) selected */
#define _80_COM0_PGA_SELECTED                         (0x80U) /* signal from PGA */
/* Negative pin input signal selection of Comparator 0 (C0REFS1,C0REFS0) */
#define _00_COM0_VREF0_SELECTED                       (0x00U) /* internal reference voltage VREF0 selected */
#define _01_COM0_INTERNAL_REFERENCE_SELECTED          (0x01U) /* internal reference voltage (1.45 V) selected */
#define _02_COM0_IVREF0_SELECTED                      (0x02U) /* external pin (IVREF0) selected */

/*
    Comparator 1 Input Signal Selection Control Register (CMPSEL1) 
*/
/* Positive pin input signal selection of Comparator 1 (CMP1SEL1,CMP1SEL0) */
#define _00_COM1_IVCMP10_POSITIVE                     (0x00U) /* external pin (IVCMP10) selected */
#define _40_COM1_IVCMP11_POSITIVE                     (0x40U) /* external pin (IVCMP11) selected */
#define _80_COM1_IVCMP12_POSITIVE                     (0x80U) /* external pin (IVCMP12) selected */
#define _C0_COM1_IVCMP13_POSITIVE                     (0xC0U) /* external pin (IVCMP13) selected */
/* Negative pin input signal selection of Comparator 1 (C1REFS2,C1REFS1,C1REFS0) */
#define _00_COM1_VREF1_SELECTED                       (0x00U) /* internal reference voltage VREF1 selected */
#define _01_COM1_INTERNAL_REFERENCE_SELECTED          (0x01U) /* internal reference voltage (1.45 V) selected */
#define _02_COM1_IVCMP10_NEGATIVE                     (0x02U) /* external pin (IVCMP10) selected */
#define _03_COM1_IVCMP11_NEGATIVE                     (0x03U) /* external pin (IVCMP11) selected */
#define _04_COM1_IVCMP12_NEGATIVE                     (0x04U) /* external pin (IVCMP12) selected */
#define _05_COM1_IVCMP13_NEGATIVE                     (0x05U) /* external pin (IVCMP13) selected */

#define C0MON                                         (1<<3U) /* CMP0 monitor flag */
#define C1MON                                         (1<<7U) /* CMP1 monitor flag */

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define COMP_WAIT_TIME                                (0x08U)   /* change the waiting time according to the system */
#define _00_C0RVM_VALUE                               (0x00U) /* comparator 0 internal reference voltage selection register */
#define _00_C1RVM_VALUE                               (0x00U) /* Comparator 1 internal reference voltage selection register */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef enum 
{
    CMP_CHANNEL_0 = 1U,  // COMP0
    CMP_CHANNEL_1 = 2U,  // COMP1
    CMP_CHANNEL_A = 3U   // COMP0 & COMP1
} cmp_channel_t;

typedef enum 
{
    CMP_VDD_VSS_REFERENCE_VOLTAGE = 0U,        // VDD/VSS selected as power/ground for internal reference voltage
    CMP_AVREFP_AVREFM_REFERENCE_VOLTAGE = 1U   // AVREFP/AVREFM selected as power/ground for internal reference voltage
} cmp_vref_t;

typedef enum 
{
    CMP_PSIDE_VCIN0 = 0U,   	// P32
    CMP_PSIDE_PGAO  = 1U,     	// PGA0OUT
    CMP_NSIDE_IVREF  = 0U,     	// internal voltage reference
    CMP_NSIDE_BGR   = 1U,     	// BGR output reference voltage
    CMP_NSIDE_EVREF  = 2U,   	// P30
    CMP_PSIDE_VCIN10   = 0U,   	// P20
    CMP_PSIDE_VCIN11   = 1U, 	// P21
    CMP_PSIDE_VCIN12   = 2U, 	// P26
    CMP_PSIDE_VCIN13   = 3U, 	// P25
    CMP_NSIDE_VCIN10   = 2U,    // P20
    CMP_NSIDE_VCIN11   = 3U,  	// P21
    CMP_NSIDE_VCIN12   = 4U,  	// P26
    CMP_NSIDE_VCIN13   = 5U   	// P25
} cmp_side_t;

typedef enum 
{
    CMP_FILTER_NONE    = 0U,  // None Filter
    CMP_FILTER_FCLK    = 1U,  // Comparator filter enabled, sampling at fCLK
    CMP_FILTER_FCLK_8  = 2U,  // Comparator filter enabled, sampling at fCLK/8    
    CMP_FILTER_FCLK_32 = 3U   // Comparator filter enabled, sampling at fCLK/32
} cmp_filter_t;

typedef enum 
{
    CMP_EDGE_RISING    = 0U,  // Interrupt request at comparator rising edge
    CMP_EDGE_FALLING   = 1U,  // Interrupt request at comparator falling edge
    CMP_EDGE_BOTH      = 2U   // Interrupt request at comparator both edge    
} cmp_edge_t;

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void CMP_VREF_Init(cmp_channel_t ch, cmp_vref_t vref);
void CMP_VREF_Set_Value(cmp_channel_t ch, uint8_t regvalue);
void CMP_Input_Select(cmp_channel_t ch, cmp_side_t positive_side, cmp_side_t negative_side);
void CMP_Filter_Setting(cmp_channel_t ch, cmp_filter_t fck, cmp_edge_t edge);
void CMP_Init(cmp_channel_t ch, cmp_side_t positive_side, cmp_side_t negative_side);
void CMP_Start(cmp_channel_t ch);
void CMP_Stop(cmp_channel_t ch);
/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif

