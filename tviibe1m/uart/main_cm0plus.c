/***************************************************************************//**
* \file main_cm0plus.c
*
* \brief
* Main file for CM0+
*
********************************************************************************
* \copyright
* Copyright 2016-2019, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "cy_project.h"
#include "cy_device_headers.h"

#if (CY_USE_PSVP == 1)
    #define USER_LED_PORT           CY_LED0_PORT
    #define USER_LED_PIN            CY_LED0_PIN
    #define USER_LED_PIN_MUX        CY_LED0_PIN_MUX
#else
    #define USER_LED_PORT           CY_CB_LED_PORT
    #define USER_LED_PIN            CY_CB_LED_PIN
    #define USER_LED_PIN_MUX        CY_CB_LED_PIN_MUX 
#endif


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
static uint16_t data_a;
uint16_t data_b;
typedef struct DEMO
{
  uint16_t x,y;
  uint32_t (*func)(uint16_t x,uint16_t y); //函数指针
}DEMO_t;
uint32_t add(uint16_t x,uint16_t y);
uint32_t product(uint16_t x,uint16_t y);
uint32_t data_temp[10];
int main(void)
{
    DEMO_t demo;
    float data_i=0.1;
    float data_j=0;
    uint8_t data_h=20;
    uint16_t data_bb=0;
    SystemInit();
    __enable_irq();
     /* Initializing console uart interface */
    Cy_Semihosting_InitAll(CY_USB_SCB_UART_TYPE, 115200, NULL, false);
    /* Enable CM4. CY_CORTEX_M4_APPL_ADDR is calculated in linker script, check it in case of problems. */
    Cy_SysEnableApplCore(CY_CORTEX_M4_APPL_ADDR);

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    Cy_GPIO_Pin_Init(USER_LED_PORT, USER_LED_PIN, &user_led_port_pin_cfg);

    printf("-------------------------------\n\r");
    for(;;)
    {
        // Wait 0.05 [s]
       Cy_SysTick_DelayInUs(500000);
       data_a++;
       data_b++;
       data_bb++;

//        Cy_GPIO_Inv(USER_LED_PORT, USER_LED_PIN);
        Cy_GPIO_Set(USER_LED_PORT, USER_LED_PIN);
        Cy_SysTick_DelayInUs(500000);
        data_j=data_i*data_h;
        Cy_GPIO_Clr(USER_LED_PORT, USER_LED_PIN);
        demo.func=product;
        data_temp[0]=demo.func(3,4);
        printf("-------------------------------\n\r");
        printf("demo.func(3,4)=%ld\n",data_temp[0]);
        printf("-------------------------------\n\r");
        demo.func=add;
        data_temp[1]=demo.func(3,4);
        printf("-------------------------------\n\r");
        printf("demo.func(3,4)=%ld\n",data_temp[1]);
        printf("-------------------------------\n\r");
    }
}


uint32_t add(uint16_t x,uint16_t y)
{
    return x+y;
}
uint32_t product(uint16_t x,uint16_t y)
{
    return x*y;
}
 

/* [] END OF FILE */
