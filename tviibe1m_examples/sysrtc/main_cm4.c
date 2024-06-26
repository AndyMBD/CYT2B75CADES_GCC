/***************************************************************************//**
* \file main_cm4.c
*
* \version 1.0
*
* \brief Main example file for CM4
*
********************************************************************************
* \copyright
* Copyright 2016-2020, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "cy_project.h"
#include "cy_device_headers.h"

/*******************************************************************************
*    Parameters definitions
*******************************************************************************/

/* Select clock source for RTC */
#define RTC_CLK_SRC_ILO0              (0ul)
#define RTC_CLK_SRC_WCO               (1ul)
#define RTC_USES_CLK_SRC              RTC_CLK_SRC_WCO

/* Initial Time and Date definitions */
#define RTC_INITIAL_DATE_SEC          (0ul)
#define RTC_INITIAL_DATE_MIN          (22ul)
#define RTC_INITIAL_DATE_HOUR         (16ul)
#define RTC_INITIAL_DATE_HOUR_FORMAT  (CY_RTC_24_HOURS)
#define RTC_INITIAL_DATE_DOW          (1ul)
#define RTC_INITIAL_DATE_DOM          (30ul)
#define RTC_INITIAL_DATE_MONTH        (8ul)
#define RTC_INITIAL_DATE_YEAR         (21ul)

/** The instance-specific configuration structure. This should be used in the 
*  associated RTC_Init() function.
*/
cy_stc_rtc_config_t const RTC_config =
{
    /* Initiate time and date */
    .sec       = RTC_INITIAL_DATE_SEC,
    .min       = RTC_INITIAL_DATE_MIN,
    .hour      = RTC_INITIAL_DATE_HOUR,
    .hrMode    = RTC_INITIAL_DATE_HOUR_FORMAT,
    .dayOfWeek = RTC_INITIAL_DATE_DOW,
    .date      = RTC_INITIAL_DATE_DOM,
    .month     = RTC_INITIAL_DATE_MONTH,
    .year      = RTC_INITIAL_DATE_YEAR,
};

/* This configuration will generate interrupt in every minute whenever
 * SECONDS match. 
 * Example: 00:00, 01:00, 02:00...
 */
cy_stc_rtc_alarm_t const alarm =
{
  .sec          = 0ul,
  .sec_en       = CY_RTC_ALARM_ENABLE,
  .min          = 0ul,
  .min_en       = CY_RTC_ALARM_DISABLE,
  .hour         = 0ul,
  .hour_en      = CY_RTC_ALARM_DISABLE,
  .dayOfWeek    = 1ul,
  .dayOfWeek_en = CY_RTC_ALARM_DISABLE,
  .date         = 1ul,
  .date_en      = CY_RTC_ALARM_DISABLE,
  .month        = 1ul,
  .month_en     = CY_RTC_ALARM_DISABLE,
  .alm_en       = CY_RTC_ALARM_ENABLE
};

cy_str_rtc_dst_t const RTC_dstConfig;

/** RTC_rtcDstStatus variable which is for DST feature and is 
*  called in Cy_RTC_Interrupt() PDL driver function. This variable is 
*  defined as true if DST is enabled and as false if DST is disabled
*/
bool RTC_rtcDstStatus = false;
cy_stc_rtc_config_t Read_DateTime;

void RTC_Handler(void)
{
    Cy_Rtc_Interrupt(&RTC_dstConfig, RTC_rtcDstStatus);
}

void Cy_Rtc_Alarm1Interrupt(void)
{
    /* Clear any pending interrupts */
    Cy_Rtc_ClearInterrupt(CY_RTC_INTR_ALARM1);
    Cy_Rtc_GetDateAndTime(&Read_DateTime);
}

int main(void)
{
    SystemInit();

    __enable_irq(); /* Enable global interrupts. */

    cy_stc_sysint_irq_t irq_cfg;
    irq_cfg = (cy_stc_sysint_irq_t)
    {
        .sysIntSrc  = srss_interrupt_backup_IRQn,
        .intIdx     = CPUIntIdx2_IRQn,
        .isEnabled  = true,
    };
    Cy_SysInt_InitIRQ(&irq_cfg);
    Cy_SysInt_SetSystemIrqVector(irq_cfg.sysIntSrc, RTC_Handler);

    NVIC_SetPriority(CPUIntIdx2_IRQn, 3ul);
    NVIC_ClearPendingIRQ(CPUIntIdx2_IRQn);
    NVIC_EnableIRQ(CPUIntIdx2_IRQn);
    
#if(RTC_USES_CLK_SRC == RTC_CLK_SRC_WCO)
    /* Check if LPECO is active */
    #ifndef CY_SYSTEM_WCO_ENABLE
        /* enable WCO clock for RTC */
        Cy_SysClk_WcoEnable(100ul);
    #endif    
    /* Set the WCO as the clock source to the RTC block */
    Cy_Rtc_clock_source(CY_RTC_CLK_SRC_WCO);
#else
    /* Set the ILO_0 as the clock source to the RTC block */
    Cy_Rtc_clock_source(CY_RTC_CLK_SRC_ILO_0);
#endif

    /* Wait for alarm to be set */
    while(Cy_Rtc_Init(&RTC_config) != CY_RET_SUCCESS);

   /* Clear any pending interrupts */
    Cy_Rtc_ClearInterrupt(CY_RTC_INTR_ALARM1);

    /*Configures the source (Alarm1) that trigger the interrupts */
    Cy_Rtc_SetInterruptMask(CY_RTC_INTR_ALARM1);

    /* Wait for alarm to be set */
    while(Cy_Rtc_SetAlarmDateAndTime(&alarm,CY_RTC_ALARM_1) != CY_RET_SUCCESS);

    for(;;)
    {
        Cy_SysTick_DelayInUs(50000ul);
        Cy_Rtc_GetDateAndTime(&Read_DateTime);
    }
}

/* [] END OF FILE */
