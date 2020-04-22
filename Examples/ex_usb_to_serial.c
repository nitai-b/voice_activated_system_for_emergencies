/////////////////////////////////////////////////////////////////////////
////                                                                 ////
////                         ex_usb_to_serial.c                      ////
////                                                                 ////
//// Acts a serial to USB converter.  LED1 is off when not connected ////
//// to the PC, on when connected to the PC and blinks on traffic.   ////
//// The serial baud rate is changed when the USB CDC baud rate is   ////
//// changed.                                                        ////
////                                                                 ////
/////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2005 Custom Computer Services         ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
/////////////////////////////////////////////////////////////////////////

#include <ex_usb_common.h>

// Includes all USB code and interrupts, as well as the CDC API
#include <usb_cdc.h>

typedef unsigned int16 TICK;
#define TickGet() get_ticks()
#use timer(timer=1, tick=1ms, bits=16, noisr)

#define INIT_UART_BAUDRATE   9600
#use rs232(UART1, baud=INIT_UART_BAUDRATE, stream=STREAM_UART1, RECEIVE_BUFFER=250, errors)

TICK g_LedBlinkTick;

void LedBlinkTask(void)
{
   if ((TickGet() - g_LedBlinkTick) >= TICKS_PER_SECOND/10)
   {
      LED_ON(LED1);
   }
}

void LedBlinkDo(void)
{
   LED_OFF(LED1);
   g_LedBlinkTick = TickGet();
}

void main(void)
{
   unsigned int32 currBaud, newBaud;
   char c;
   
   restart_wdt();
   setup_wdt(WDT_ON | WDT_1S);
   restart_wdt();

   setup_comparator(NC_NC_NC_NC);
   setup_adc_ports(NO_ANALOGS);
   
   LEDS_OFF();
   
   usb_init_cs();

   currBaud = INIT_UART_BAUDRATE;

   //wait for serial terminal to open COM port
   for(;;)
   {
      restart_wdt();
      
      usb_task();
      
      if (!usb_enumerated())
      {
         LedBlinkDo();
         continue;
      }
      
      LedBlinkTask();
      
      newBaud = usb_cdc_line_coding.dwDTERrate;
         
      if (newBaud != currBaud)
      {
         if (newBaud == 115200)
         {
            setup_uart(115200, STREAM_UART1);
         }
         else
         {
            switch(newBaud)
            {
               case 300:   setup_uart(300, STREAM_UART1);   break;
               case 1200:   setup_uart(1200, STREAM_UART1);   break;
               case 2400:   setup_uart(2400, STREAM_UART1);   break;
               case 4800:   setup_uart(4800, STREAM_UART1);   break;
               default:
               case 9600:   setup_uart(9600, STREAM_UART1);   break;
               case 19200:   setup_uart(19200, STREAM_UART1);   break;
               case 38400:   setup_uart(38400, STREAM_UART1);   break;
               case 57600:    setup_uart(57600, STREAM_UART1);   break;
            }
         }
      }
      
      currBaud = newBaud;
      
      while(usb_cdc_kbhit())
      {
         LedBlinkDo();
         
         c = usb_cdc_getc();
         
         fputc(c, STREAM_UART1);
      }
      
      while(kbhit(STREAM_UART1) && usb_cdc_putready())
      {
         LedBlinkDo();

         c = fgetc(STREAM_UART1);
         
         usb_cdc_putc(c);
      }
   }
}
