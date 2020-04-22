/////////////////////////////////////////////////////////////////////////
////                           EX_PCD_STWT1.C                        ////
////                                                                 ////
////  This program uses timer 1 and interrupts to keep a             ////
////  real time seconds counter.  A simple stop watch function is    ////
////  then implemented.                                              ////
////                                                                 ////
////  This example will work with the PCD compiler.  The             ////
////  following conditional compilation lines are used to include a  ////
////  valid device for each compiler.  Change the device, clock and  ////
////  RS232 pins for your hardware if needed.                        ////
/////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2004 Custom Computer Services         ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
/////////////////////////////////////////////////////////////////////////


#include <24HJ128GP306.h>
#use delay(crystal=20mhz)
//#use rs232(UART1, baud=9600)   // Use for UART port
#use RS232(ICD)                // Use for ICD port  


#define PERIOD_VALUE (int16)(getenv("INSTRUCTION_CLOCK")/256-1)
      // The 256 is because the setup timer1 divides by 256

int16 seconds;      // A running seconds counter

#INT_TIMER1                        // This function is called every time
void clock_isr() {                 // timer 1 overflows (PERIOD_VALUE).  One second.
   ++seconds;
}


void main() {

   setup_timer1(TMR_INTERNAL | TMR_DIV_BY_256, PERIOD_VALUE);
   set_timer1(0);
   clear_interrupt(INT_TIMER1);
   enable_interrupts(INT_TIMER1);
   enable_interrupts(INTR_GLOBAL);

   while(TRUE) {
      printf("Press any key to begin.\n\r");
      getc();
      seconds = 0;
      set_timer1(0);
      printf("Press any key to stop.\n\r");
      getc();
      printf("%u seconds.\n\r", seconds);
   }
}

