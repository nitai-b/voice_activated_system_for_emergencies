///////////////////////////////////////////////////////////////////////////
////                    EX_PCD_AUX_BOOTLOADER.C                        ////
////                                                                   ////
////  This program is an example standalone bootloader which uses the  ////
////  Auxiliary Memory Region for the bootloader.  The Auxiliary       ////
////  Memory is only available on some PIC24EP and dsPIC33EP devices.  ////
////                                                                   ////
////  This program must be loaded into a target chip using a device    ////
////  programmer.  Afterwards this program may be used to load new     ////
////  versions of the application program.                             ////
////                                                                   ////
////  This bootloader is designed to detect a pushbutton low on reset. ////
////  It will then use the RS232 link to download a new program.       ////
////  Otherwise the application program is started.                    ////
////                                                                   ////
////  Use an RS232 link and the SIOW.EXE program to load a new HEX     ////
////  file into the target chip. See example EX_PCD_AUX_BOOTLOAD.C.    ////
////                                                                   ////
////  Uncomment the define BOOTLOADER_MODE2X to use an alternate       ////
////  bootloader that receives each line in the hex file twice and     ////
////  compares them before acknowledging the line and writing it to    ////
////  the target chip's memory.  This mode only works with the         ////
////  CCS_BOOTLOADER.exe program, add MODE2X to the command line to    ////
////  use this mode.                                                   ////
////                                                                   ////
////  The config bits will be write protected, in software, by the     ////
////  rom_modify() function.  To allow config bits to change then      ////
////  you need to #define ROM_WRITE_CAN_MODIFY_CONFIG_BITS             ////
////                                                                   ////
////  This example will work with the PCD compiler.  The               ////
////  following conditional compilation lines are used to include a    ////
////  valid device for the compiler.  Change the device, clock,        ////
////  push button, and RS232 pins for your hardware if needed.         ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2016 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS    ////
//// C compiler.  This source code may only be distributed to other    ////
//// licensed users of the CCS C compiler.  No other use,              ////
//// reproduction or distribution is permitted without written         ////
//// permission.  Derivative programs created using this software      ////
//// in object code form are not restricted in any way.                ////
///////////////////////////////////////////////////////////////////////////

// see explanation above
//#define BOOTLOADER_MODE2X

// see explanation above
//#define ROM_WRITE_CAN_MODIFY_CONFIG_BITS

#include <24EP512GU810.h>
#fuses NOWDT,RESET_AUX  //The RESET_AUX fuse causes PIC to reset into Auxiliary Memory
#build (AUX_MEMORY)     //Build code for Auxiliary Memory
#use delay(clock=120MHz, crystal=8MHz)

#pin_select U1TX = PIN_F5
#pin_select U1RX = PIN_F4  
#use rs232(Baud=9600,UART1,errors)

#define PUSH_BUTTON  PIN_D6
#define LED1         PIN_A0
#define LED2         PIN_A1

//The following defines are necessary to use PCD's serial bootloader, loader_pcd.c.
#define _bootloader

#define LOADER_SIZE        0
#define APPLICATION_START  0
#define APPLICATION_END    getenv("PROGRAM_MEMORY")

//Include PCD's serial bootloader
#include <loader_pcd.c>

//Example of how to use the Auxiliary Memory's ISR.  The Auxiliary Memory has 
//a single ISR for all interrupts, INT_AUX.  If more then one ISR is used the
//interrupt_active() and interrupt_enabled() functions can be used to determine
//which interrupt caused the interrupt to occur.  Additionally the 
//clear_interrupt() function needs to be used to clear interrupt flag of the
//interrupt that caused the interrupt to occur.
#INT_AUX
void auxialary_isr(void)
{
   if(interrupt_enabled(INT_TIMER1) && interrupt_active(INT_TIMER1))
   {
      output_toggle(LED1);
      clear_interrupt(INT_TIMER1);
   }
   
   if(interrupt_enabled(INT_TIMER2) && interrupt_active(INT_TIMER2))
   {
      output_toggle(LED2);
      clear_interrupt(INT_TIMER2);
   }
}

void main(void)
{
   if(!input(PUSH_BUTTON))
   {
      delay_ms(140); // wait for PLL
      
      printf("\r\nBootloader Version 1.0\r\n");

      // Let the user know it is ready to accept a download
      printf("\r\nWaiting for download...");
      
      setup_timer1(TMR_INTERNAL | TMR_DIV_BY_256, 58593);   //~250 ms period
      setup_timer2(TMR_INTERNAL | TMR_DIV_BY_256, 23437);   //~100 ms period
      
      enable_interrupts(INT_TIMER1);
//!      enable_interrupts(INT_TIMER2);
      enable_interrupts(GLOBAL);

      // Load the program
      load_program();
   }

   goto_address(APPLICATION_START);
}
