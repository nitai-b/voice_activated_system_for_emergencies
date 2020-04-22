///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                      ex_ezapp_security.c                          ////
////                                                                   ////
//// This example implements the EZAPP_CHECK_AUTHENTICATION() macro    ////
//// to verify incoming passwords from the smartphone/tablet host.     ////
//// When the example starts, EZAppSetAuthLevel() is used to initially ////
//// lock the device and wait for passwords to come from the host.     ////
//// Host won't have access unless they provide the correct password,  ////
//// "read" for read-only access or "write" for full access.           ////
//// After 10 minutes the authorization times out.                     ////
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

// EXAMPLE_EZAPP_SECURITY tells ex_ezapp_common.h to create 
// EZAPP_CHECK_AUTHENTICATION() macro that points to MyEZAppAuthCheck().
#define EXAMPLE_EZAPP_SECURITY

// Common definitions for all the examples.
// Hardware is configured in this .h file and then populates several
// macros/definitions used by all the EZ App Lynx examples.
#include "ex_ezapp_common.h"

#include <EZApp.h>

#include <string.h>

struct
{
   bool valid;
   TICK t;
   uint16_t seconds;
} g_MyAuth = {false};

// lock (remove authorization) after 10 minutes of being logged in, or if
// the user disconnects bluetooth.
void MyEZAppAuthTask(void)
{
   if (g_MyAuth.valid)
   {
      if ((TickGet() - g_MyAuth.t) >= TICKS_PER_SECOND)
      {
         g_MyAuth.t += TICKS_PER_SECOND;
         g_MyAuth.seconds++;
      }
      
      if 
      (
         !BtSppIsConnected() ||
         (g_MyAuth.seconds >= 600)
      )   
      {
         g_MyAuth.valid = false;
      
         EZAppSetAuthLevel(EZAPP_AUTH_LOCKED);

         UARTConsolePutsROM((rom char *)"MyEZAppAuthTask() LOCK\r\n");
      }
   }
}

// check incoming password from the App and see if we should unlock the
// system.
bool MyEZAppAuthCheck(char *str)
{
   ezapp_authlevel_t newAuth = EZAPP_AUTH_LOCKED;
   bool ret = false;
   
   static char write[] = "write";
   static char read[] = "read";

   UARTConsolePutsROM((rom char *)"MyEZAppAuthCheck(\"");
   UARTConsolePuts(str);
   UARTConsolePutsROM((rom char *)"\") ");

   if (strcmp(str, write) == 0)
   {
      newAuth = EZAPP_AUTH_OPEN;
      ret = true;
      UARTConsolePutsROM((rom char *)"OPEN ");
   }
   
   if (strcmp(str, read) == 0)
   {
      newAuth = EZAPP_AUTH_READ_ONLY;
      ret = true;
      UARTConsolePutsROM((rom char *)"READ_ONLY ");
   }

   UARTConsolePutsROM((rom char *)"\r\n");
   
   EZAppSetAuthLevel(newAuth);
   
   g_MyAuth.valid = ret;
   g_MyAuth.seconds = 0;
   g_MyAuth.t = TickGet();
   
   return(ret);
}

void main(void)
{
   ezapp_field_index_t adcIndex = EZAPP_FIELD_INDEX_INVALID, 
      dacIndex = EZAPP_FIELD_INDEX_INVALID, 
      buttonIndex = EZAPP_FIELD_INDEX_INVALID;

   HW_INIT();
   
   ADC_INIT();
   ADC0_CHANNEL();

   DAC0_INIT();

   UARTConsoleInit(19200);

   UARTConsolePutsROM((rom char *)"ex_ezapp_security.c START\r\n");

   EZAppInit();
   
   EZAppSetAuthLevel(EZAPP_AUTH_LOCKED);
      
   EZAppSetTitleROM((rom char *)"ex_ezapp_security.c");     //0

   EZAppSetValueStringROM(
         EZAppAddFieldString(), 
         (rom char*)"EZ App Lynx Auth Demo"
      );                                                    //1
  
   adcIndex = EZAppAddFieldAnalogValue(
         (rom char *)ADC0_STR, 
         EZAPP_ANALOG_TYPE_GAUGE, 
         ADC_MAX
      );                                                   //2

   dacIndex = EZAppAddFieldAnalogValue(
         (rom char *)DAC0_STR, 
         EZAPP_ANALOG_TYPE_SLIDER_RW, 
         DAC0_MAX
      );                                                  //3
  
   buttonIndex = EZAppAddFieldButtonTwoState(
         (rom char *)LED2_STR, 
         (rom char *)"OFF\tON"
      );                                                 //4
   
   for(;;)
   {
      restart_wdt();
      
      HW_TASK();
      
      EZAppTask();
      
      MyEZAppAuthTask();
      
      EZAppSetValue(adcIndex, ADC_READ());
      
      if (EZAppGetKbhit(dacIndex))
      {
         DAC0_SET(EZAppGetValue(dacIndex));
      }
         
      LED_SET(LED0_PIN, BtSppIsConnected());
      
      LED_SET(LED1_PIN, EZAppIsConnected());
      
      LED_SET(LED2_PIN, EZAppGetValue(buttonIndex));      
   }
}
