//Hare Krsna Hare Krsna Krsna Krsna Hare Hare Hare Rama Hare Rama Rama Rama Hare Hare//
//====================I N T R O D U C T I O N====================//
/*		
 *		Author:		Mr. Nitai Barran
 *		Supervisor: Prof. Stephan J.G. Gift
 *		System:		Voice Activated System for Emergencies
 */
//<<end of INTRODUCTION
#include <18f4620.h>
#include <string.h>
#include "protocol.h"
#fuses HS,NOLVP,NOWDT,PUT,NOPROTECT
#use delay(clock=10000000)
#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,stream=EVR,bits=8)		//HW serial pins EasyVR	||	EVR
#use rs232(baud=115200,parity=N,xmit=PIN_D2,rcv=PIN_D3,stream=GSM,bits=8)	//SW serial pins Fona	||	GSM
#use rs232(baud=9600,parity=N,xmit=PIN_A4,rcv=PIN_A3,stream=DB,bits=8)		//SW serial pins Debug	||	DB

/*						DEBUGGING FUNCTIONS
	for debugging, output information to another serial port using the functions
	fputc(a variable,the stream);
	a variable = fgetc(the stream);
	fprintf(stream,"some string");
	
	////code for echoing characters////
	char k = null;
	while(1){
		k = fgetc(GSM);
		fputc(k,GSM);
		blinkD0(1);
	}
*/


