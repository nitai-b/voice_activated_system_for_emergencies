///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                         ex_ezapp_gui.c                            ////
////                                                                   ////
//// An example of EZ App Lynx that shows many of the possible GUI     ////
//// elements at once.  Many of the GUI elements are tied to the       ////
//// hardware (DAC, ADC, buttons, LEDs, etc).                          ////
////                                                                   ////
//// There are several more EZ App Lynx examples.  See EZApp.h in the  ////
//// drivers directory for full documentation of EZ App Lynx and a     ////
//// list of more examples.                                            ////
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
   uint16_t digitalVal = 0;
   uint8_t newLedValue;
   char str[32];
   
   ezapp_field_index_t potIndex = EZAPP_FIELD_INDEX_INVALID, 
      pot2Index = EZAPP_FIELD_INDEX_INVALID, 
      photoIndex = EZAPP_FIELD_INDEX_INVALID, 
      thermoIndex = EZAPP_FIELD_INDEX_INVALID, 
      buttonIndex = EZAPP_FIELD_INDEX_INVALID, 
      diginIndex = EZAPP_FIELD_INDEX_INVALID,
      DAC0Index = EZAPP_FIELD_INDEX_INVALID,
      DAC1Index = EZAPP_FIELD_INDEX_INVALID,
      STRIndex = EZAPP_FIELD_INDEX_INVALID,
      pulldownIndex = EZAPP_FIELD_INDEX_INVALID,
      digout0Index = EZAPP_FIELD_INDEX_INVALID,
      digout1Index = EZAPP_FIELD_INDEX_INVALID;
   
  #if defined(__TICK_H__)
   TICK t;
  #endif
   
   HW_INIT();
   
   ADC_INIT();
   ADC0_CHANNEL();
   
   DAC0_INIT();
  
  #if defined(DAC1_INIT)
   DAC1_INIT();
  #endif

   EZAppInit();
           
   EZAppSetTitleROM((rom char *)"ex_ezapp_gui.c");                         // 0

   EZAppSetValueStringROM(                                                 // 1
         EZAppAddFieldString(), 
         (rom char*)"EZ App Lynx GUI Demo"
      );
   
   EZAppSetValueStringROM(                                                 // 2
         EZAppAddFieldStringDynamic(0xFF0000, 12),
         (rom char*)"Running"
      );
   
   EZAppAddFieldSpacerDynamic(0x00FF00, 12, (rom char*)"Inputs");    // 3

  #if defined(ADC0_STR)
   potIndex = EZAppAddFieldAnalogValue(                                    // 4
         (rom char *)ADC0_STR, 
         EZAPP_ANALOG_TYPE_GAUGE, 
         ADC_MAX
      );
  #endif

  #if defined(ADC1_STR)
   pot2Index = EZAppAddFieldAnalogValueScaled(                             // 5
      (rom char *)ADC1_STR, 
         EZAPP_ANALOG_TYPE_GAUGE, 
         0,
         ADC_MAX,
         330,
         2
      );
  #endif
  
  #if defined(ADC2_STR)
   photoIndex = EZAppAddFieldAnalogValue(                                  // 6
         (rom char *)ADC2_STR, 
         EZAPP_ANALOG_TYPE_SLIDER, 
         ADC_MAX
      );
  #endif

  #if defined(ADC3_STR)
   thermoIndex = EZAppAddFieldAnalogValue(                                 // 7
         (rom char *)ADC3_STR, 
         EZAPP_ANALOG_TYPE_TEXT_VALUE, 
         ADC_MAX
      );
  #endif
  
  #if defined(ADC0_STR)
   STRIndex = EZAppAddFieldText(ADC0_STR, 0);                              // 8
   sprintf(str, "Reading...");
   EZAppSetValueString(STRIndex, str);
  #endif

   diginIndex = EZAppAddFieldDigitalValue((rom char *)"DIGITAL", 4);     // 9
   
   EZAppAddFieldSpacerDynamic(0x0000FF, 12, (rom char*)"Outputs");   //10
   
  #if defined(DAC0_STR)
   DAC0Index = EZAppAddFieldAnalogValue(                                   // 11
         (rom char *)DAC0_STR, 
         EZAPP_ANALOG_TYPE_SLIDER_RW, 
         DAC0_MAX
      );
  #endif
  
  #if defined(DAC0_STR) || defined(DAC1_STR)
   DAC1Index = EZAppAddFieldAnalogValue(                                   //12
        #if defined(DAC1_STR)
         (rom char *)DAC1_STR, 
        #else
         (rom char *)DAC0_STR, 
        #endif
         EZAPP_ANALOG_TYPE_RW_TEXT_VALUE, 
        #if defined(DAC1_STR)
         DAC1_MAX
        #else
         DAC0_MAX
        #endif
      );
  #endif
 
   pulldownIndex = EZAppAddFieldPulldownValue(        //13
         (rom char*)"LEDS", 
         3, 
         (rom char*)"Off\tOn\tFrom Buttons"
      );
   EZAppSetValue(pulldownIndex, 2);
 
   ezapp_buttons_t buttonCfg;
   memset((void*)&buttonCfg, 0, sizeof(buttonCfg));
   buttonCfg.numButtons = 3;
   buttonCfg.oneState = 1;
   buttonCfg.fillRow = 1;
  #ifndef LED2_STR
   #define LED2_STR  "INVALID"
  #endif
  #ifndef LED1_STR
   #define LED1_STR  "INVALID"
  #endif
  #ifndef LED0_STR
   #define LED0_STR  "INVALID"
  #endif
   static char buttonString[48];
   sprintf(buttonString, "%s\t%s\t%s", LED0_STR, LED1_STR, LED2_STR);
   buttonIndex = EZAppAddFieldButtons(buttonCfg, NULL, buttonString);      //14

  #if defined(DIGOUT0_STR)
   buttonCfg.numButtons = 1;
   buttonCfg.oneState = 0;
   buttonCfg.fillRow = 1;
   digout0Index = EZAppAddFieldButtonsROM(buttonCfg, (rom char*)DIGOUT0_STR, (rom char*)"LOW\tHIGH");    //15
  #endif
  
  #if defined(DIGOUT1_STR)
   buttonCfg.numButtons = 1;
   buttonCfg.oneState = 0;
   buttonCfg.fillRow = 1;
   digout1Index = EZAppAddFieldButtonsROM(buttonCfg, (rom char*)DIGOUT1_STR, (rom char*)"LOW\tHIGH");    //16
  #endif
  
   for(;;)
   {
      restart_wdt();
      
      HW_TASK();
      
      EZAppTask();

     #if defined(__TICK_H__)
      if ((TickGet() - t) >= (TICK)TICKS_PER_SECOND)
      {
         t = TickGet();
         
         digitalVal++;
      }
     #endif
      
     #if defined(ADC0_CHANNEL)
      ADC0_CHANNEL();
      EZAppSetValue(potIndex, ADC_READ());

      #if defined(__PCM__) || defined(__PCH__)
       sprintf(str, "Value read is 0x%LX", ADC_READ());
      #else
       sprintf(str, "Value read is 0x%X", ADC_READ());
      #endif
      EZAppSetValueString(STRIndex, str);
     #endif

     #if defined(ADC1_CHANNEL)
      ADC1_CHANNEL();
      EZAppSetValue(pot2Index, ADC_READ());
     #endif

     #if defined(ADC2_CHANNEL)
      ADC2_CHANNEL();
      EZAppSetValue(photoIndex, ADC_READ());
     #endif

     #if defined(ADC3_CHANNEL)
      ADC3_CHANNEL();
      EZAppSetValue(thermoIndex, ADC_READ());
     #endif
    
     #if defined(DAC0_SET)
      if (EZAppGetKbhit(DAC0Index))
      {
         DAC0_SET(EZAppGetValue(DAC0Index));
      }
     #endif
      
     #if defined(DAC1_SET) || defined(DAC0_SET)
      if (EZAppGetKbhit(DAC1Index))
      {
        #if defined(DAC1_SET)
         DAC1_SET(EZAppGetValue(DAC1Index));
        #else
         DAC0_SET(EZAppGetValue(DAC1Index));
        #endif
      }
     #endif
     
     #if defined(DIGOUT0_PIN)
      if (EZAppGetKbhit(digout0Index))
      {
         if (EZAppGetValue(digout0Index))
         {
            DIGOUT_SET(DIGOUT0_PIN, 1);
         }
         else
         {
            DIGOUT_SET(DIGOUT0_PIN, 0);
         }
      }
     #endif
     
     #if defined(DIGOUT1_PIN)
      if (EZAppGetKbhit(digout1Index))
      {
         if (EZAppGetValue(digout1Index))
         {
            DIGOUT_SET(DIGOUT1_PIN, 1);
         }
         else
         {
            DIGOUT_SET(DIGOUT1_PIN, 0);
         }
      }
     #endif

      if (EZAppGetValue(pulldownIndex) == 0)
         newLedValue = 0;
      else if (EZAppGetValue(pulldownIndex) == 1)
         newLedValue = 0xFF;
      else
      {
         newLedValue = EZAppGetValue(buttonIndex);
      }

     #if defined(LED0_PIN)
      LED_SET(LED0_PIN, bit_test(newLedValue, 0));
     #endif
     #if defined(LED1_PIN)
      LED_SET(LED1_PIN, bit_test(newLedValue, 1));
     #endif
     #if defined(LED2_PIN)
      LED_SET(LED2_PIN, bit_test(newLedValue, 2));
     #endif
      
     #if !defined(BUTTON0_PRESSED)
      EZAppSetValue(diginIndex, digitalVal);
     #elif defined(BUTTON0_PRESSED) && defined(BUTTON1_PRESSED)
      EZAppSetValue(diginIndex, BUTTON0_PRESSED() + (uint8_t)BUTTON1_PRESSED()*2);
     #else
      EZAppSetValue(diginIndex, BUTTON0_PRESSED());
     #endif
   }
}
