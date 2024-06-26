/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "cy_project.h"
#include "cy_device_headers.h"

#if (CY_USE_PSVP == 1)
    #define USER_LED_PORT               CY_LED0_PORT
    #define USER_LED_PIN                CY_LED0_PIN
    #define USER_LED_PIN_MUX            CY_LED0_PIN_MUX
#else
    #define USER_LED_PORT               CY_CB_LED_PORT
    #define USER_LED_PIN                CY_CB_LED_PIN
    #define USER_LED_PIN_MUX            CY_CB_LED_PIN_MUX 
#endif


/* HF clock selection as target clock, HF_CLK1 -- 80MHz */
#define CSV_MONITOR_ON_CLK_HF           CY_SYSCLK_HFCLK_1

/* Select reference clock source */
#define CSV_REF_CLK_IMO                 0
#define CSV_REF_CLK_ECO                 1
#define CSV_REF_CLK_SELECT              CSV_REF_CLK_ECO

/* Select the CSV error generation type from any of these case */
#define CSV_TRIG_ERROR_NONE             0
#define CSV_TRIG_ERROR_LOWER_INCREASE   1
#define CSV_TRIG_ERROR_UPPER_DECREASE   2
#define CSV_TRIG_ERROR_PERIOD_CHANGE    3

/* Macro for upper, lower, period limit for csv, based on the AN32474 */
#define CSV_MONITOR_NUM_OF_CYCLE        200
#define CSV_REF_LIMIT_LOWER             191
#define CSV_REF_LIMIT_UPPER             208

/* For reference clock as ECO (16MHz) and target HF_CLK1 (80MHz) */
#if (CSV_REF_CLK_SELECT == CSV_REF_CLK_ECO)
#define CSV_MONITOR_START_DELAY         570
#define CSV_MONITOR_CTRL_PERIOD         999
/* For reference clock as IMO (8MHz) and target HF_CLK1 (80MHz) */
#else /* (CSV_REF_CLK_SELECT == CSV_REF_CLK_IMO) */
#define CSV_MONITOR_START_DELAY         282
#define CSV_MONITOR_CTRL_PERIOD         1999
#endif /* CSV_REF_CLK_SELECT */


/* Configure CSV for the HF clock */
cy_stc_hf_csv_config_t csvConfigure =
{
#if (CSV_REF_CLK_SELECT == CSV_REF_CLK_ECO)
    .supervisorClock    = CY_SYSCLK_HFCLK_CSV_SUPERVISOR_ECO,   /* select ECO as supervisory clock */
#else
    .supervisorClock    = CY_SYSCLK_HFCLK_CSV_SUPERVISOR_IMO,   /* select IMO as supervisory clock */
#endif    
    .noOfCycleInWindow  = CSV_MONITOR_NUM_OF_CYCLE,             /* number of supervising clock cycles */
    .actionFaultOrReset = CY_SYSCLK_HFCLK_CSV_ACTION_FAULT,     /* action on fault */
    .windowLowerLimit   = CSV_REF_LIMIT_LOWER,                  /* refernece lower limit */
    .windowUpperLimit   = CSV_REF_LIMIT_UPPER,                  /* refernece upper limit */
    .windowRefPeriod    = CSV_MONITOR_CTRL_PERIOD,              /* target clock period */
    .startupDelay       = CSV_MONITOR_START_DELAY,              /* clock start-up delay */
};

/* IO pin configuration for LED */
cy_stc_gpio_pin_config_t user_led_port_pin_cfg =             
{                                                  
    .outVal = 0x00,                                
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,    
    .hsiom = USER_LED_PIN_MUX,                           
    .intEdge = 0,                                  
    .intMask = 0,                                  
    .vtrip = 0,                                    
    .slewRate = 0,                                 
    .driveSel = 0,                                 
    .vregEn = 0,                                   
    .ibufMode = 0,                                 
    .vtripSel = 0,                                 
    .vrefSel = 0,                                  
    .vohSel = 0,                                   
};


/* Indicate the CSV fault event */
bool isCSVFaultOccured = false;

/* Set it appropiate value to generate the CSV falult */
uint8_t generateCSVFault = CSV_TRIG_ERROR_NONE;


