/////////////////////////////////////////////////////////////////////////
////                                                                 ////
////                        ex_usb_multihid.c                        ////
////                                                                 ////
//// An example of creating a mouse, keyboard and generic/custom HID ////
//// combo USB device, implemented by using multiple interfaces.     ////
//// In essence, this is combining ex_usb_mouse.c,                   ////
//// ex_usb_keyboard.c and ex_usb_hid.c into one device that does    ////
//// all simultaneously.  One interface/endpoint is used for the     ////
//// mouse, another interface/endpoint for the keyboard, another     ////
//// interface/endpoint is used for the generic/custom HID device.   ////
//// Uses your operating system's HID drivers, which on most systems ////
//// should already be installed.  See ex_usb_multihid2.c which      ////
//// implements a combo device using multiple HID reports, but only  ////
//// uses one interface/endpoint.                                    ////
////                                                                 ////
//// Once enumerated, the PIC will move the mouse cursor around in   ////
//// a circle if the button is held down.  Also, whenever the        ////
//// button is held down the PIC will send an 'a' as a keyboard      ////
//// press.  (open a text editor to see the 'a' being sent)          ////
////                                                                 ////
//// To communicate with the vendor specific portion of this demo,   ////
//// use the CCS provided hiddemo.exe PC program.  When using this   ////
//// program, open the detected device with a Windows Device         ////
//// Instance ID that contains &mi_02 in the string, as the &mi_xx   ////
//// refers to the interface number of the device.  Since the        ////
//// the vendor specific portion of this demo uses the third         ////
//// interface in the descriptor, it shows up in Windows as          ////
//// &mi_02.                                                         ////
////                                                                 ////
//// This file is part of CCS's PIC USB driver code.  See USB.H      ////
//// for more documentation and a list of examples.                  ////
////                                                                 ////
/////////////////////////////////////////////////////////////////////////
////                                                                 ////
//// VERSION HISTORY                                                 ////
////                                                                 ////
//// Dec 18, 2014:                                                   ////
////  Created.                                                       ////
////                                                                 ////
/////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2014 Custom Computer Services         ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
/////////////////////////////////////////////////////////////////////////

#include <ex_usb_common.h>

/////////////////////////////////////////////////////////////////////////////
//
// Include the CCS USB Libraries.  See the comments at the top of these
// files for more information
//
/////////////////////////////////////////////////////////////////////////////
#if __USB_PIC_PERIF__
 #if defined(__PCM__)
  #include <pic16f_usb.h>   //Microchip PIC16Fxxxx hardware layer for usb.c
 #elif defined(__PCH__)
  #include <pic18_usb.h>   //Microchip PIC18Fxx5x hardware layer for usb.c
 #elif defined(__PCD__)
  #include <pic24_usb.h>   //Microchip PIC18Fxx5x hardware layer for usb.c
 #endif
#else
 #include <usbn960x.h>   //National 960x hardware layer for usb.c
#endif
#include <usb_desc_multihid.h>    //USB Configuration and Device descriptors for this UBS device
#include <usb.c>        //handles usb setup tokens and get descriptor reports

/////////////////////////////////////////////////////////////////////////////
//
// usb_debug_task()
//
// When called periodically, displays debugging information over serial
// to display enumeration and connection states.  Also lights LED2 and LED3
// based upon enumeration and connection status.
//
/////////////////////////////////////////////////////////////////////////////
void usb_debug_task(void)
{
   static int8 last_connected;
   static int8 last_enumerated;
   int8 new_connected;
   int8 new_enumerated;

   new_connected=usb_attached();
   new_enumerated=usb_enumerated();

   if (new_enumerated)
   {
      LED_ON(LED1);
   }
   else
   {
      LED_OFF(LED1);
   }
      
   if (new_connected && !last_connected)
      uart_printf("\r\n\nUSB connected, waiting for enumaration...");
   if (!new_connected && last_connected)
      uart_printf("\r\n\nUSB disconnected, waiting for connection...");
   if (new_enumerated && !last_enumerated)
      uart_printf("\r\n\nUSB enumerated by PC/HOST");
   if (!new_enumerated && last_enumerated)
      uart_printf("\r\n\nUSB unenumerated by PC/HOST, waiting for enumeration...");

   last_connected=new_connected;
   last_enumerated=new_enumerated;
}

void display_array(unsigned int8 *p, unsigned int8 n)
{
   while(n--)
   {
      uart_printf(uart_putc, " 0x%X", *p++);
   }
}

/////////////////////////////////////////////////////////////////////////////
//
// usb_mouse_task()
//
// Sends a packet of data containing mouse information.  The protocol was
// defined in the HID report descriptor (see usb_desc_kbmouse.h), and is:
//   out_data[0]=button state
//   out_data[1]=x delta
//   out_data[2]=y delta
//   out_data[3]=wheel delta
//
/////////////////////////////////////////////////////////////////////////////
void usb_mouse_task(void) 
{
   #define MOUSE_SEQUENCE_STEPS  16
   const char mouse_seq[MOUSE_SEQUENCE_STEPS]=
      {0, 1, 3, 4, 4, 4, 3, 1, 0, -1, -3, -4, -4, -4, -3, -1};

   int8 out_data[4];
   static int8 x_seq=0;  static int8 y_seq=MOUSE_SEQUENCE_STEPS/4;
   static int8 skip=0;

   if (BUTTON_PRESSED()) 
   {
      out_data[1] = mouse_seq[x_seq];
      out_data[2] = mouse_seq[y_seq];
   }
   else {
      out_data[1] = 0;
      out_data[2] = 0;
   }
   out_data[0] = 0;
   out_data[3] = 0;

   if (usb_put_packet(1,out_data,4,USB_DTS_TOGGLE)) 
   {
      skip++;
      if (skip>10) 
      {
         if (++x_seq>=MOUSE_SEQUENCE_STEPS) {x_seq=0;}
         if (++y_seq>=MOUSE_SEQUENCE_STEPS) {y_seq=0;}
         skip=0;
      }
   }
}

