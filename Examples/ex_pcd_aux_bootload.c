///////////////////////////////////////////////////////////////////////////
////                     EX_PCD_AUX_BOOTLOAD.c                         ////
////                                                                   ////
////  This program is an example application program that can be       ////
////  be loaded, recompiled and reloaded by a standalone bootloader.   ////
////  This application is specifically written for a bootloader that   ////
////  resides in the Auxiliary Memory Region of the Flash.  The        ////
////  Auxiliary Memory Region is only available on some PIC24EP and    ////
////  dsPIC33EP devices.                                               ////
////                                                                   ////
////  Before loading this program the bootloader                       ////
////  (EX_PCD_AUX_BOOTLOADER.c) must be compiled and loaded into the   ////
////  target chip.                                                     ////
////                                                                   ////
////  Use an RS232 link and the SIOW.EXE program to load this HEX file ////
////  into the target chip.  See ex_pcd_aux_bootloader.c for more      ////
////  information.                                                     ////
////                                                                   ////
////  This example will work with the PCD compiler.  The following     ////
////  conditional compilation lines are used to include a valid device ////
////  for the compiler.  Change the device, clock and RS232 pins for   ////
////  your hardware if needed.                                         ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2016 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS    ////
//// C compiler.  This source code may only be distributed to other    ////
//// licensed users of the CCS C compiler.  No other use,              ////
//// reproduction or distribution is permitted without written         ////
//// permission.  Derivative programs created using this software      ////
//// in object code form are not restricted in any way.                ////
///////////////////////////////////////////////////////////////////////////

#include <24EP512GU810.h>
#fuses NOWDT,RESET_AUX
#use delay(clock=120MHz, crystal=8MHz)

#pin_select U1TX = PIN_F5
#pin_select U1RX = PIN_F4  
#use rs232(Baud=9600,UART1,errors)

#define PUSH_BUTTON  PIN_D6  
#define LED1         PIN_A0
#define LED2         PIN_A1
#define LED3         PIN_A2

rom char version[] = "Application Version 1.0";

void DisplayLeds(unsigned int8 Led)
{
   output_low(LED1);
   output_low(LED2);
   output_low(LED3);
   
   switch(Led)
   {
      case 0:
         output_high(LED1);
         break;
      case 1: 
         output_high(LED2);
         break;
      case 2:
         output_high(LED3);
         break;
   }
}

#INT_TIMER1
void led_isr(void)
{
   static unsigned int8 Led = 0;
   
   DisplayLeds(Led);
   
   if(++Led >= 3)
      Led = 0;
}

void main()
{
   delay_ms(100);

   printf("\r\n%s\r\n", version);
   
   printf("You can put whatever code you want here.\r\n");
   
   printf("So enjoy!\r\n");
   
   setup_timer1(TMR_INTERNAL | TMR_DIV_BY_256, 46874);   //200 ms period;
   
   enable_interrupts(INT_TIMER1);
   enable_interrupts(GLOBAL);
   
   while(TRUE)
   {
      
   }
}
