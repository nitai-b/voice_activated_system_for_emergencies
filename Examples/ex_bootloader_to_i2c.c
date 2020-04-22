///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                    ex_bootloader_to_i2c.c                         ////
////                                                                   ////
//// SIOW.EXE and CCSBOOTLOADER.EXE can send a HEX file to a COM port  ////
//// with software flow control.  This program takes those serial      ////
//// messages from that software and converts it to the I2C protocol   ////
//// compatible with this bootloader.                                  ////
////                                                                   ////
//// For full documentation, see i2c_bootloader.h.                     ////
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

//// The number of bytes stored per address for the target PICmicro.
//// This is necessary to scale the address in the HEX file to the proper
//// addressing scheme on the target PICmicro.
//// PCB and PCM chips (PIC10-PIC16), BPA = 2 (12 or 14bits per address, one address per instruction)
//// PCH chips (PIC18), BPA = 1 (1 byte per address, two addresses per instruction)
//// PCD chips (PIC24, dsPIC30, dsPIC30), BPA = 2 (2 bytes per address, two addresses per instruction)
#define TARGET_BPA   1  //PIC18 chips
//#define TARGET_BPA   2  //Everything but PIC18 chips

#define TARGET_I2C_SLAVE_ADDRESS 0xA0

#define BUFFER_LEN_LOD    64  //max line length in the hex file


/////////////////// end configuration //////////////////////////////////////

#include <stdlib.h>

unsigned int atoi_b16(char *s) {  // Convert two hex characters to a int8
   unsigned int result = 0;
   int i;

   for (i=0; i<2; i++,s++)  {
      if (*s >= 'A')
         result = 16*result + (*s) - 'A' + 10;
      else
         result = 16*result + (*s) - '0';
   }

   return(result);
}

void ext_pic_start_application(void)
{
   i2c_start();
   i2c_write(TARGET_I2C_SLAVE_ADDRESS);
   i2c_write(3);   //I2C_LOADER_COMMAND_GOTO_APPLICATION
   i2c_write(0x55);  //magic lsb
   i2c_write(0xAA);  //magic msb
   i2c_stop();
}

void ext_pic_start_bootloader(void)
{
   i2c_start();
   i2c_write(TARGET_I2C_SLAVE_ADDRESS);
   i2c_write(4);   //I2C_LOADER_COMMAND_GOTO_BOOTLOADER
   i2c_write(0x55);  //magic lsb
   i2c_write(0xAA);  //magic msb
   i2c_stop();
}

void ext_pic_write_memory(unsigned int32 extAddress, unsigned int8 *p, size_t count)
{
   #if (TARGET_BPA >= 2)
   extAddress /= TARGET_BPA;
   #endif
   
   i2c_start();
   i2c_write(TARGET_I2C_SLAVE_ADDRESS);
   i2c_write(1);   //I2C_LOADER_COMMAND_ROM_ADDRESS
   i2c_write(make8(extAddress, 0));
   i2c_write(make8(extAddress, 1));
   i2c_write(make8(extAddress, 2));
   i2c_write(make8(extAddress, 3));
   i2c_stop();
   
   i2c_start();
   i2c_write(TARGET_I2C_SLAVE_ADDRESS);
   i2c_write(2);   //I2C_LOADER_COMMAND_ROM_ACCESS
   while(count--)
   {
      i2c_write(*p++);
   }
   i2c_stop();
}

#define ACKLOD 0x06
#define XON    0x11
#define XOFF   0x13

void load_program(void)
{
   int1  do_ACKLOD, done=FALSE;
   int8  checksum, line_type;
   int16 l_addr,h_addr=0;
   int32 addr;
   int32 next_addr;
   int8  dataidx, i, count;
   int8  data[BUFFER_LEN_LOD / 2];
   struct
   {
      int8 idx;
      char buffer[BUFFER_LEN_LOD];
   } rBuffer[1];   

   while (!done)  // Loop until the entire program is downloaded
   {
      rBuffer[0].idx = 0;
      
      do {
         rBuffer[0].buffer[rBuffer[0].idx] = getc();
         
      } while ((rBuffer[0].buffer[rBuffer[0].idx++] != 0x0D) && (rBuffer[0].idx <= BUFFER_LEN_LOD));
  
      putc(XOFF);  // Suspend sender

      do_ACKLOD = TRUE;

      // Only process data blocks that start with ':'
      if (rBuffer[0].buffer[0] == ':')
      {
         count = atoi_b16 (&rBuffer[0].buffer[1]);  // Get the number of bytes from the buffer

         // Get the lower 16 bits of address
         l_addr = make16(atoi_b16(&rBuffer[0].buffer[3]),atoi_b16(&rBuffer[0].buffer[5]));

         line_type = atoi_b16 (&rBuffer[0].buffer[7]);

         addr = make32(h_addr,l_addr);
         
         checksum = 0;  // Sum the bytes to find the check sum value
         for (i=1; i<(rBuffer[0].idx-3); i+=2)
            checksum += atoi_b16 (&rBuffer[0].buffer[i]);
         checksum = 0xFF - checksum + 1;

         if (checksum != atoi_b16 (&rBuffer[0].buffer[rBuffer[0].idx-3]))
            do_ACKLOD = FALSE;
         else
         {
            // If the line type is 1, then data is done being sent
            if (line_type == 1)
               done = TRUE;
            else if (line_type == 4)
               h_addr = make16(atoi_b16(&rBuffer[0].buffer[9]), atoi_b16(&rBuffer[0].buffer[11]));
            else if (line_type == 0)
            {
               for (i = 9,dataidx=0; i < rBuffer[0].idx-3; i += 2)
                  data[dataidx++]=atoi_b16(&rBuffer[0].buffer[i]);
                  
               ext_pic_write_memory(addr, data, count);
            }
         }
      }

      if (do_ACKLOD)
         putc(ACKLOD);
      
      putc(XON);
   }

   putc(ACKLOD);
   putc(XON);
}

void main(void)
{
   char c;
   
   for(;;)
   {
      do
      {
         printf("\r\n\r\nPress ! to start the bootloader\r\n\r\n");
         c = getc();
      } while (c != '!');
      
      printf("Starting bootloader, ");
      printf("use SIOW's Download Software to transmit new program\r\n\r\n");
      
      ext_pic_start_bootloader();
      
      load_program();
      
      ext_pic_start_application();
      
      printf("Bootloader finished.\r\n\r\n");
   }
}

