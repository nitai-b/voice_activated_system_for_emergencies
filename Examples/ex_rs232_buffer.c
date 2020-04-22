/////////////////////////////////////////////////////////////////////////
////                       EX_RS232_BUFFER.C                         ////
////                                                                 ////
////  This program shows how to setup and use the built-in receive   ////
////  and transmit buffers that was added to the #use rs232()        ////
////  library in version 5 of the PCM, PCH and PCD compilers.        ////
////                                                                 ////
////  This example sets up a 32 character receive and transmit       //// 
////  buffer for UART1.  The compiler automatically creates the RDA  ////
////  interrupt to receive data and place it in the software buffer. ////
////  Then calls to kbhit(), getc(), gets(), etc. will check the     ////
////  software buffer for new data.                                  ////
////                                                                 ////
////  For the transmit buffer data calls to printf(), putc(), etc.   ////
////  moves data into the software buffer.  If the option TXISR is   ////
////  used the compiler will automatically create the TBE interrupt  ////
////  which will move the data from the software buffer to the HW    ////
////  transmit register.  If not using the TXISR option then the     ////
////  function putc_send() needs to be called often to move data     ////
////  data from the software buffer to the hardware transmit         ////
////  register.                                                      ////
////                                                                 ////
////  This example uses the HW UART, both the receive and transmit   ////
////  buffers can also be used with a software UART.  To use the     ////
////  receive buffer with a software UART the only condition is that ////
////  the rcv pin needs to be one of the external interrupt pins.    ////
////  To use the transmit buffer with a software UART the only       ////
////  condition is that the putc_send() function needs to be called  ////
////  to transmit data.                                              ////
////                                                                 ////
////  This example will work with the PCM, PCH and PCD compilers.    ////
////  The following conditional compilation lines are used to        ////
////  include a valid device for each compiler.  Change the device,  ////
////  clock and RS232 pins for your hardware if needed.              ////
/////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2015 Custom Computer Services         ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
/////////////////////////////////////////////////////////////////////////

//Comment out following define if you don't want to use TBE interrupt to
//transmit data.
#define USE_TX_ISR

#if defined(__PCM__)
 #include <16F887.h>
 #fuses NOWDT
 #use delay(crystal=20MHz)
#elif defined(__PCH__)
 #include <18F45K22.h>
 #fuses NOWDT
 #use delay(crystal=16MHz)
#elif defined(__PCD__)
 #include <24FJ128GA006.h>
 #fuses NOWDT
 #use delay(crystal=20MHz)
#endif

#ifdef USE_TX_ISR
 #use rs232(UART1, BAUD=9600, RECEIVE_BUFFER=32, TRANSMIT_BUFFER=32, TXISR)
#else
 #use rs232(UART1, BAUD=9600, RECEIVE_BUFFER=32, TRANSMIT_BUFFER=32)
#endif

void main()
{
   char c;
   
   printf("\r\nEX_RS232_BUFFER.c\r\n\n");
   
   enable_interrupts(GLOBAL);
   
   while(TRUE)
   {
     #ifndef USE_TX_ISR
      putc_send();      //When not using the TXISR option with a HW UART
                        //this needs called often to cause data be moved
                        //from software buffer to HW transmit register.
                        //If using a software UART it also needs called to
                        //transmit data.
     #endif
     
      if(kbhit())
      {
         c = getc();
         putc(c);
         
         if(c == '\r')
            putc('\n');
         else if(c == '\b')
         {
            putc(' ');
            putc(c);
         }
      }
   }
}
         
