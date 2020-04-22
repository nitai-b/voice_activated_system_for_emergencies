//Hare Krsna Hare Krsna Krsna Krsna Hare Hare Hare Rama Hare Rama Rama Rama Hare Hare//
///////////////////////////I N T R O D U C T I O N///////////////////////////
/*		
 *		Author:		Mr. Nitai Barran
 *		Supervisor: Prof. Stephan J.G. Gift
 *		System:		Voice Activated System for Emergencies
 */
//<<end of INTRODUCTION
#include <18f4620.h>
#include "protocol.h"
#fuses HS,NOLVP,NOWDT,PUT,NOPROTECT
#use delay(clock=4000000)
#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,stream=EVR,bits=8)
#use rs232(baud=9600,parity=N,xmit=PIN_D2,rcv=PIN_D3,stream=GSM,bits=8)

/*						DEBUGGING FUNCTIONS
	for debugging, output information to another serial port using the functions
	fputc(a variable,the stream);
	a variable = fgetc(the stream);
	fprintf(stream,"some string");

*/


///////////////////////////E N U M E R A T I O N S///////////////////////////
enum ErrorCode{//some of the possible error codes returned by the EasyVR Module
    //-- 0x: Data collection errors (patgen, wordspot, t2si)
    ERR_DATACOL_TOO_LONG        = 0x02, /**< too long (memory overflow) */
    ERR_DATACOL_TOO_NOISY       = 0x03, /**< too noisy */
    ERR_DATACOL_TOO_SOFT        = 0x04, /**< spoke too soft */
    ERR_DATACOL_TOO_LOUD        = 0x05, /**< spoke too loud */
    ERR_DATACOL_TOO_SOON        = 0x06, /**< spoke too soon */
    ERR_DATACOL_TOO_CHOPPY      = 0x07, /**< too many segments/too complex */
    ERR_DATACOL_BAD_WEIGHTS     = 0x08, /**< invalid SI weights */
    ERR_DATACOL_BAD_SETUP       = 0x09, /**< invalid setup */

    //-- 1x: Recognition errors (si, sd, sv, train, t2si)
    ERR_RECOG_FAIL              = 0x11, /**< recognition failed */
    ERR_RECOG_LOW_CONF          = 0x12, /**< recognition result doubtful */
    ERR_RECOG_MID_CONF          = 0x13, /**< recognition result maybe */
    ERR_RECOG_BAD_TEMPLATE      = 0x14, /**< invalid SD/SV template */
    ERR_RECOG_BAD_WEIGHTS       = 0x15, /**< invalid SI weights */
    ERR_RECOG_DURATION          = 0x17, /**< incompatible pattern durations */

    //-- 2x: T2si errors (t2si)
    ERR_T2SI_EXCESS_STATES      = 0x21, /**< state structure is too big */
    ERR_T2SI_BAD_VERSION        = 0x22, /**< RSC code version/Grammar ROM dont match */
    ERR_T2SI_OUT_OF_RAM         = 0x23, /**< reached limit of available RAM */
    ERR_T2SI_UNEXPECTED         = 0x24, /**< an unexpected error occurred */
    ERR_T2SI_OVERFLOW           = 0x25, /**< ran out of time to process */
    ERR_T2SI_PARAMETER          = 0x26, /**< bad macro or grammar parameter */

    ERR_T2SI_NN_TOO_BIG         = 0x29, /**< layer size out of limits */
    ERR_T2SI_NN_BAD_VERSION     = 0x2A, /**< net structure incompatibility */
    ERR_T2SI_NN_NOT_READY       = 0x2B, /**< initialization not complete */
    ERR_T2SI_NN_BAD_LAYERS      = 0x2C, /**< not correct number of layers */

    ERR_T2SI_TRIG_OOV           = 0x2D, /**< trigger recognized Out Of Vocabulary */
    ERR_T2SI_TOO_SHORT          = 0x2F, /**< utterance was too short */

    //-- 3x: Record and Play errors (standard RP and messaging)
    ERR_RP_BAD_LEVEL            = 0x31, /**<  play - illegal compression level */
    ERR_RP_NO_MSG               = 0x38, /**<  play, erase, copy - msg doesn't exist */
    ERR_RP_MSG_EXISTS           = 0x39, /**<  rec, copy - msg already exists */

