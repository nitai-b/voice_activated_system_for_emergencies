///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                    ex_i2c_ldr_bootloader.c                        ////
////                                                                   ////
//// I2C Loader, an example application that can be changed by the     ////
//// bootloader.  This is a template of how to write an application    ////
//// compatible with the I2C Loader.                                   ////
////                                                                   ////
//// For full documentation, see i2c_bootloader.h.                     ////
////                                                                   ////
//// i2c_bootloader.h contains a function, i2c_isr_bootload_check(),   ////
//// that must be called in your applications I2C Slave ISR to check   ////
//// for a GOTO BOOTLOADER message.  See the example                   ////
//// i2c_slave_interrupt() in this demo to see how to implement        ////
//// calling the i2c_isr_bootload_check() in your I2C slave ISR.       //// 
//// If the user application disables global interrupts then this      ////
//// interrupt handler will not work and the software that talks to    ////
//// PICmicro won't be able to put the PIC into bootloader mode.       ////
////                                                                   ////
//// i2c_bootloader.h has an #import() to add the bootloader portion   ////
//// of the code; that means it creates one HEX file that contains     ////
//// both the bootloader and application in one HEX file.  If you get  ////
//// a compile error because you are missing this HEX file, then you   ////
//// need to compile ex_i2c_ldr_bootloader.c first.                    ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2016 Custom Computer Services           ////
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

#include <18f4520.h>
#use delay(crystal=20MHz)
#fuses NOWDT   //fuses are listed here, but the bootloader can't change them
#fuses NOWRT   //fuses are listed here, but the bootloader can't change them
#define INT_I2C_BOOTLOADER INT_SSP
#use i2c(SLAVE, I2C1, address=0xA0, STREAM=STREAM_I2C_BOOTLOADER) //SDA=PIN_C4, SCL=PIN_C3

#use rs232(UART1, baud=9600, ERRORS)
#define PIN_LED0  PIN_B5
#define PIN_LED1  PIN_B4


//////////// end configuration /////////////

#include "i2c_bootloader.h"

rom char ver[] = "1.0";

#use timer(tick=10ms, bits=16, ISR)

#if (defined(INT_SSP) && (INT_I2C_BOOTLOADER == INT_SSP))
#INT_SSP
#elif (defined(INT_SSP2) && (INT_I2C_BOOTLOADER == INT_SSP2))
#INT_SSP2
#elif (defined(INT_SI2C) && (INT_I2C_BOOTLOADER == INT_SI2C))
#INT_SI2C
#elif (defined(INT_SI2C1) && (INT_I2C_BOOTLOADER == INT_SI2C1))
#INT_SI2C1
#elif (defined(INT_SI2C2) && (INT_I2C_BOOTLOADER == INT_SI2C2))
#INT_SI2C2
#elif (defined(INT_SI2C3) && (INT_I2C_BOOTLOADER == INT_SI2C3))
#INT_SI2C3
#else
#error missing ISR preprocessor command for this hardware
#endif
void i2c_slave_interrupt()
{
   unsigned int8 state, read;

   state = i2c_isr_state(STREAM_I2C_BOOTLOADER);

   if(state <= 0x80) //Master is sending data
   {
      if(state == 0x80)
         read = i2c_read(STREAM_I2C_BOOTLOADER, 2);  //Passing 2 as parameter, causes the function to read the SSPBUF without releasing the clock
      else
         read = i2c_read(STREAM_I2C_BOOTLOADER);
         
      if (state < 0x80)
      {
         i2c_isr_bootload_check(state, read);
      }
   }

   if(state >= 0x80)                      //Master is requesting data
   {
      i2c_write(STREAM_I2C_BOOTLOADER, 0);  //we don't have any data, write dummy data
   }
}

void main(void)
{
   unsigned int16 tick;
   
   printf("Application %s running\r\n", ver);
   
   for(;;)
   {
      if ((get_ticks() - tick) >= TICKS_PER_SECOND/2)
      {
         tick = get_ticks();
         output_toggle(PIN_LED0);
      }
   }
}
