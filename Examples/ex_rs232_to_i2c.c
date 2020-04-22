///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                       ex_rs232_to_i2c.c                           ////
////                                                                   ////
//// An example program that communicates with any I2C device.         ////
//// Slave address, payload to transmit and number of bytes to         ////
//// receive from I2C device can be controlled through a serial        ////
//// interface.                                                        ////
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

#if 1
   #include <e3.h>
   #use i2c(MASTER, I2C1)  // SDA = PIN_B4, SCL = PIN_B6
#elif defined(__PCM__)
   #include <16F877.H>
   #use delay(crystal=20MHz)
   #fuses NOWDT
   #use rs232(UART1, baud=9600, errors)
   #use i2c(MASTER, I2C1)
#elif defined(__PCH__)
   #include <18F4520.H>
   #use delay(crystal=20MHz)
   #fuses NOWDT
   #use rs232(UART1, baud=9600, errors)
   #use i2c(MASTER, I2C1)
#elif defined(__PCD__)
   #include <24FJ128GA006.h>
   #fuses NOWDT
   #use delay(crystal=20MHz)
   #use rs232(UART1, baud=9600, errors)
   #use i2c(MASTER, I2C1)
#endif

#include <stdlib.h>
#include <input.c>

unsigned int8 payload[64];

void main(void)
{
   unsigned int8 slave = 0xA0;
   unsigned int8 num, i;
   char c;
   
   printf("\r\n");
   
   for(;;)
   {
      do
      {
         printf("R)ead or W)rite or S)lave address 0x%X\r\n", slave);
         c = toupper(getc());
      } while ((c != 'R') && (c != 'W') && (c != 'S'));
      
      printf("\r\n");
      
      if (c == 'S')
      {
         printf("New slave address (hex): ");
         slave = gethex();
         printf("\r\n\r\n");
         continue;
      }
      
      if (c == 'R')
      {
         printf("Read how many bytes (decimal): ");
      }
      else
      {
         printf("Write how many bytes (decimal) (max=%u): ", sizeof(payload));
      }
            
      num = get_int();
      printf("\r\n");
      
      if (num == 0)
      {
         printf("\r\n");
         continue;
      }
         
      if (c == 'R')
      {
         i2c_start();
         i2c_write(slave | 1);
         while (num)
         {
            num--;
            c = i2c_read(num != 0);
            printf("%X ", c);
         }
         i2c_stop();
      }
      else
      {
         if (num > sizeof(payload)) num = payload;
         
         for(i=0; i<num; i++)
         {
            printf("Byte %u (hex): ", i);
            payload[i] = gethex();
            printf("\r\n");
         }
         
         i2c_start();
         i2c_write(slave);
         for(i=0; i<num; i++)
         {
            if (i2c_write(payload[i]) != 0)
            {
               printf("ACK ERROR %u", i);
               break;
            }
         }
         i2c_stop();
      }
      printf("\r\n");
      printf("\r\n");
   }
}
