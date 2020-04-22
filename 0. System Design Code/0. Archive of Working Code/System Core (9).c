//this code works, but it outputs an 'x' to the screen and
//it doesnt loop back like it fickin should have
//in the next version, I am going to try stuff I found from the book


//Hare Krsna Hare Krsna Krsna Krsna Hare Hare Hare Rama Hare Rama Rama Rama Hare Hare//
//                    =======================                    //
//====================I N T R O D U C T I O N====================//
//                    =======================                    //
/*		
 *		Author:		Mr. Nitai Barran
 *		Supervisor: Prof. Stephan J.G. Gift
 *		System:		Voice Activated System for Emergencies
 */
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



//                    =======================                    //
//====================E N U M E R A T I O N S====================//
//                    =======================                    //
//********Variable Def**********//
	volatile int recType = NUL;
	///////////////////
	volatile int groupSD = NUL;
	volatile int idxSD = NUL;
	///////////////////
	volatile int groupSI = NUL;
	volatile int idxSI = NUL;
	///////////////////
	volatile unsigned int call_flag = FALSE;
	///////////////////
	//these are automatically stored in RAM
 	char AT[] = "AT\r\n";// To initialize mode
	// char OK[] = "OK";
	char noecho[] = "ATE0\r\n";
	char mode_text[] = "AT+CMGF=1";   // to set text mode
	char endCall[] = "ATH\r\n";
	char ansCall[] = "ATA\r\n";
	//char ring[] = "\r\nRing\r\n";
	//char mode_text[] = "AT+CMGF=1";   // to set text mode
	//char char_mode[] = "AT+CSCS=\"GSM\"";   // to set character mode
	//char param[] = "AT+CSMP=17,167,0,0";   // set the parameter of character
	//char mobile_no[] = "AT+CMGS=\"+92090078601\"";   //use to set recepient number and mesg
	
//************Errors************//
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
enum rec_type{
	SI = 0, SD = 1,
};

