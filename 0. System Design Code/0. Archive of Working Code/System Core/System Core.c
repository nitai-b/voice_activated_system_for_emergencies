//////////////////////////I N T R O D U C T I O N////////////////////////////////////////////////////////////////////
/*        Date: 19/01/2017
 *        This is the code that describes the main system
 */
#include "protocol.h"                                                           //the headder file with all the ascii commands
char rec = null;                                                                //golbal variable for uart storage
//<<end of INTRODUCTION


//////////////////F U N C T I O N  D E F I N I T I O N S/////////////////////////////////////////////////////////////
void blinkD1(int a){
	int i = 0;
	for (i; i < a; i++ ){
		LATD1_bit = 1;
		Delay_ms(50);
		LATD1_bit = 0;
		Delay_ms(100);
	}
}
void blinkD0(int a){
	int i = 0;
	for (i; i < a; i++ ){
		LATD0_bit = 1;
		Delay_ms(50);
		LATD0_bit = 0;
		Delay_ms(100);
	}
}
//<<end of FUNCTION DEFINITIONS


///////////////////////////M A I N  F U N C T I O N//////////////////////////////////////////////////////////////////
void main(){
	char error;
	TRISD0_bit = 0;
	TRISD1_bit = 0;
	error = Soft_UART_Init(&PORTC, 7, 6, 14400, 0);
	UART1_Init(9600);
	Delay_ms(100);

	//wake up procedure
	UART1_Write(CMD_BREAK);
	Delay_ms(25);
	while(rec != STS_SUCCESS){
		if (UART1_Data_Ready()){
			rec = UART1_Read();
			UART1_Write(rec);
		}
	}rec = null;

	//set the language to English
	UART1_Write(CMD_LANGUAGE);
	Delay_ms(25);
	UART1_Write(ARG_ZERO);
	Delay_ms(25);
	while(rec != STS_SUCCESS){
		if (UART1_Data_Ready()){
		rec = UART1_Read();
		UART1_Write(rec);
		}
	}rec = null;

	//set the timeout for 5 seconds
	UART1_Write(CMD_TIMEOUT);
	Delay_ms(25);
	UART1_Write(ARG_FIVE);
	Delay_ms(25);
	while(rec != STS_SUCCESS){
		if (UART1_Data_Ready()){
			rec = UART1_Read();
			UART1_Write(rec);
		}
	}rec = null;

	//Listening to SI commands
	UART1_Write(CMD_RECOG_SI);
	Delay_ms(25);
	UART1_Write(ARG_ONE);
	Delay_ms(25);
	while(1){
		if (UART1_Data_Ready()){
			rec = UART1_Read();                                                  // read the received data,
			//UART1_Write(uart_rd);                                              // and send data via UART {for testing}
			if (rec == STS_SIMILAR){
				blinkD1(1);                                                       //if SI is similar, then blink LED once
			} else
			if (rec == STS_TIMEOUT){
				blinkD1(1);blinkD0(1);                                            //if timeout, then blink both LEDs
			} else
			if (rec == STS_ERROR){
				blinkD1(2);blinkD0(2);                                            //if error, then blink both LEDs twice
			}else{
				break;
			}
		}
	}rec = null;

}
//<<end of MAIN FUNCTION