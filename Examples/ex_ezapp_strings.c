///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                        ex_ezapp_strings.c                         ////
////                                                                   ////
//// A demo that shows how to strings that can be edited on the        ////
//// smartphone/tablet host and how the PIC can read new values        ////
//// sent by the host.  One editable string is held in RAM, another    ////
//// is held in EE.                                                    ////
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

#if EZAPP_EEPROM_SIZE   //created in EZApp.h if EEPROM present, or overwritten by user if he/she wants to use their own API for external values
   #define MY_EE_STRING_ADDY  16 //store our data at address 16 of the EE

   #if defined(__PCB__)||defined(__PCM__)||defined(__PCH__)||defined(__PCD__)
      // store an initial value into the HEX file
      #rom getenv("EEPROM_ADDRESS")+MY_EE_STRING_ADDY = {"HELLO"}
   #endif
#endif

void main(void)
{
   char editTextStr[32] = "TEXT TO EDIT";
   char viewTextStr[32] = "READ-ONLY TEXT";
   ezapp_field_index_t editTextIdx = EZAPP_FIELD_INDEX_INVALID;
   ezapp_field_index_t viewTextIdx = EZAPP_FIELD_INDEX_INVALID;
   ezapp_field_index_t edit2TextIdx = EZAPP_FIELD_INDEX_INVALID;
   ezapp_field_index_t view2TextIdx = EZAPP_FIELD_INDEX_INVALID;

   HW_INIT();

   EZAppInit();
   
   EZAppSetTitleROM((rom char *)"ex_ezapp_strings.c");   //0

   EZAppSetValueStringROM(                               //1
         EZAppAddFieldStringDynamic(0, +3), 
         (rom char*)"String example"
      );

   viewTextIdx = EZAppAddFieldText((rom char*)"VIEW_RAM:", 0);  //2
   EZAppSetValueString(viewTextIdx, viewTextStr);
   
   editTextIdx = EZAppAddFieldText((rom char*)"EDIT_RAM:", sizeof(editTextStr));//3
   EZAppSetValueString(editTextIdx, editTextStr);

  #if EZAPP_EEPROM_SIZE
   view2TextIdx = EZAppAddFieldText((rom char*)"VIEW_EE:", 0);  //4
   EZAppSetValueStringEE(view2TextIdx, MY_EE_STRING_ADDY);
   
   edit2TextIdx = EZAppAddFieldText((rom char*)"EDIT_EE:", 32);   //5
   EZAppSetValueStringEE(edit2TextIdx, MY_EE_STRING_ADDY);
  #endif
  
   for(;;)
   {
      restart_wdt();
      
      HW_TASK();
      
      EZAppTask();
      
      if (EZAppGetKbhit(editTextIdx))
      {
         strcpy(viewTextStr, EZAppGetValueString(editTextIdx));
         EZAppSetValueString(viewTextIdx, viewTextStr);
      }
      
     #if EZAPP_EEPROM_SIZE
      if (EZAppGetKbhit(edit2TextIdx))
      {
         EZAppSetValueStringEE(view2TextIdx, EZAppGetValueStringEE(edit2TextIdx));
      }
     #endif
   }
}
