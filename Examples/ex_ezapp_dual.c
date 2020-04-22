///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                         ex_ezapp_dual.c                           ////
////                                                                   ////
//// This example shows how EZAppFieldsClearAll() can be called to     ////
//// dynamically clear the GUI screen so an all new screen can be      ////
//// displayed.  This technique can be used to create apps that have   ////
//// different screens.  In this example it shows one screen if        ////
//// ADC Channel 0 is below a threshold, and shows a seperate warning  ////
//// screen in that ADC value is above a threshold.                    ////
////                                                                   ////
//// For full EZ App Lynx documentation, EZApp.PDF contains an         ////
//// overview of the entire system while EZApp.h documents the full    ////
//// API that can be used with this library.                           ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2015 Custom Computer Services           ////
////                                                                   ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler or to users who purchased the rights to use this code.   ////
//// This source code may only be distributed to other licensed users  ////
//// of the CCS C compiler or other users who purchased the rights to  ////
//// this code.  The rights to use this code can only be granted       ////
//// by CCS.  No other use, reproduction or distribution is permitted  ////
//// without written permission.  Derivative programs created using    ////
//// this software in object code form are not restricted in any way.  ////
////                                                                   ////
////                     http://www.ccsinfo.com                        ////
///////////////////////////////////////////////////////////////////////////

// Common definitions for all the examples.
// Hardware is configured in this .h file and then populates several
// macros/definitions used by all the EZ App Lynx examples.
#include "ex_ezapp_common.h"

#include <EZApp.h>

struct
{
   struct
   {
      ezapp_field_index_t pot;
      ezapp_field_index_t dac0;
   } screen1;
      
   struct
   {
      ezapp_field_index_t slider;
   } screen2;
} idx;

struct
{
   bool button;
   uint16_t dac0;
   uint16_t dac1;
} values;

void Screen1Init(void)
{
   memset(&idx, EZAPP_FIELD_INDEX_INVALID, sizeof(idx));
      
   EZAppFieldsClearAll();
   
   EZAppSetTitleROM((rom char *)"ex_ezapp_dual.c");

   EZAppSetValueStringROM(
         EZAppAddFieldString(), 
         (rom char*)"Screen 1"
      );
  
   idx.screen1.pot = EZAppAddFieldAnalogValue(
         (rom char *)ADC0_STR, 
         EZAPP_ANALOG_TYPE_GAUGE, 
         ADC_MAX
      );
   
   idx.screen1.dac0 = EZAppAddFieldAnalogValue(
         (rom char *)DAC0_STR, 
         EZAPP_ANALOG_TYPE_SLIDER_RW, 
         DAC0_MAX
      );
      
   EZAppSetValue(idx.screen1.dac0, values.dac0);
}

void Screen1Task(void)
{
   EZAppSetValue(idx.screen1.pot, ADC_READ());

   values.dac0 = EZAppGetValue(idx.screen1.dac0);
   DAC0_SET(values.dac0);      
}

void Screen2Init(void)
{
   memset(&idx, EZAPP_FIELD_INDEX_INVALID, sizeof(idx));
   
   EZAppFieldsClearAll();
   
   EZAppSetTitleROM((rom char *)"POT too high!");

   EZAppSetValueStringROM(
         EZAppAddFieldString(), 
         (rom char*)"Screen 2"
      );
  
   EZAppSetValueStringROM(
         EZAppAddFieldStringDynamic(0xFF0000, +2), 
         (rom char*)"Turn down the pot!"
      );
   
   idx.screen2.slider = EZAppAddFieldAnalogValue(
         (rom char *)ADC0_STR, 
         EZAPP_ANALOG_TYPE_SLIDER, 
         ADC_MAX
      );
}

void Screen2Task(void)
{
   EZAppSetValue(idx.screen2.slider, ADC_READ());
}

void main(void)
{
   uint8_t screen = -1;
   
   HW_INIT();
   
   ADC_INIT();
   ADC0_CHANNEL();
   
   DAC0_INIT();
   
   memset(&values, 0x00, sizeof(values));
      
   EZAppInit();
   
   for(;;)
   {
      restart_wdt();
      
      HW_TASK();
      
      EZAppTask();
      
      if (ADC_READ() > 900)
      {
         if (screen != 2)
         {
            screen = 2;
            
            Screen2Init();
         }
         
         Screen2Task();
      }
      else
      {
         if (screen != 1)
         {
            screen = 1;
            
            Screen1Init();
         }
         
         Screen1Task();
      }
      
      LED_SET(LED1_PIN, BtSppIsConnected());
      
      LED_SET(LED2_PIN, EZAppIsConnected());
   }
}