    //-- 4x: Synthesis errors (talk, sxtalk)
    ERR_SYNTH_BAD_VERSION       = 0x4A, /**< bad release number in speech file */
    ERR_SYNTH_ID_NOT_SET        = 0x4B, /**< (obsolete) bad sentence structure */
    ERR_SYNTH_TOO_MANY_TABLES   = 0x4C, /**< (obsolete) too many talk tables */
    ERR_SYNTH_BAD_SEN           = 0x4D, /**< (obsolete) bad sentence number */
    ERR_SYNTH_BAD_MSG           = 0x4E, /**< bad message data or SX technology files missing */

    //-- 8x: Custom errors
    ERR_CUSTOM_NOTA             = 0x80, /**< none of the above (out of grammar) */
    ERR_CUSTOM_INVALID          = 0x81, /**< invalid data (for memory check) */

    //-- Cx: Internal errors (all)
    ERR_SW_STACK_OVERFLOW       = 0xC0, /**< no room left in software stack */
    ERR_INTERNAL_T2SI_BAD_SETUP = 0xCC, /**< T2SI test mode error */
};
enum playback{//constants for playback
	p_ZERO, p_ONE, p_TWO, p_THREE, p_FOUR, p_FIVE, p_SIX, p_SEVEN, p_EIGHT, p_NINE, BEEP, SAY_COMMAND, 
};
enum wordSet1{//constants for wordset1
	ACTION, MOVE, TURN, RUN, LOOK, ATTACK, STOP, HELLO,
};
enum wordset2{//constants for wordset2
	LEFT, RIGHT, UP, DOWN, FORWARD, BACKWARD,
};
enum wordset3{//constants for wordset3
	ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN
};
enum group1{//constants for action words
	
};
enum group2{//constants for target of action words
	
};
//<<end of ENUMERATIONS