//*Speaker Independent Commands*//
enum wordset{					//constants for list of SI Commands
	TRIGGER_0 = 0,
	WORDSET_1 = 1, WORDSET_2 = 2, WORDSET_3 = 3,
	GRAMMAR_4 = 4, GRAMMAR_5 = 5, GRAMMAR_6 = 6,
	GRAMMAR_7 = 7, GRAMMAR_8 = 8, GRAMMAR_9 = 9,
	GRAMMAR_10 = 10,
};
enum trigger0{					//constants for trigger
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




//                   ========================                    //
//===================F U N C T I O N  D E F N====================//
//                   ========================                    //
//**********System Fns**********//
void blinkA0(int _a){			//LED indicator
	int _i = 0;
	for (_i; _i < _a; _i++ ){
		output_high(PIN_A0);
		delay_ms(50);
		output_low(PIN_A0);
		delay_ms(100);
	}
}
void blinkA1(int _a){			//LED indicator
	int _i = 0;	
	for (_i; _i < _a; _i++ ){
		output_high(PIN_A1);
		delay_ms(50);
		output_low(PIN_A1);
		delay_ms(100);
	}
}
void blinkD2(int _a){			//bulb indicator
	int _i = 0;	
	for (_i; _i < _a; _i++ ){
		output_high(PIN_D2);
		delay_ms(50);
		output_low(PIN_D2);
		delay_ms(100);
	}
}
void ms25(){delay_ms(25);}		//easy way to call delay_ms(25)

//***********Fona Fns***********//
/* void toggle_pwr(){
	output_low(PIN_C4);
	delay_ms(2000);
	output_high(PIN_C4);
} */
void useGSM(){fputc(NUL,GSM);}			//ensures get_string()[from input.c] uses GSM RS232 stream
void usePC(){fputc(NUL,PC);}			//ensures get_string()[from input.c] uses GSM RS232 stream
void gsm_reset(){
	output_low(PIN_C5);
	delay_ms(100);
	output_high(PIN_C5);
}
/* boolean gsmReplyOK(char _reply_gsm[]){	//checks to see if the reply is ring...might not actually be needed
	if(strncmp(_reply_gsm,OK,strlen(OK))==0){
		return true;
	}else{
		return false;
	}
} */
void gsmSetup(){
//NOTE: we do not necessairly need to check to see if the data received is "OK". we can asume that it will happen
	fprintf(GSM,AT);
	delay_us(50);
	fprintf(GSM,noecho);
	delay_us(50);
	fprintf(GSM,mode_text);
}

void gsmAction(char _a){
	switch (_a){
		case 'a':
			fprintf(GSM,ansCall);
			break;
		case 'c':
			fprintf(GSM,"ATD+18686822015;\r\n");
			break;
		case 'e':
			fprintf(GSM,endCall);
			break;
		default: _a = NUL;
		break;
	}
}

//*********EasyVR Fns***********//
void halt(){						//stop current action
	char _rec_halt = NUL;
	while (_rec_halt != STS_SUCCESS){
		fputc(CMD_BREAK,EVR);
		ms25();
		if (kbhit(EVR)){_rec_halt = fgetc(EVR);}
	}
	_rec_halt = NUL;
}
void sleep_wake(){					//put device to sleep and wake on whistle or received a character
	char _rec_sleep = NUL;
	while (_rec_sleep != STS_SUCCESS){
		fputc(CMD_SLEEP,EVR);
		ms25();
		fputc(ARG_ONE,EVR);
		ms25();
		if (kbhit(EVR)){_rec_sleep = fgetc(EVR);}
	}
	_rec_sleep = NUL;
}
void setLang(char _whatLang){		//set the language based on the argument passed
	char _rec_setlang = NUL;
	while (_rec_setlang != STS_SUCCESS){
		fputc(CMD_LANGUAGE,EVR);
		ms25();
		fputc(_whatLang,EVR);
		ms25();
		if (kbhit(EVR)){_rec_setlang = fgetc(EVR);}
	}
}
void setTimeout(char _period){		//set a period of the timeout based on the argument passed
	char _rec_time = NUL;
	while (_rec_time != STS_SUCCESS){
		fputc(CMD_TIMEOUT,EVR);
		ms25();
		fputc(_period,EVR);
		ms25();
		if (kbhit(EVR)){_rec_time = fgetc(EVR);}
	}
}
int recSIcmd(char _SIgroup){		//starts recognition of SI commands or Custom Grammars
	char _SIrec = NUL;
	fputc(CMD_RECOG_SI,EVR);
	ms25();
	fputc(_SIgroup,EVR);
	ms25();
	if (kbhit(EVR)){_SIrec = fgetc(EVR);}
	return _SIrec;				//this should return success or failure
}
int getSIcmd(char _SIrec){			//gets the index of the SI commands or Custom Grammars
	char _SIcmd = NUL;
	if (_SIrec == STS_SIMILAR){
		fputc(ARG_ACK,EVR);
		ms25();
		_SIrec = NUL;
		if (kbhit(EVR)){_SIrec = fgetc(EVR);}
		_SIcmd = _SIrec - ARG_ZERO;
	return _SIcmd;
	}
}
int recSDcmd(char _SDgroup){		//starts recognition of SD commands or Custom Commands
	char _SDrec = NUL;
	fputc(CMD_RECOG_SD,EVR);
	ms25();
	fputc(_SDgroup,EVR);
	ms25();
		if (kbhit(EVR)){_SDrec = fgetc(EVR);}
	return _SDrec;
}
int getSDcmd(char _SDrec){			//gets the index of the SD commands or Custom Commands
	char _SDcmd = NUL;
	if (_SDrec == STS_RESULT){
		fputc(ARG_ACK,EVR);
		ms25();
		_SDrec = NUL;
		if (kbhit(EVR)){_SDrec = fgetc(EVR);}
		_SDcmd = _SDrec - ARG_ZERO;
	return _SDcmd;
	}
}
boolean isTimeout(char _recTimeout){//returns true if there is a timeout
	if (kbhit(EVR)){_recTimeout = fgetc(EVR);}
	if (_recTimeout == STS_TIMEOUT){
		return true;
	}else{
		return false;
	}
}
void playback(int _playback){		//plays back custom sound
	char _rec_play = NUL;
	switch (_playback){
		case BEEP:
			while (_rec_play != STS_SUCCESS){
				fputc(CMD_PLAY_SX,EVR);
				ms25();
				fputc(ARG_ZERO,EVR);
				ms25();
				fputc(ARG_ZERO,EVR);
				ms25();
				fputc(ARG_FIFTEEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
			}
			break;
		case SND_zero:
			while (_rec_play != STS_SUCCESS){
				fputc(CMD_PLAY_SX,EVR);
				ms25();
				fputc(ARG_ZERO,EVR);
				ms25();
				fputc(ARG_ONE,EVR);
				ms25();
				fputc(ARG_FIFTEEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
			}
			break;
		case SND_one:
			while (_rec_play != STS_SUCCESS){
				fputc(CMD_PLAY_SX,EVR);
				ms25();
				fputc(ARG_ZERO,EVR);
				ms25();
				fputc(ARG_TWO,EVR);
				ms25();
				fputc(ARG_FIFTEEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
			}
			break;
		case SND_two:
			while (_rec_play != STS_SUCCESS){
				fputc(CMD_PLAY_SX,EVR);
				ms25();
				fputc(ARG_ZERO,EVR);
				ms25();
				fputc(ARG_THREE,EVR);
				ms25();
				fputc(ARG_FIFTEEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
			}
			break;
		case SND_three:
			while (_rec_play != STS_SUCCESS){
				fputc(CMD_PLAY_SX,EVR);
				ms25();
				fputc(ARG_ZERO,EVR);
				ms25();
				fputc(ARG_FOUR,EVR);
				ms25();
				fputc(ARG_FIFTEEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
			}
			break;
		case SND_four:
			while (_rec_play != STS_SUCCESS){
				fputc(CMD_PLAY_SX,EVR);
				ms25();
				fputc(ARG_ZERO,EVR);
				ms25();
				fputc(ARG_FIVE,EVR);
				ms25();
				fputc(ARG_FIFTEEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
			}
			break;
		case SND_five:
			while (_rec_play != STS_SUCCESS){
				fputc(CMD_PLAY_SX,EVR);
				ms25();
				fputc(ARG_ZERO,EVR);
				ms25();
				fputc(ARG_SIX,EVR);
				ms25();
				fputc(ARG_FIFTEEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
			}
			break;
		case SND_six:
			while (_rec_play != STS_SUCCESS){
				fputc(CMD_PLAY_SX,EVR);
				ms25();
				fputc(ARG_ZERO,EVR);
				ms25();
				fputc(ARG_SEVEN,EVR);
				ms25();
				fputc(ARG_FIFTEEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
			}
			break;
		case SND_seven:
			while (_rec_play != STS_SUCCESS){
				fputc(CMD_PLAY_SX,EVR);
				ms25();
				fputc(ARG_ZERO,EVR);
				ms25();
				fputc(ARG_EIGHT,EVR);
				ms25();
				fputc(ARG_FIFTEEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
			}
			break;
		case SND_eight:
			while (_rec_play != STS_SUCCESS){
				fputc(CMD_PLAY_SX,EVR);
				ms25();
				fputc(ARG_ZERO,EVR);
				ms25();
				fputc(ARG_NINE,EVR);
				ms25();
				fputc(ARG_FIFTEEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
			}
			break;
		case SND_nine:
			while (_rec_play != STS_SUCCESS){
				fputc(CMD_PLAY_SX,EVR);
				ms25();
				fputc(ARG_ZERO,EVR);
				ms25();
				fputc(ARG_TEN,EVR);
				ms25();
				fputc(ARG_FIFTEEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
			}
			break;
		case SND_please_say_a_command:
			while (_rec_play != STS_SUCCESS){
				fputc(CMD_PLAY_SX,EVR);
				ms25();
				fputc(ARG_ZERO,EVR);
				ms25();
				fputc(ARG_ELEVEN,EVR);
				ms25();
				fputc(ARG_FIFTEEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
			}
			break;
		case SND_hello_please_say_a_command:
			while (_rec_play != STS_SUCCESS){
				fputc(CMD_PLAY_SX,EVR);
				ms25();
				fputc(ARG_ZERO,EVR);
				ms25();
				fputc(ARG_TWELVE,EVR);
				ms25();
				fputc(ARG_FIFTEEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
			}
			break;
		case SND_who_would_you_like_to_call:
			while (_rec_play != STS_SUCCESS){
				fputc(CMD_PLAY_SX,EVR);
				ms25();
				fputc(ARG_ZERO,EVR);
				ms25();
				fputc(ARG_THIRTEEN,EVR);
				ms25();
				fputc(ARG_FIFTEEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
			}
			break;
		case SND_calling:
			while (_rec_play != STS_SUCCESS){
				fputc(CMD_PLAY_SX,EVR);
				ms25();
				fputc(ARG_ZERO,EVR);
				ms25();
				fputc(ARG_FOURTEEN,EVR);
				ms25();
				fputc(ARG_FIFTEEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
			}
			break;
		case SND_doctor:
			while (_rec_play != STS_SUCCESS){
				fputc(CMD_PLAY_SX,EVR);
				ms25();
				fputc(ARG_ZERO,EVR);
				ms25();
				fputc(ARG_FIFTEEN,EVR);
				ms25();
				fputc(ARG_FIFTEEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
			}
			break;
		case SND_say_send_to_place_the_call:
			while (_rec_play != STS_SUCCESS){
				fputc(CMD_PLAY_SX,EVR);
				ms25();
				fputc(ARG_ZERO,EVR);
				ms25();
				fputc(ARG_SIXTEEN,EVR);
				ms25();
				fputc(ARG_FIFTEEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
			}
			break;
		case SND_say_reject_to_reject_the_call:
			while (_rec_play != STS_SUCCESS){
				fputc(CMD_PLAY_SX,EVR);
				ms25();
				fputc(ARG_ZERO,EVR);
				ms25();
				fputc(ARG_SEVENTEEN,EVR);
				ms25();
				fputc(ARG_FIFTEEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
			}
			break;
		case SND_say_answer_to_answer_the_call:
			while (_rec_play != STS_SUCCESS){
				fputc(CMD_PLAY_SX,EVR);
				ms25();
				fputc(ARG_ZERO,EVR);
				ms25();
				fputc(ARG_EIGHTEEN,EVR);
				ms25();
				fputc(ARG_FIFTEEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
			}
			break;
		case SND_say_end_to_cancel:
			while (_rec_play != STS_SUCCESS){
				fputc(CMD_PLAY_SX,EVR);
				ms25();
				fputc(ARG_ZERO,EVR);
				ms25();
				fputc(ARG_NINETEEN,EVR);
				ms25();
				fputc(ARG_FIFTEEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
			}
			break;
		case SND_turning_on_LED:
			while (_rec_play != STS_SUCCESS){
				fputc(CMD_PLAY_SX,EVR);
				ms25();
				fputc(ARG_ZERO,EVR);
				ms25();
				fputc(ARG_TWENTY,EVR);
				ms25();
				fputc(ARG_FIFTEEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
			}
			break;
		case SND_turning_on_light_bulb:
			while (_rec_play != STS_SUCCESS){
				fputc(CMD_PLAY_SX,EVR);
				ms25();
				fputc(ARG_ZERO,EVR);
				ms25();
				fputc(ARG_TWENTYONE,EVR);
				ms25();
				fputc(ARG_FIFTEEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
			}
			break;
		case SND_turning_off_led:
			while (_rec_play != STS_SUCCESS){
				fputc(CMD_PLAY_SX,EVR);
				ms25();
				fputc(ARG_ZERO,EVR);
				ms25();
				fputc(ARG_TWENTYTWO,EVR);
				ms25();
				fputc(ARG_FIFTEEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
			}
			break;
		case SND_turning_off_light_bulb:
			while (_rec_play != STS_SUCCESS){
				fputc(CMD_PLAY_SX,EVR);
				ms25();
				fputc(ARG_ZERO,EVR);
				ms25();
				fputc(ARG_TWENTYTHREE,EVR);
				ms25();
				fputc(ARG_FIFTEEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
			}
			break; 
		case SND_you_have_an_incoming_call:
			while (_rec_play != STS_SUCCESS){
				fputc(CMD_PLAY_SX,EVR);
				ms25();
				fputc(ARG_ZERO,EVR);
				ms25();
				fputc(ARG_TWENTYFOUR,EVR);
				ms25();
				fputc(ARG_FIFTEEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
			}
			break;
		case SND_command_accepted:
			while (_rec_play != STS_SUCCESS){
				fputc(CMD_PLAY_SX,EVR);
				ms25();
				fputc(ARG_ZERO,EVR);
				ms25();
				fputc(ARG_TWENTYFIVE,EVR);
				ms25();
				fputc(ARG_FIFTEEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
			}
			break;
	}
}
void actionSD(){					//performs action based on SD commands
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
						fprintf(GSM,"ATD+18686822015;\r\n");
						
						// write your action code here
						// groupSD = GROUP_X; <-- or jump to another group X for composite commands
						break;
					case G2_GUARDIAN:
						// fprintf(GSM,"ATD+18686822015;\r\n");
						// write your action code here
						// groupSD = GROUP_X; <-- or jump to another group X for composite commands
						break;
					case G2_MOM:
						// fprintf(GSM,"ATD+18686822015;\r\n");
						// write your action code here
						// groupSD = GROUP_X; <-- or jump to another group X for composite commands
						break;
					case G2_SON:
						// fprintf(GSM,"ATD+18686822015;\r\n");
						// write your action code here
						// groupSD = GROUP_X; <-- or jump to another group X for composite commands
						break;
					case G2_DAUGHTER:
						// fprintf(GSM,"ATD+18686822015;\r\n");
						// write your action code here
						// groupSD = GROUP_X; <-- or jump to another group X for composite commands
						break;
					case G2_DAD:
						// fprintf(GSM,"ATD+18686822015;\r\n");
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
void actionSI(){					//performs action based on SI commands
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




//                       =================                       //
//=======================I N T E R U P T S=======================//
//                       =================                       //
////External Interrupt
#INT_EXT
void EXT_isr(void){
	clear_interrupt(INT_RB);	//Clear RB port change interrupt flag bit
	call_flag = TRUE;
}

////USART Interrupt
//UART Receive Buffer
#define RX_BUFFER_SIZE 24
volatile char rx_buffer[RX_BUFFER_SIZE + 1];
volatile char rx_wr_index = 0;
volatile char rx_rd_index = 0;
volatile char rx_counter = 0;
volatile int1 rx_buffer_overflow = 0;
//UART Transmit Buffer
#define TX_BUFFER_SIZE 24
volatile char tx_buffer[TX_BUFFER_SIZE + 1];
volatile char tx_rd_index = 0;
volatile char tx_wr_index = 0;
volatile char tx_counter = 0;
// UART Receiver Interrupt Service Rouine
#INT_RDA
void serial_rx_isr(void){
	rx_buffer[rx_wr_index] = fgetc(GSM);		//put the received character in a buffer
	if (++rx_wr_index > RX_BUFFER_SIZE){	//wrap the pointer
		rx_wr_index = 0;
	}
	if (++rx_counter > RX_BUFFER_SIZE){		//keep a character count overflow check
		rx_counter = RX_BUFFER_SIZE;		//if too many characters came in before the could be used, that could cause an error
		rx_buffer_overflow = 1;
	}
}
//get a character from the UART receiver buffer
char bgetc(void){
	char c;
	while (rx_counter == 0){				//wait for a character
		c = rx_buffer[rx_rd_index];			//get one from the buffer
	}
	if (++rx_rd_index > RX_BUFFER_SIZE){	//wrap the pointer
		rx_rd_index = 0;
	}
	if (rx_counter){
		rx_counter--;						//keep a character count
	}
	return c;
}
// UART Transmitter Interrupt Service Routine
#INT_TBE
void serial_tx_isr(void){
	if(tx_counter != 0){						//if there are characters to be transmitted
		fputc(tx_buffer[tx_rd_index],GSM);		//send a character out the port
		if (++tx_rd_index > TX_BUFFER_SIZE){
			tx_rd_index = 0;
		}
		tx_counter--;							//keep track of the counter
		if (tx_counter == 0){					//if there are no more characters the disable the interrupt
			disable_interrupts(INT_TBE);
		}
	}
}
//write a character to the serial transmit buffer
void bputc(char c){
	char restart = 0;
	while (tx_counter > (TX_BUFFER_SIZE - 1));	//wait, buffer is getting filled
	if (tx_counter == 0){						//if buffer empty, setup interrupt
		restart = 1;
	}
	tx_buffer[tx_wr_index++] = c;				//jam the char in the buffer
	if (tx_wr_index > TX_BUFFER_SIZE){			//wrap the pointer
		tx_wr_index = 0;
	}
	tx_counter++;								//keep track of buffered characters
	if (restart == 1){	//if there were not chars but are now, re-enable the interrupts
		enable_interrupts(INT_TBE);
	}
}


//                   ========================                    //
//===================M A I N  F U N C T I O N====================//
//                   ========================                    //
void main(){
////Some Constants and Setup
	volatile unsigned char k;
	// char reply_gsm[16];
	// char gsm_action;
	gsmSetup();
	
////setup interrupts
	set_tris_b(0x01);
	ext_int_edge(H_TO_L);			//interrupt on falling edge of pin RB0
	clear_interrupt(INT_EXT);
	//enable_interrupts(INT_EXT);
	enable_interrupts(INT_RDA);
	enable_interrupts(GLOBAL);
	
	printf(bputc, "\r\nRunning...\r\n");

////Program Loop
  	while (TRUE){
		if (rx_counter > 4){
			while (rx_counter > 0){
				k = bgetc();
				delay_ms(10);
				bputc(k);
			}
		}
		enable_interrupts(INT_RDA);
	}

/*	
//************Setup*************
	////setup for interrupts
	set_tris_b(0x01);
	ext_int_edge(H_TO_L);			//interrupt on falling edge of pin RB0
	clear_interrupt(INT_EXT);
	enable_interrupts(INT_EXT);
	enable_interrupts(INT_RDA);
	enable_interrupts(GLOBAL);

	////some setup constants
	char reply_gsm[16];
	char gsm_action;
	
	////some setup functions
 	halt();						//stop or wake up device
	setTimeout(ARG_FIVE);		//set the timeout for 5 seconds
	setLang(ARG_ZERO);			//set the language to english
	sleep_wake();				//sets to wake on whistle
	gsmSetup();
	
	////global constants initialization
	recType = 0;				//start recognition with SI commands
	groupSD = 1;				//start recognition with SD group 1
	groupSI = 0;				//start recognition with SI group 0 / trigger
	idxSD = 0;					//set SD command index to 0
	idxSI = 0;					//set SI command index to 0
	int receive;

//************Loop**************
while(TRUE){
	if (call_flag == TRUE){
		recType = 0;
		groupSD = 1;
		groupSI = 0;
		//	this would enable us to navigate to section of code--- 
		//that would be able to utalise voice recognition to answer the call.
	}else
	if (recType == SI){
		disable_interrupts(GLOBAL);
		playback(SND_hello_please_say_a_command);
		receive = recSIcmd(groupSI);
		idxSI = getSIcmd(receive);		//this function gets the command index if the operation is successful (determined from receive variable)
		if (idxSI >= 0){
			actionSI();
		}else
		if (receive == STS_TIMEOUT){	//in the case there is a timeout then the process should start over by resetting the variables
			blinkD0(2);
			recType = 0;				//start recognition with SI commands
			groupSD = ARG_ONE;			//start recognition with SD group 1
			groupSI = ARG_ZERO;			//start recognition with SI group 0 / trigger
			idxSD = 0;					//set SD command index to 0
			idxSI = 0;					//set SI command index to 0
		}else
		if (receive == STS_ERROR){		//if there is an error then the same should happen in the case of the timeout
			blinkD0(3);
			recType = 0;				//start recognition with SI commands
			groupSD = ARG_ONE;			//start recognition with SD group 1
			groupSI = ARG_ZERO;			//start recognition with SI group 0 / trigger
			idxSD = 0;					//set SD command index to 0
			idxSI = 0;					//set SI command index to 0
		}
	}else
	if (recType == SD){
		disable_interrupts(GLOBAL);
		playback(SND_hello_please_say_a_command);
		receive = recSDcmd(groupSD);
		idxSD = getSDcmd(receive);	//this function gets the command index if the operation is successful (determined from receive variable)
		if (idxSD >= 0){
			actionSD();
			blinkD0(1);
		}else
		if (receive == STS_TIMEOUT){	//in the case there is a timeout then the process should start over by resetting the variables
			blinkD0(2);
			recType = 0;				//start recognition with SI commands
			groupSD = ARG_ONE;			//start recognition with SD group 1
			groupSI = ARG_ZERO;			//start recognition with SI group 0 / trigger
			idxSD = 0;					//set SD command index to 0
			idxSI = 0;					//set SI command index to 0
		}else
		if (receive == STS_ERROR){		//if there is an error then the same should happen in the case of the timeout
			blinkD0(3);
			recType = 0;				//start recognition with SI commands
			groupSD = ARG_ONE;			//start recognition with SD group 1
			groupSI = ARG_ZERO;			//start recognition with SI group 0 / trigger
			idxSD = 0;					//set SD command index to 0
			idxSI = 0;					//set SI command index to 0
		}
	}
}  //*/
}
//Hare Krsna Hare Krsna Krsna Krsna Hare Hare Hare Rama Hare Rama Rama Rama Hare Hare//