/////////////////////////////////////////////////////////////////////////////
//
// usb_keyboard_task()
//
// Sends a packet of keyboard data.  The protocol was specified in the HID
// report descriptor (see usb_desc_kbmouse.h), and is:
//     tx_msg[0] = modifier (an 8bit bitmap of shift, tab, alt keypress)
//     tx_msg[1] = const 0
//     tx_msg[2:6] = an array of held down keys.  a=4, b=5, etc.
//                   if msg[2:7]={0} then no keys are held down
//
//     rx_msg[0] = 5bit bitmap of led status
//
/////////////////////////////////////////////////////////////////////////////
void usb_keyboard_task(void) 
{
   static int8 tx_msg[7]={0,0,0,0,0,0,0};
   static int8 leds;

   if (BUTTON_PRESSED())
      tx_msg[2]=4;
   else
      tx_msg[2]=0;

   usb_put_packet(2,tx_msg,sizeof(tx_msg),USB_DTS_TOGGLE);

   //receive NUM LOCK, CAPS LOCK, etc LED status from PC.
   //we won't do anything with it.
   if (usb_kbhit(2)) 
   {
      usb_get_packet(2, &leds, 1);
   }
}


/////////////////////////////////////////////////////////////////////////////
//
// usb_vendor_task()
//
// Sends a vendor speicific HID packet, which is sent/received by the
// usbhid.exe PC software.
//
//     tx_msg[0] = HID report id (3)
//     tx_msg[1] = ADC value
//     tx_msg[2] = Button status
//
//     rx_msg[0] = HID report id (3)
//     rx_msg[1] = LED #1 on/off control
//       rx_msg[2] = LED #2 on/off control
//
/////////////////////////////////////////////////////////////////////////////
void usb_vendor_task(void)
{
   unsigned int8 out_data[USB_CONFIG_HID_TX_SIZE];
   unsigned int8 in_data[USB_CONFIG_HID_RX_SIZE];
   static unsigned int send_timer;
   
   if (send_timer-- == 0)
   {
      send_timer = 50;
      out_data[0] = read_adc();
      out_data[1] = BUTTON_PRESSED();

      if (usb_put_packet(3, out_data, USB_CONFIG_HID_TX_SIZE, USB_DTS_TOGGLE))
      {
         uart_printf(uart_putc, "\r\n<-- Sending %u bytes:", USB_CONFIG_HID_TX_SIZE);
         display_array(out_data, USB_CONFIG_HID_TX_SIZE);
      }
   }
   
   if (usb_kbhit(3))
   {
      usb_get_packet(3, in_data, USB_CONFIG_HID_RX_SIZE);
      
      uart_printf(uart_putc, "\r\n--> Received %u bytes:", USB_CONFIG_HID_RX_SIZE);
      display_array(in_data, USB_CONFIG_HID_RX_SIZE);
      
     #if defined(LEDS_GREEN)
      if (in_data[0] && in_data[1])
      {
         LEDS_RED();
      }
      else if (in_data[0] || in_data[1])
      {
         LEDS_GREEN();
      }
      else
      {
         LEDS_OFF();
      }
     #else
      if (in_data[0]) {LED_ON(LED2);} else {LED_OFF(LED2);}
      if (in_data[1]) {LED_ON(LED3);} else {LED_OFF(LED3);}
     #endif
   }
}

void main(void) 
{
   HW_INIT();

   setup_adc_ports(HW_ADC_PORTS);
   setup_adc(HW_ADC_CONFIG);
   set_adc_channel(HW_ADC_CHANNEL);
   
   LEDS_OFF();

   uart_printf("\r\n\nCCS HID Mouse/Keyboard/Vendor Multiple Interface Demo");
   uart_printf("\r\nPress button to make mouse move in a circle and ");
   uart_printf("send an A over the keyboard");
   
  #ifdef __PCH__
   uart_printf("\r\nPCH: v");
   uart_printf(__PCH__);
  #elif defined(__PCD__)
   uart_printf("\r\nPCD: v");
   uart_printf(__PCD__);
  #else
   uart_printf("\r\nPCM: v");
   uart_printf(__PCM__);
  #endif

   usb_init_cs();

  #if !(__USB_PIC_PERIF__)
   uart_printf("\r\nUSBN: 0x%X", usbn_get_version());
  #endif
   uart_printf("\r\n");

   while (TRUE)
   {
      usb_task();
      usb_debug_task();

      if (usb_enumerated())
      {
         usb_mouse_task();
         
         usb_keyboard_task();
         
         usb_vendor_task();
         
         delay_ms(10);
      }
   }
}