///////////////////////////C O R E  F U N C T I O N  D E F I N I T I O N S///////////////////////////
void blinkD1(int a){//LED indicator
	int i = 0;
	for (i; i < a; i++ ){
		output_high(PIN_D1);
		delay_ms(50);
		output_low(PIN_D1);
		delay_ms(100);
	}
}
void blinkD0(int a){//LED indicator
	int i = 0;	
	for (i; i < a; i++ ){
		output_high(PIN_D0);
		delay_ms(50);
		output_low(PIN_D0);
		delay_ms(100);
	}
}
void halt(){//stop current action
	char rec_halt = null;
	while (rec_halt != STS_SUCCESS){
		fputc(CMD_BREAK,EVR);
		delay_ms(25);
		if (kbhit()){
			rec_halt = fgetc(EVR);
		}
	}
	rec_halt = null;
}
void sleep_wake(){//put device to sleep and wake on whistle or received a character
	char rec_sleep = null;
	while (rec_sleep != STS_SUCCESS){
		fputc(CMD_SLEEP,EVR);
		delay_ms(25);
		fputc(ARG_ONE,EVR);
		delay_ms(25);
		if (kbhit()){
			rec_sleep = fgetc(EVR);
		}
	}
	rec_sleep = null;
}
void setLang(char i){//set the language based on the argument passed
	char rec_setlang = null;
	while (rec_setlang != STS_SUCCESS){
		fputc(CMD_LANGUAGE,EVR);
		delay_ms(25);
		fputc(i,EVR);
		delay_ms(25);
		if (kbhit()){
			rec_setlang = fgetc(EVR);
		}
	}
}
void setTimeout(char i){//set a timeout based on the argument passed
	char rec_time = null;
	while (rec_time != STS_SUCCESS){
		fputc(CMD_TIMEOUT,EVR);
		delay_ms(25);
		fputc(i,EVR);
		delay_ms(25);
		if (kbhit()){
			rec_time = fgetc(EVR);
		}
	}
}
void listenWS1(){//listens to wordset1 and determines which word was spoken
	char rec_WS1 = null;
	char cmd_WS1 = null;
	fputc(CMD_RECOG_SI,EVR);
	delay_ms(25);
	fputc(ARG_ONE,EVR);
	delay_ms(25);
	while (1){
		if (kbhit()){
			rec_WS1 = fgetc(EVR);
		}
		if (rec_WS1 == STS_SIMILAR){
			fputc(ARG_ACK,EVR);
			delay_ms(25);
			rec_WS1 = null;
			rec_WS1 = fgetc(EVR);
			cmd_WS1 = rec_WS1 - ARG_ZERO;
				switch (cmd_WS1){
					case ACTION:
					break;
					case MOVE:
					break;
					case TURN:
					break;
					case RUN:
					break;
					case LOOK:
					break;
					case ATTACK:
					break;
					case STOP:
					break;
					case HELLO:
					break;
					default:		rec_WS1 = STS_SIMILAR;
					break;
				}
		break;
		}else
		if (rec_WS1 == STS_TIMEOUT){
		break;
		}else
		if (rec_WS1 == STS_ERROR){
		break;
		}
	}
}
void listenWS2(){//listens to wordset2 and determines which word was spoken
	char rec_WS2 = null;
	char cmd_WS2 = null;
	fputc(CMD_RECOG_SI,EVR);
	delay_ms(25);
	fputc(ARG_TWO,EVR);
	delay_ms(25);
	while (1){
		if (kbhit()){
			rec_WS2 = fgetc(EVR);
		}
		if (rec_WS2 == STS_SIMILAR){
			fputc(ARG_ACK,EVR);
			delay_ms(25);
			rec_WS2 = null;
			rec_WS2 = fgetc(EVR);
			cmd_WS2 = rec_WS2 - ARG_ZERO;
				switch (cmd_WS2){
					case LEFT:
					break;
					case RIGHT:
					break;
					case UP:
					break;
					case DOWN:
					break;
					case FORWARD:
					break;
					case BACKWARD:
					break;
					default:		rec_WS2 = STS_SIMILAR;
					break;
				}
		break;
		}else
		if (rec_WS2 == STS_TIMEOUT){
		break;
		}else
		if (rec_WS2 == STS_ERROR){
		break;
		}
	}
}
void listenWS3(){//listens to wordset3 and determines which word was spoken
	char rec_WS3 = null;
	char cmd_WS3 = null;
	fputc(CMD_RECOG_SI,EVR);
	delay_ms(25);
	fputc(ARG_THREE,EVR);
	delay_ms(25);
	while (1){
		if (kbhit()){
			rec_WS3 = fgetc(EVR);
		}
		if (rec_WS3 == STS_SIMILAR){
			fputc(ARG_ACK,EVR);
			delay_ms(25);
			rec_WS3 = null;
			rec_WS3 = fgetc(EVR);
			cmd_WS3 = rec_WS3 - ARG_ZERO;
				switch (cmd_WS3){
					case ZERO:
					break;
					case ONE:
					break;
					case TWO:
					break;
					case THREE:
					break;
					case FOUR:
					break;
					case FIVE:
					break;
					case SIX:
					break;
					case SEVEN:
					break;
					case EIGHT:
					break;
					case NINE:
					break;
					case TEN:
					break;
					default:		rec_WS3 = STS_SIMILAR;
					break;
				}
		break;
		}else
		if (rec_WS3 == STS_TIMEOUT){
		break;
		}else
		if (rec_WS3 == STS_ERROR){
		break;
		}
	}
}
void playback(int playback){//plays back custom sound
	char rec_play = null;
	switch (playback){
		case BEEP:
			while (rec_play != STS_SUCCESS){
				fputc(CMD_PLAY_SX,EVR);
				delay_ms(25);
				fputc(ARG_ZERO,EVR);
				delay_ms(25);
				fputc(ARG_ZERO,EVR);
				delay_ms(25);
				fputc(ARG_FIFTEEN,EVR);
				delay_ms(25);
				if (kbhit()){
					rec_play = fgetc(EVR);
				}
				/*if (rec_play == STS_SUCCESS){
					break;
				}*/
			}
		break;
		case SAY_COMMAND:
		break;
		default:
		break;
	}
}
//<<end of FUNCTION DEFINITIONS



///////////////////////////M A I N  F U N C T I O N///////////////////////////
void main(){
	//char k = null;
	
	halt();					//stop or wake up device
	setLang(ARG_ZERO);		//set the language to english
	setTimeout(ARG_FIVE);	//set the timeout for 5 seconds
	playback(BEEP);
	
	
	
	while(1){blinkD0(1);}

	/*while(1){
		k=fgetc(GSM);
		delay_ms(25);
		fputc(k,EVR);
		delay_ms(25);
		if (kbhit()){
			receive = fgetc(EVR);
			fputc(receive,GSM);
		}
	}*/
}
//<<end of MAIN FUNCTION
//Hare Krsna Hare Krsna Krsna Krsna Hare Hare Hare Rama Hare Rama Rama Rama Hare Hare//