//====================E N U M E R A T I O N S====================//
enum ErrorCode{					//some of the possible error codes returned by the EasyVR Module
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
//******Custom Sound Table******//
enum soundtable{				//constants for playback
	BEEP,
	SND_zero, SND_one, SND_two, SND_three, SND_four, SND_five, SND_six, SND_seven, SND_eight, SND_nine, 
	SND_please_say_a_command,
	SND_hello_please_say_a_command,
	SND_who_would_you_like_to_call,
	SND_calling,
	SND_doctor,
	SND_say_send_to_place_the_call,
	SND_say_reject_to_reject_the_call,
	SND_say_answer_to_answer_the_call,
	SND_say_end_to_cancel,
	SND_turning_on_LED,
	SND_turning_on_light_bulb,
	SND_turning_off_led,
	SND_turning_off_light_bulb,
	SND_you_have_an_incoming_call,
	SND_command_accepted,
	
};
//*Speaker Independent Commands*//
enum wordset{					//constants for list of SI Commands
	TRIGGER_0 = 0,
	WORDSET_1 = 1, WORDSET_2 = 2, WORDSET_3 = 3,
	GRAMMAR_4 = 4, GRAMMAR_5 = 5, GRAMMAR_6 = 6,
	GRAMMAR_7 = 7, GRAMMAR_8 = 8, GRAMMAR_9 = 9,
	GRAMMAR_10 = 10,
};
enum trigger{					//constants for trigger
	Robot,
};
enum wordSet1{					//constants for wordset1
	WS1_ACTION, WS1_MOVE, WS1_TURN, WS1_RUN, WS1_LOOK, WS1_ATTACK, WS1_STOP, WS1_HELLO,
};
enum wordset2{					//constants for wordset2
	WS2_LEFT, WS2_RIGHT, WS2_UP, WS2_DOWN, WS2_FORWARD, WS2_BACKWARD,
};
enum wordset3{					//constants for wordset3
	WS3_ZERO, WS3_ONE, WS3_TWO, WS3_THREE, WS3_FOUR, WS3_FIVE, WS3_SIX, WS3_SEVEN, WS3_EIGHT, WS3_NINE, WS3_TEN,
};
enum grammar4{					//constants for grammar4
	GR4_OK_EASYVEEAR
};
enum grammar5{					//constants for grammar5
	GR5_NEXT, GR5_PREVIOUS, GR5_STOP, GR5_PLAY, GR5_PAUSE, GR5_RECORD,
};
enum grammar6{					//constants for grammar6
	GR6_TURN_ON, GR6_SWITCH_OFF, GR6_INCREASE, GR6_DECREASE, GR6_OPEN, GR6_CLOSE, GR6_START, GR6_STOP,
};
enum grammar7{					//constants for grammar7
	GR7_DOOR, GR7_T_V, GR7_TEMPERATURE, GR7_WINDOW, GR7_MUSIC, GR7_LIGHT, GR7_TIMER, GR7_OVEN, GR7_WASHING_MACHINE, GR7_DISH_WASHER,
};
enum grammar8{					//constants for grammar8
	GR8_YES, GR8_NO,
};
enum grammar9{					//constants for grammar9
	GR9_HELLO_DEVICE,
};
enum grammar10{					//constants for grammar10
	GR10_PASSWORD_SETUP, GR10_PASSWORD_ACTIVATED, GR10_CURTAIN_UP, GR10_CURTAIN_DOWN, GR10_LIGHTS_ON, GR10_LIGHTS_OFF,
};
//**Speaker Dependent Commands**//
enum group{						//constants for list of SD Commands
	GROUP_1 = 1, GROUP_2 = 2,
};
enum group1{					//constants for action words
	G1_CALL, G1_LIGHTS, G1_LED,
};
enum group2{					//constants for target of action words
	G2_DOCTOR, G2_GUARDIAN, G2_MOM, G2_SON, G2_DAUGHTER, G2_DAD, G2_ON, G2_OFF, G2_OTHER,
};
//<<end of ENUMERATIONS


//====================F U N C T I O N  D E F N====================//
//**********System Fns**********//
void blinkD1(int a){			//LED indicator
	int i = 0;
	for (i; i < a; i++ ){
		output_high(PIN_D1);
		delay_ms(50);
		output_low(PIN_D1);
		delay_ms(100);
	}
}
void blinkD0(int a){			//LED indicator
	int i = 0;	
	for (i; i < a; i++ ){
		output_high(PIN_D0);
		delay_ms(50);
		output_low(PIN_D0);
		delay_ms(100);
	}
}
void blinkD4(int a){			//bulb indicator
	int i = 0;	
	for (i; i < a; i++ ){
		output_high(PIN_D4);
		delay_ms(50);
		output_low(PIN_D4);
		delay_ms(100);
	}
}
//***********Fona Fns***********//
void toggle_pwr(){
	output_low(PIN_C4);
	delay_ms(2000);
	output_high(PIN_C4);
}
//*********EasyVR Fns***********//
void halt(){					//stop current action
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
void sleep_wake(){				//put device to sleep and wake on whistle or received a character
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
void setLang(char whatLang){	//set the language based on the argument passed
	char rec_setlang = null;
	while (rec_setlang != STS_SUCCESS){
		fputc(CMD_LANGUAGE,EVR);
		delay_ms(25);
		fputc(whatLang,EVR);
		delay_ms(25);
		if (kbhit()){
			rec_setlang = fgetc(EVR);
		}
	}
}
void setTimeout(char period){	//set a period of the timeout based on the argument passed
	char rec_time = null;
	while (rec_time != STS_SUCCESS){
		fputc(CMD_TIMEOUT,EVR);
		delay_ms(25);
		fputc(period,EVR);
		delay_ms(25);
		if (kbhit()){
			rec_time = fgetc(EVR);
		}
	}
}
int getSIcommand(char SIgroup){	//gets the index of the SI commands or Custom Grammars
	char SIreceive = null;
	char SIcommand = null;
	fputc(CMD_RECOG_SI,EVR);
	delay_ms(25);
	fputc(SIgroup,EVR);
	delay_ms(25);
	if (kbhit()){
		SIreceive = fgetc(EVR);
	}
	if (SIreceive == STS_SIMILAR){
		fputc(ARG_ACK,EVR);
		delay_ms(25);
		SIreceive = null;
		SIreceive = fgetc(EVR);
		SIcommand = SIreceive - ARG_ZERO;
	}
	return SIcommand;
}
int getSDcommand(char SDgroup){	//gets the index of the SD commands
	char SDreceive = null;
	char SDcommand = null;
	fputc(CMD_RECOG_SD,EVR);
	delay_ms(25);
	fputc(SDgroup,EVR);
	delay_ms(25);
	if (kbhit()){
		SDreceive = fgetc(EVR);
	}
	if (SDreceive == STS_RESULT){
		fputc(ARG_ACK,EVR);
		delay_ms(25);
		SDreceive = null;
		SDreceive = fgetc(EVR);
		SDcommand = SDreceive - ARG_ZERO;
	}
	return SDcommand;
}
int1 isTimeout(){				//returns true if there is a timeout
	char recTimeout = null;
	if (kbhit()){
		recTimeout = fgetc(EVR);
	}
	if (recTimeout == STS_TIMEOUT){
		return TRUE;
	}else{
		return FALSE;
	}
}
void playback(int playback){	//plays back custom sound
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
			}
			break;
		case SND_zero:
			while (rec_play != STS_SUCCESS){
				
			}
			break;
		case SND_one:
			while (rec_play != STS_SUCCESS){
				
			}
			break;
		case SND_two:
			while (rec_play != STS_SUCCESS){
				
			}
			break;
		case SND_four:
			while (rec_play != STS_SUCCESS){
				
			}
			break;
		case SND_five:
			while (rec_play != STS_SUCCESS){
				
			}
			break;
		case SND_six:
			while (rec_play != STS_SUCCESS){
				
			}
			break;
		case SND_seven:
			while (rec_play != STS_SUCCESS){
				
			}
			break;
		case SND_eight:
			while (rec_play != STS_SUCCESS){
				
			}
			break;
		case SND_nine:
			while (rec_play != STS_SUCCESS){
				
			}
			break;
		case SND_please_say_a_command:
			while (rec_play != STS_SUCCESS){
				
			}
			break;
		case SND_hello_please_say_a_command:
			while (rec_play != STS_SUCCESS){
				
			}
			break;
		case SND_who_would_you_like_to_call:
			while (rec_play != STS_SUCCESS){
				
			}
			break;
		case SND_calling:
			while (rec_play != STS_SUCCESS){
				
			}
			break;
		case SND_doctor:
			while (rec_play != STS_SUCCESS){
				
			}
			break;
		case SND_say_send_to_place_the_call:
			while (rec_play != STS_SUCCESS){
				
			}
			break;
		case SND_say_reject_to_reject_the_call:
			while (rec_play != STS_SUCCESS){
				
			}
			break;
		case SND_say_answer_to_answer_the_call:
			while (rec_play != STS_SUCCESS){
				
			}
			break;
		case SND_say_end_to_cancel:
			while (rec_play != STS_SUCCESS){
				
			}
			break;
		case SND_turning_on_LED:
			while (rec_play != STS_SUCCESS){
				
			}
			break;
		case SND_turning_on_light_bulb:
			while (rec_play != STS_SUCCESS){
				
			}
			break;
		case SND_turning_off_led:
			while (rec_play != STS_SUCCESS){
				
			}
			break;
		case SND_turning_off_light_bulb:
			while (rec_play != STS_SUCCESS){
				
			}
			break;
		case SND_you_have_an_incoming_call:
			while (rec_play != STS_SUCCESS){
				
			}
			break;
		case SND_command_accepted:
			while (rec_play != STS_SUCCESS){
				
			}
			break;
		default:
			break;
	}
}
void SDaction(){				//performs tasks based on what command was given to the module
	int groupSD = null;
	int idxSD = null;	
	switch (groupSD){
		case GROUP_1:
			switch (idxSD){
				case G1_CALL:
					// write your action code here
					// groupSD = GROUP_X; <-- or jump to another group X for composite commands
					break;
				case G1_LIGHTS:
					// write your action code here
					// groupSD = GROUP_X; <-- or jump to another group X for composite commands
					break;
				case G1_LED:
					// write your action code here
					// groupSD = GROUP_X; <-- or jump to another group X for composite commands
					break;
			}
			break;
		case GROUP_2:
			switch (idxSD){
				case G2_DOCTOR:
					// write your action code here
					// groupSD = GROUP_X; <-- or jump to another group X for composite commands
					break;
				case G2_GUARDIAN:
					// write your action code here
					// groupSD = GROUP_X; <-- or jump to another group X for composite commands
					break;
				case G2_MOM:
					// write your action code here
					// groupSD = GROUP_X; <-- or jump to another group X for composite commands
					break;
				case G2_SON:
					// write your action code here
					// groupSD = GROUP_X; <-- or jump to another group X for composite commands
					break;
				case G2_DAUGHTER:
					// write your action code here
					// groupSD = GROUP_X; <-- or jump to another group X for composite commands
					break;
				case G2_DAD:
					// write your action code here
					// groupSD = GROUP_X; <-- or jump to another group X for composite commands
					break;
				case G2_ON:
					// write your action code here
					// groupSD = GROUP_X; <-- or jump to another group X for composite commands
					break;
				case G2_OFF:
					// write your action code here
					// groupSD = GROUP_X; <-- or jump to another group X for composite commands
					break;
				case G2_OTHER:
					// write your action code here
					// groupSD = GROUP_X; <-- or jump to another group X for composite commands
					break;
			}
			break;
	}
}
void SIaction(){
	int groupSI = null;
	int idxSI = null;
	switch (groupSI){
		case TRIGGER_0:
			switch (idxSI){
				case Robot:
					break;
			}
			break;
		case WORDSET_1:
			switch (idxSI){
				case WS1_ACTION:
					break;
				case WS1_MOVE:
					break;
				case WS1_TURN:
					break;
				case WS1_RUN:
					break;
				case WS1_LOOK:
					break;
				case WS1_ATTACK:
					break;
				case WS1_STOP:
					break;
				case WS1_HELLO:
					break;
			}
			break;
		case WORDSET_2:
			switch (idxSI){
				case WS2_LEFT:
					break;
				case WS2_RIGHT:
					break;
				case WS2_UP:
					break;
				case WS2_DOWN:
					break;
				case WS2_FORWARD:
					break;
				case WS2_BACKWARD:
					break;
			}
			break;
		case WORDSET_3:
			switch (idxSI){
				case WS3_ZERO:
					break;
				case WS3_ONE:
					break;
				case WS3_TWO:
					break;
				case WS3_THREE:
					break;
				case WS3_FOUR:
					break;
				case WS3_FIVE:
					break;
				case WS3_SIX:
					break;
				case WS3_SEVEN:
					break;
				case WS3_EIGHT:
					break;
				case WS3_NINE:
					break;
				case WS3_TEN:
					break;					
			}
			break;
		case GRAMMAR_4:
			switch (idxSI){
				case GR4_OK_EASYVEEAR:
				break;
			}
			break;
		case GRAMMAR_5:
			switch (idxSI){
				case GR5_NEXT:
					break;
				case GR5_PREVIOUS:
					break;
				case GR5_STOP:
					break;
				case GR5_PLAY:
					break;
				case GR5_PAUSE:
					break;
				case GR5_RECORD:
					break;
			}
			break;
		case GRAMMAR_6:
			switch (idxSI){
				case GR6_TURN_ON:
					break;
				case GR6_SWITCH_OFF:
					break;
				case GR6_INCREASE:
					break;
				case GR6_DECREASE:
					break;
				case GR6_OPEN:
					break;
				case GR6_CLOSE:
					break;
				case GR6_START:
					break;
				case GR6_STOP:
					break;

			}
			break;
		case GRAMMAR_7:
			switch (idxSI){
				case GR7_DOOR:
					break;
				case GR7_T_V:
					break;
				case GR7_TEMPERATURE:
					break;
				case GR7_WINDOW:
					break;
				case GR7_MUSIC:
					break;
				case GR7_LIGHT:
					break;
				case GR7_TIMER:
					break;
				case GR7_OVEN:
					break;
				case GR7_WASHING_MACHINE:
					break;
				case GR7_DISH_WASHER:
					break;
			}
			break;
		case GRAMMAR_8:
			switch (idxSI){
				case GR8_YES:
					break;
				case GR8_NO:
					break;
			}
			break;
		case GRAMMAR_9:
			switch (idxSI){
				case GR9_HELLO_DEVICE:
					break;
			}
			break;
		case GRAMMAR_10:
			switch (idxSI){
				case GR10_PASSWORD_SETUP:
					break;
				case GR10_PASSWORD_ACTIVATED:
					break;
				case GR10_CURTAIN_UP:
					break;
				case GR10_CURTAIN_DOWN:
					break;
				case GR10_LIGHTS_ON:
					break;
				case GR10_LIGHTS_OFF:
					break;
			}
			break;
	}
}
//<<end of FUNCTION DEFINITIONS


//====================M A I N  F U N C T I O N====================//
void main(){
	//halt();				//stop or wake up device
	//sleep_wake();			//sets to wake on whistle
	setTimeout(ARG_FIVE);	//set the timeout for 5 seconds
	setLang(ARG_ZERO);		//set the language to english
	
	toggle_pwr();
	
	output_high(PIN_D4);

//	const int CR = 0x0d;
//	fprintf(GSM,"AT\r\n");	/* checks are they communicating */ 
//	fprintf(GSM,"AT+CMGF=1\r\n"); /* sends an at command reay to send */ 
//	fputc(CR,GSM); 
//	delay_ms(1000);	/*sends a carriage return */ 
//	fprintf(GSM,"AT+CMGS=\"07749960685\""); /* sents the following text to this number */ 
//	fprintf(GSM,"\r\n");	/* cariage */ 
//	fputc(CR,GSM);  

//	delay_ms(1000); 
//	fprintf(GSM,"Hi world\r\n"); /* the text that will actually be sent */ 

//	/* sends CTRL Z */ 
//	delay_ms(1000); 
//	fprintf(GSM,"\r\n"); 
//	fputc(0x1A,GSM); /* sends carriage return */
	
}
//<<end of MAIN FUNCTION
//Hare Krsna Hare Krsna Krsna Krsna Hare Hare Hare Rama Hare Rama Rama Rama Hare Hare//