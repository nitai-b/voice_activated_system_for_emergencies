///////////////////////////////////////////////////////////////////////////
////                      ex_lowpass_filter_adc2.c                     ////
////                                                                   ////
//// This program is an example of using the Analog-to-Digital         ////
//// Converter with Computation (ADC2) peripheral to implement a       ////
//// low-pass filter on an analog input pin.                           ////
////                                                                   ////
//// It uses a RS232 link and the SIOW.exe program's Data Graphing     ////
//// feature to graph the filtered readings.                           ////
////                                                                   ////
//// This example will work with the PCM compiler.  Change the device, ////
//// clock and RS232 pins for your hardware if needed.  Additionally   ////
//// following defines can be changes to set cut-off frequency of the  ////
//// filter, the number of sample to perform before staring the filter ////
//// calculation, and the Analog input pin to read:                    ////
////                                                                   ////
////     FILTER_CUT_OFF_FREQ                                           ////
////     ADC_READINGS                                                  ////
////     ADC_CHANNEL                                                   ////
////     ADC_PIN                                                       ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2016 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS    ////
//// C compiler.  This source code may only be distributed to other    ////
//// licensed users of the CCS C compiler.  No other use,              ////
//// reproduction or distribution is permitted without written         ////
//// permission.  Derivative programs created using this software      ////
//// in object code form are not restricted in any way.                ////
///////////////////////////////////////////////////////////////////////////

#include <16F18875.h>
#fuses NOWDT
#device ADC=10
#use delay(internal=8MHz)

#pin_select U1TX=PIN_C6
#pin_select U1RX=PIN_C7
#use rs232(UART1,baud=9600)

#define GRAPH_TITLE   "Low-Pass Filter Graph"  
#define GRAPH_Y_TITLE "Filtered ADC Reading"              
#define GRAPH_SERIES1_TITLE "ADC Input"
#define GRAPH_X_MIN  "0"              
#define GRAPH_Y_MIN  "0"              
#define GRAPH_X_MAX  "100"            
#define GRAPH_Y_MAX  "1023"            
#define GRAPH_X_SCALE  "1"            
#define GRAPH_Y_SCALE  "1"            
#define GRAPH_X_OFFSET  "0"           
#define GRAPH_Y_OFFSET  "0"           
#define GRAPH_X_DECIMAL_POINTS "1"    
#define GRAPH_Y_DECIMAL_POINTS "0"    

#include <graph_siow.h>

//The following define sets the cut-off frequency of the low-pass filter, can be
//be set from 1 to 5.  Lower value higher cut-off frequency, higher value lower
//cut-off frequency, see TABLE 23-4 in device's datasheet for cut-off frequency.
#define FILTER_CUT_OFF_FREQ   3

//The following define sets the number of readings the ADC peripheral will 
//perform before doing the filter calculation.
#define ADC_READINGS          8

//The following defines set the ADC channel and PIN that will be used sampled.
#define ADC_CHANNEL           0
#define ADC_PIN               sAN0

void main()
{
   unsigned int16 FilteredResult;
   
   delay_ms(100);
   
   printf("\r\nLow-Pass Filter Example using ADC with Computation Module on a %s", getenv("DEVICE"));
   
   delay_ms(1000);
   
   init_graph();
   
   setup_timer_2(T2_CLK_INTERNAL | T2_DIV_BY_128, 155, 10); //~10ms period, 100ms interrupt
   
   setup_adc_ports(ADC_PIN, VSS_VDD);
   setup_adc(ADC_LOW_PASS_FILTER_MODE | ADC_CLOCK_INTERNAL | ADC_TAD_MUL_255 | ADC_THRESHOLD_INT_END_OF_CALCULATION, FILTER_CUT_OFF_FREQ, ADC_READINGS);
   set_adc_channel(ADC_CHANNEL);  
   set_adc_trigger(ADC_TRIGGER_TIMER2);
   
   while(TRUE)
   {
      if(interrupt_active(INT_AD_THRESHOLD))
      {
         FilteredResult= adc_read(ADC_FILTER);
         
         graph_point(FilteredResult);

         clear_interrupt(INT_AD_THRESHOLD);
      }
   }
}
