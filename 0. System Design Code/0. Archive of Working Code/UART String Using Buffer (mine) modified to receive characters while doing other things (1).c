// Author  : Nitai Barran
// School  : The University of the West Indies
// Program : Electrical and Computer Engineering
// Project : Voice Activated System for Emergencies

// This is the setup of functions I needed to use for my project.
// Some of it is not necessary for this testing.
// Some of it can be changed to suit your needs.
#include <18f4620.h>
#include <string.h>
#include <stdlib.h>
#include <protocol.h>
#fuses HS,NOLVP,NOWDT,PUT,NOPROTECT
#use delay(clock=10000000)
#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,stream=GSM,bits=8)		//HW serial pins EasyVR	||	EVR
#use rs232(baud=9600,parity=N,xmit=PIN_D0,rcv=PIN_D1,stream=EVR,bits=8)		//SW serial pins Fona	||	GSM
#use rs232(baud=9600,parity=N,xmit=PIN_A4,rcv=PIN_A3,stream=PC,bits=8)		//SW serial pins Debug	||	DB
#include <input.c> //this file must be ONLY included after #use rs232

// This is some code to blink an LED on Pin A0.
// This was done to emulate things happening in the foreground.
void blinkA0(int _a){			//LED indicator
	int _i = 0;
	for (_i; _i < _a; _i++ ){
		output_high(PIN_A0);
		delay_ms(50);
		output_low(PIN_A0);
		delay_ms(100);
	}
}

//                       =================                       //
//=======================I N T E R U P T S=======================//
//                       =================                       //
#DEVICE HIGH_INTS=TRUE
// This was some code from before that I didn't need again.
// You can uncomment it if you want.
////External Interrupt
// #INT_EXT
// void EXT_isr(void){
	// clear_interrupt(INT_RB);	//Clear RB port change interrupt flag bit
	// call_flag = TRUE;
// }

#define BUFFER_SIZE 32 
volatile unsigned char buffer[BUFFER_SIZE]; 
volatile unsigned int rx_buff_index = 0;
volatile int got_string_flag = 0;
// Interrupt service routine for receiving characters
// This ISR is followed from the book:
// "Microprocessors From Assembly Language to C Using The PIC18Fxx2"
// There is some personal modificaton from the algorithim in the book
// The reason being: I needed to get it to do other things while receiving data
#INT_RDA HIGH
void serial_rx_isr(void){
	if (interrupt_enabled(INT_RDA) == 1){
		buffer[rx_buff_index] = getc();
		if (buffer[rx_buff_index] == '\r'){
			got_string_flag = 1;
			rx_buff_index = 0;
			disable_interrupts(INT_RDA);
		}
		++rx_buff_index;
	}
}


//this is a modified version of the function from INPUT.c
void bgets(char* s, unsigned int8 max){
	unsigned char _c;
	unsigned int8 len;
	unsigned int8 rd_buff_index;

	--max;
	len = 0;
	rd_buff_index = 0;
	do{
		_c = buffer[rd_buff_index++];
		if (len <= max){
			s[len++] = _c;
		}
		
	}while(_c != '\r');
	s[len] = 0;
	len = 0;
	rx_buff_index = 0;
	rd_buff_index = 0;
}




#define STRING_SIZE 16
//                   ========================                    //
//===================M A I N  F U N C T I O N====================//
//                   ========================                    //
void main(){
	char input_str[STRING_SIZE];
	
	enable_interrupts(INT_RDA);
	enable_interrupts(GLOBAL);
	
	printf("enter a string:\r\n");
////Program Loop
	while (TRUE){
		if (got_string_flag == 1){
		//disable uart receive interrupt if it is active
			if (interrupt_active(INT_RDA)) disable_interrupts(INT_RDA);
		//read the string received by the software buffer into input_str
			bgets(input_str,STRING_SIZE-1);
		//then do something based on what is read
			//in this case I just want to output it to the screen for testing
			fprintf(GSM,input_str);
			fputs("\r\n",GSM);
		//reset flag and re-enable interrupt
			got_string_flag = 0;
			enable_interrupts(INT_RDA);
		}
		
		
		blinkA0(1);
	}
}