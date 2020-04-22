//=============================================//
//==========C O N F I G U R A T I O N==========//
//=============================================//
/*		Author:		Mr. Nitai Barran
 *		Supervisor: Prof. Stephan J.G. Gift
 *		System:		Voice Activated System for Emergencies
 */
//<<end of INTRODUCTION
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

// char AT[] =			"AT\r\n";
// char save[] =		"AT&W\r\n";
// char noecho[] =		"ATE0\r\n";
// char mode_text[] =	"AT+CMGF=1\r\n";   // to set text mode
// char endCall[] =	"ATH\r\n";
// char ansCall[] =	"ATA\r\n";
char ring[] = 		"\r\nRING\r\n";


//=================================================//
//================F U N C T I O N S================//
//=================================================//
void blinkA0(int _a){
	int _i = 0;
	do{
		output_toggle(PIN_A0);
		delay_ms(200);
		_i++;
	}while (_i < _a);
}
void blinkA1(int _a){
	int _i = 0;
	do{
		output_toggle(PIN_A1);
		delay_ms(200);
		_i++;
	}while (_i < _a);
}
void blinkD2(int _a){
	int _i = 0;
	do{
		output_toggle(PIN_D2);
		delay_ms(200);
		_i++;
	}while (_i < _a);
}


//=================================================//
//================I N T E R U P T S================//
//=================================================//
#define RX_SIZE 20
volatile unsigned int call_flag = FALSE;
volatile char rec_string[16];
#INT_EXT
void detect_call(void){
	//for this I would use it for detecting a call
	// fgets(rec_string,GSM);
	// if(strncmp(rec_string,ring,strlen(ring))==0){
		// call_flag = TRUE;		//you have received a string
	// }
	output_toggle(PIN_A0);

}

#INT_EXT1
void button1(void){
	output_toggle(PIN_A1);
	delay_ms(50);
}

#INT_EXT2
void button2(void){
	output_toggle(PIN_D2);
	delay_ms(50);
}



//=================================================//
//=====================M A I N=====================//
//=================================================//
void main (){

	SET_TRIS_B(0x0F);				// set lower half of port B as input
	ext_int_edge(H_TO_L);			//
	ext_int_edge(1,H_TO_L);			// set up the trigger level for interrupts
	ext_int_edge(2,H_TO_L);			//
	
	clear_interrupt(INT_EXT);		//
	clear_interrupt(INT_EXT1);		// clear interrupts on start up...so no more problems would happen
	clear_interrupt(INT_EXT2);		//
	
	enable_interrupts(INT_EXT);		//
	enable_interrupts(INT_EXT1);	// enable all the external interrupts
	enable_interrupts(INT_EXT2);	//
	
	enable_interrupts(GLOBAL);		// enable golbal interrutps
	
	while(TRUE){
		blinkA0(1);
		// blinkA1(1);
		// blinkD2(1);
	}
}