/////////////////////////////////////////////////////////////////////////
////                       ex_dmx_controller.c                       ////
////                                                                 ////
//// An example program showing how to the DMX Driver to create a    ////
//// DMX Controller.  The DMX Driver requires the use of one of two  ////
//// I/O pins for RS485 communication.                               ////
////                                                                 ////
//// This is an example of DMX Controller device which uses two      ////
//// channels to set the duty cycle of a PWM signal on a slave       ////
//// device which starting at channel 1.  This example can be used   ////
//// in conjunction with ex_dmx_slave.c to demonstrate its           ////
//// functionality.                                                  ////
////                                                                 ////
//// This example will work on the PCM, PCH and PCD compiler.  The   ////
//// following conditional compilation lines have been made.  Change ////
//// the device, clock, rs232, etc. as need for your hardware        ////
//// configuration.                                                  ////
////                                                                 ////
/////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2016 Custom Computer Services         ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
/////////////////////////////////////////////////////////////////////////

#include <18F45K22.h>
#fuses NOWDT
#device ADC=10
#use delay(clock=64MHz, crystal=16MHz)
#use rs232(UART1,baud=9600, stream=PC)

//Setup DMX Driver Settings
#define DMX_TYPE        DMX_CONTROLLER
#define DMX_XMIT        PIN_B3
#define DMX_ENABLE      PIN_B4

#define DMX_UNITS       1
#define DMX_CHANNELS    2

//Include DMX Driver
#include <dmx.c>

#include <tick.c>

void main()
{
   TICK DMXTick;
   unsigned int16 PWMDuty;
   
   delay_ms(100);
   
   //Initialize the DMX Driver
   DMXInit();
   TickInit();
   
   printf("\r\nex_dmx_controller.c\r\n");
   
   setup_adc_ports(sAN0, VSS_VDD);
   setup_adc(ADC_CLOCK_INTERNAL);
   set_adc_channel(0);
   
   DMXTick = TickGet();
   
   while(TRUE)
   {
      if(TickIsExpired(DMXTick, (TICKS_PER_SECOND / 40)))
      {
         PWMDuty = ((unsigned int32)read_adc() * 1000) / 1023;
         
         //Set the DMX Channel Data
         DMXSetChannel(1, make8(PWMDuty, 0));
         DMXSetChannel(2, make8(PWMDuty, 1));
         
         //Send the DMX Data
         DMXCommit();
         
         DMXTick = TickGet();
      }
   }
}  
