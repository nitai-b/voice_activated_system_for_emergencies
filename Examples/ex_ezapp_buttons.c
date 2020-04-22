///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                        ex_ezapp_buttons.c                         ////
////                                                                   ////
//// An example of using buttons in EZ App Lynx.  Creates two rows of  ////
//// buttons that behave differently:                                  ////
////                                                                   ////
//// Row 1 - One state buttons (value is set while the button is       ////
////     held down, value is clear while the button is not held down). ////
////                                                                   ////
//// Row 2 - Two state buttons (value is toggled when a button is      ////
////     'pressed', where 'pressed' is the push then release of a      ////
////     button).  Also has LED status indicators inside the button,   ////
////     which can be controlled independently of the press status     ////
////     with EZAppSetButtonLED().                                     ////
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

// CCS C Compiler

// Common definitions for all the examples.
// Hardware is configured in this .h file and then populates several
// macros/definitions used by all the EZ App Lynx examples.
#include "ex_ezapp_common.h"

#include <EZApp.h>

#ifndef BUTTON0_PRESSED
#define BUTTON0_PRESSED() (false)
#endif

#ifndef BUTTON1_PRESSED
#define BUTTON1_PRESSED() (false)
#endif

void main(void)
{
   ezapp_field_index_t ezIdxButtonRow0, ezIdxButtonRow1;
   ezapp_buttons_t ezButtonConfig;
   uint16_t val;
   
   HW_INIT();
   
   ADC_INIT();
   ADC0_CHANNEL();

   EZAppInit();
   
   EZAppSetTitleROM((rom char *)"ex_ezapp_buttons.c");   //0

   EZAppSetValueStringROM(EZAppAddFieldString(), (rom char*)"Button Example");   //1

   memset(&ezButtonConfig, 0x00, sizeof(ezButtonConfig));
   ezButtonConfig.numButtons = 3;
   ezButtonConfig.fillRow = true;
   ezButtonConfig.oneState = true;
   
   ezIdxButtonRow0 = EZAppAddFieldButtonsROM(
         ezButtonConfig, 
         (rom char *)"LEDS",
         (rom char *)"LED0\tLED1\tLED2"
      );                                                             //2
   
   memset(&ezButtonConfig, 0x00, sizeof(ezButtonConfig));
   ezButtonConfig.numButtons = 2;
   ezButtonConfig.fillRow = true;
   ezButtonConfig.ledColor0 = EZAPP_LED_GRAY;
   ezButtonConfig.ledColor1 = EZAPP_LED_RED;
   
   ezIdxButtonRow1 = EZAppAddFieldButtonsROM(
         ezButtonConfig, 
         (rom char *)"RELAYS",
         (rom char *)"OPEN\tCLOSED\tOPEN\tCLOSED"
      );                                                             //3

   EZAppSetValue(ezIdxButtonRow1, 0);
   DIGOUT_SET(DIGOUT0_PIN, 0);
   DIGOUT_SET(DIGOUT1_PIN, 0);

   for(;;)
   {
      restart_wdt();
      
      HW_TASK();
      
      EZAppTask();
      
      val = EZAppGetValue(ezIdxButtonRow0);
      LED_SET(LED0_PIN, bit_test(val, 0));
      LED_SET(LED1_PIN, bit_test(val, 1));
      LED_SET(LED2_PIN, bit_test(val, 2));

      if (EZAppGetKbhit(ezIdxButtonRow1))
      {
         val = EZAppGetValue(ezIdxButtonRow1);
         DIGOUT_SET(DIGOUT0_PIN, bit_test(val, 0));
         DIGOUT_SET(DIGOUT1_PIN, bit_test(val, 1));
      }
      
      EZAppSetButtonLED(ezIdxButtonRow1, 0, BUTTON0_PRESSED());
      EZAppSetButtonLED(ezIdxButtonRow1, 1, BUTTON1_PRESSED());
   }
}