/* Fault report callback */
void irqFaultReport0Handler(void)
{    
    uint32_t fault_source = 0;

    /* Get fault source */
    fault_source = Cy_SysFlt_GetErrorSource(FAULT_STRUCT0);

    /* Check for the any fault */
    if(fault_source != CY_SYSFLT_NO_FAULT)
    {
        /* Fault generated by Clock supervision system */
        if(fault_source == CY_SYSFLT_SRSS_FAULT_CSV)
        {
            fault_source = Cy_SysFlt_GetData0(FAULT_STRUCT0);

            /* Check if the fault is associated with monitor clock */
            if ((fault_source >> CSV_MONITOR_ON_CLK_HF) == 1u)
            {
                isCSVFaultOccured = true;
                generateCSVFault = CSV_TRIG_ERROR_NONE;
            }
        }
    }
    Cy_SysFlt_ClearStatus(FAULT_STRUCT0);
    Cy_SysFlt_ClearInterrupt(FAULT_STRUCT0);
}

/* Configure fault structure to report the fault */
void ConfigureFaultForCsv(void)
{
    /* Clear the pending fault to capture only for current execution */
    Cy_SysFlt_ClearStatus(FAULT_STRUCT0);
    Cy_SysFlt_ClearInterrupt(FAULT_STRUCT0);
  
    /* Interrupt initialization for fault0 */
    {
        cy_stc_sysint_irq_t irq_cfg;
        irq_cfg = (cy_stc_sysint_irq_t){
            .sysIntSrc  = cpuss_interrupts_fault_0_IRQn,
            .intIdx     = CPUIntIdx3_IRQn,
            .isEnabled  = true,
        };
        Cy_SysInt_InitIRQ(&irq_cfg);
        Cy_SysInt_SetSystemIrqVector(cpuss_interrupts_fault_0_IRQn, irqFaultReport0Handler);
    }

    /* Enable the NVIC channel */
    NVIC_SetPriority(CPUIntIdx3_IRQn, 0);
    NVIC_ClearPendingIRQ(CPUIntIdx3_IRQn);
    NVIC_EnableIRQ(CPUIntIdx3_IRQn);
    
    /* Enable Fault for CSV error */
    Cy_SysFlt_ClearStatus(FAULT_STRUCT0);
    Cy_SysFlt_SetMaskByIdx(FAULT_STRUCT0, CY_SYSFLT_SRSS_FAULT_CSV);
    Cy_SysFlt_SetInterruptMask(FAULT_STRUCT0);
}

/* main function */
int main(void)
{   
    /* enable irq */
    __enable_irq();
    
    /* System init */
    SystemInit();

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    Cy_GPIO_Pin_Init(USER_LED_PORT, USER_LED_PIN, &user_led_port_pin_cfg);

    /*--------------- CSV (Clock Supervisor), Fault Configuration ------------*/
    
    /* Configure fault for the HF-CSV */
    ConfigureFaultForCsv();
    
    
    /*---------- CSV :: Reference:ECO(16MHz), Monitor: HF0(80MHz) -----------*/
    
    /* Configure csv for the target HF clock */
    Cy_SysClk_HfClockConfigureCsv(CSV_MONITOR_ON_CLK_HF, &csvConfigure);

    
    /* Monitor status here */
    for(;;)
    {
        /* Trigger an event for CSV fault, if generateCSVFault is any of the case */
        switch (generateCSVFault)
        {
            /* Increase a lower limit more than period for HF clock */
            case CSV_TRIG_ERROR_LOWER_INCREASE:
                CY_SET_REG32(CYREG_CSV_HF_CSV1_REF_LIMIT, (CSV_REF_LIMIT_UPPER << 16)|(CSV_REF_LIMIT_LOWER + 15));
                break;
            /* Decrease a upper limit less than period for HF clock */
            case CSV_TRIG_ERROR_UPPER_DECREASE:
                CY_SET_REG32(CYREG_CSV_HF_CSV1_REF_LIMIT, ((CSV_REF_LIMIT_UPPER - 15) << 16) | CSV_REF_LIMIT_LOWER);
                break;
            /* Change the period of the monitoring window */
            case CSV_TRIG_ERROR_PERIOD_CHANGE:
                CY_SET_REG32(CYREG_CSV_HF_CSV1_MON_CTL, (CSV_MONITOR_CTRL_PERIOD - 100));
                break;
            /* No error */
            default:
                break;
        }
      
        /* Blink an LED if CSV fault occured */
        if (isCSVFaultOccured == true)
        {
            while(1)
            {
                Cy_SysLib_DelayCycles(8000000);
                Cy_GPIO_Inv(USER_LED_PORT, USER_LED_PIN);
            }
        }
        /* Else turn on LED for no fault */
        else
        {
            Cy_GPIO_Write(USER_LED_PORT, USER_LED_PIN, 1u);
        }
    }
}

/* [] END OF FILE */
