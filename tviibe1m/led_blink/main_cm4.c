/***************************************************************************//**
* \file main_cm4.c
*
* \brief
* Main file for CM4
*
********************************************************************************
* \copyright
* Copyright 2016-2019, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "cy_project.h"
#include "cy_device_headers.h"
#include <stdio.h>

#define USER_LED_PORT           CY_LED0_PORT
#define USER_LED_PIN            CY_LED0_PIN
#define USER_LED_PIN_MUX        CY_LED0_PIN_MUX


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
  float data_i=0.1;
  float data_j=0.2;
  uint16_t data_h =20;
  uint8_t data_temp[10];

int main(void)
{
    __enable_irq();
//   float data_j=0.15;
    SystemInit();

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    Cy_GPIO_Pin_Init(USER_LED_PORT, USER_LED_PIN, &user_led_port_pin_cfg);
    printf("hellow world");
    for(;;)
    {
        // Cy_GPIO_Inv(USER_LED_PORT, USER_LED_PIN);
        Cy_SysTick_DelayInUs(50000);
        Cy_GPIO_Set(USER_LED_PORT, USER_LED_PIN);
        // Wait 0.05 [s]
        // data_j  = data_i*data_h+    data_j;
        // data_j++;
        data_h++;
        Cy_SysTick_DelayInUs(50000);
        printf("hellow world");
        data_temp[0]    =   data_i*data_h  +   data_j;
        data_temp[1]    =data_temp[0]+1;
        data_temp[2]    =data_temp[1]+1;
        data_temp[3]    =data_temp[2]+1;
        
        // data_j=data_i*data_h;
        Cy_GPIO_Clr(USER_LED_PORT, USER_LED_PIN);
        //Cy_GPIO_Inv(USER_LED_PORT, USER_LED_PIN);
        // printf();
    }
}



/* [] END OF FILE */
