///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                          ex_ezapp_pot.c                           ////
////                                                                   ////
//// The simplest EZ App Lynx demo, this creates a GUI with one        ////
//// element - a gas gauge showing the current ADC reading.            ////
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

void main(void)
{
   ezapp_field_index_t potIndex = EZAPP_FIELD_INDEX_INVALID;
   
   HW_INIT();  //init pins, peripherals, turn on global interrupts

   UARTConsoleInit(19200);
   UARTConsolePutsROM((rom char *)"ex_ezapp_pot.c running\r\n\r\n");

   ADC_INIT();
   ADC0_CHANNEL();

   TickInit();

   EZAppInit();   //init EZ App Lynx API, init Bluetooth module
  
   EZAppSetTitleROM((rom char *)"ex_ezapp_pot.c");
  
   potIndex = EZAppAddFieldAnalogValue(
         (rom char *)ADC0_STR,
         EZAPP_ANALOG_TYPE_GAUGE, 
         ADC_MAX
      );

   for(;;)
   {
      restart_wdt();
      
      HW_TASK();
            
      EZAppTask();

      EZAppSetValue(potIndex, ADC_READ());
   }
}
