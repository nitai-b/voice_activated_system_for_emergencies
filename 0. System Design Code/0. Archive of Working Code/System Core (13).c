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
#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,stream=EVR,bits=8)		//HW serial pins EasyVR	||	EVR
#use rs232(baud=9600,parity=N,xmit=PIN_B1,rcv=PIN_B0,stream=GSM,bits=8,SAMPLE_EARLY)		//SW serial pins Fona	||	GSM
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
	//these are automatically stored in RAM
 	char AT[] = "AT\r\n";// To initialize mode
	// char OK[] = "OK";
	char save[] = "AT&W\r\n";
	char noecho[] = "ATE0\r\n";
	char mode_text[] = "AT+CMGF=1\r\n";   // to set text mode
	char endCall[] = "ATH\r\n";
	char ansCall[] = "ATA\r\n";
	//char ring[] = "\r\nRing\r\n";
	//char mode_text[] = "AT+CMGF=1";   // to set text mode
	//char char_mode[] = "AT+CSCS=\"GSM\"";   // to set character mode
	//char param[] = "AT+CSMP=17,167,0,0";   // set the parameter of character
	//char mobile_no[] = "AT+CMGS=\"+92090078601\"";   //use to set recepient number and mesg
	void print_debug(char debug){fputc(debug,PC);}
	


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
char wordsetLUT(int _wordset){
	char _a;
	switch (_wordset){
		case TRIGGER_0: 	_a = ARG_ZERO	; break;
		case WORDSET_1: 	_a = ARG_ONE	; break;
		case WORDSET_2: 	_a = ARG_TWO	; break;
		case WORDSET_3: 	_a = ARG_THREE	; break;
		case GRAMMAR_4: 	_a = ARG_FOUR	; break;
		case GRAMMAR_5: 	_a = ARG_FIVE	; break;
		case GRAMMAR_6: 	_a = ARG_SIX	; break;
		case GRAMMAR_7: 	_a = ARG_SEVEN	; break;
		case GRAMMAR_8: 	_a = ARG_EIGHT	; break;
		case GRAMMAR_9: 	_a = ARG_NINE	; break;
		case GRAMMAR_10: 	_a = ARG_TEN	; break;
	}return _a;
}
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
char groupLUT(int _group){
	char _a;
	switch (_group){
		case GROUP_1: _a = ARG_ONE; break;
		case GROUP_2: _a = ARG_TWO; break;
	}return _a;
}
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
void ms25(){delay_ms(30);}		//easy way to call delay_ms(25)

//***********Fona Fns***********//
/* void toggle_pwr(){
	output_low(PIN_C4);
	delay_ms(2000);
	output_high(PIN_C4);
} */
void useGSM(){fputc(NUL,GSM);}			//ensures get_string()[from input.c] uses GSM RS232 stream
void usePC(){fputc(NUL,PC);}			//ensures get_string()[from input.c] uses GSM RS232 stream
void reset_gsm(){
	output_low(PIN_C5);
	delay_ms(100);
	output_high(PIN_C5);
	delay_ms(10000);	//delays the code so that the GSM module would be able to reconnect to the network
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
	delay_us(50);
	fprintf(GSM,save);
	delay_us(50);
	
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
		if (kbhit(EVR)) _rec_halt = fgetc(EVR);
		print_debug(_rec_halt);
	}
	_rec_halt = NUL;
}
void wake_on_whistle(){				//put device to sleep and wake on whistle or received a character
	char _rec_sleep = NUL;
	char back_from_power_down = NUL;
	while (_rec_sleep != STS_SUCCESS){
		fputc(CMD_SLEEP,EVR);
		ms25();
		fputc(ARG_THREE,EVR);
		ms25();
		if (kbhit(EVR)) _rec_sleep = fgetc(EVR);
		print_debug(_rec_sleep);
	}
	while (back_from_power_down != STS_AWAKEN){
		if (kbhit(EVR))	back_from_power_down = fgetc(EVR);
		print_debug(back_from_power_down);
	}
	back_from_power_down = NUL;
	_rec_sleep = NUL;
}
void setLang(char _whatLang){		//set the language based on the argument passed
	char _rec_setlang = NUL;
	while (_rec_setlang != STS_SUCCESS){
		fputc(CMD_LANGUAGE,EVR);
		ms25();
		fputc(_whatLang,EVR);
		ms25();
		if (kbhit(EVR)) _rec_setlang = fgetc(EVR);
		print_debug(_rec_setlang);
	}
}
void setTimeout(char _period){		//set a period of the timeout based on the argument passed
	char _rec_time = NUL;
	while (_rec_time != STS_SUCCESS){
		fputc(CMD_TIMEOUT,EVR);
		ms25();
		fputc(_period,EVR);
		ms25();
		if (kbhit(EVR)) _rec_time = fgetc(EVR);
		print_debug(_rec_time);
	}
}
int recSIcmd(char _SIgroup){		//starts recognition of SI commands or Custom Grammars
	char _SIrec = NUL;
	fputc(CMD_RECOG_SI,EVR);
	ms25();
	fputc(_SIgroup,EVR);
	ms25();
	_SIrec = fgetc(EVR);
	print_debug(_SIrec);
	return _SIrec;				//this should return success or failure
}
int getSIcmd(char _SIrec){			//gets the index of the SI commands or Custom Grammars
	char _SIcmd = NUL;
	if (_SIrec == STS_SIMILAR){
		fputc(ARG_ACK,EVR);
		ms25();
		_SIrec = NUL;
		_SIrec = fgetc(EVR);
		_SIcmd = _SIrec - ARG_ZERO;
		print_debug(_SIcmd);
	return _SIcmd;
	}
}
int recSDcmd(char _SDgroup){		//starts recognition of SD commands or Custom Commands
	char _SDrec = NUL;
	fputc(CMD_RECOG_SD,EVR);
	ms25();
	fputc(_SDgroup,EVR);
	ms25();
	_SDrec = fgetc(EVR);
	print_debug(_SDrec);
	return _SDrec;
}
int getSDcmd(char _SDrec){			//gets the index of the SD commands or Custom Commands
	char _SDcmd = NUL;
	if (_SDrec == STS_RESULT){
		fputc(ARG_ACK,EVR);
		ms25();
		_SDrec = NUL;
		_SDrec = fgetc(EVR);
		_SDcmd = _SDrec - ARG_ZERO;
		print_debug(_SDcmd);
	return _SDcmd;
	}
}
boolean isTimeout(char _recTimeout){//returns true if there is a timeout
	_recTimeout = fgetc(EVR);
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
				print_debug(_rec_play);
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
				fputc(ARG_SEVEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
				print_debug(_rec_play);
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
				fputc(ARG_SEVEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
				print_debug(_rec_play);
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
				fputc(ARG_SEVEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
				print_debug(_rec_play);
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
				fputc(ARG_SEVEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
				print_debug(_rec_play);
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
				fputc(ARG_SEVEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
				print_debug(_rec_play);
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
				fputc(ARG_SEVEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
				print_debug(_rec_play);
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
				fputc(ARG_SEVEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
				print_debug(_rec_play);
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
				fputc(ARG_SEVEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
				print_debug(_rec_play);
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
				fputc(ARG_SEVEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
				print_debug(_rec_play);
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
				fputc(ARG_SEVEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
				print_debug(_rec_play);
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
				fputc(ARG_SEVEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
				print_debug(_rec_play);
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
				fputc(ARG_SEVEN,EVR);
				// ms25(); ms25():
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
				print_debug(_rec_play);
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
				fputc(ARG_SEVEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
				print_debug(_rec_play);
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
				fputc(ARG_SEVEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
				print_debug(_rec_play);
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
				fputc(ARG_SEVEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
				print_debug(_rec_play);
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
				fputc(ARG_SEVEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
				print_debug(_rec_play);
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
				fputc(ARG_SEVEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
				print_debug(_rec_play);
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
				fputc(ARG_SEVEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
				print_debug(_rec_play);
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
				fputc(ARG_SEVEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
				print_debug(_rec_play);
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
				fputc(ARG_SEVEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
				print_debug(_rec_play);
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
				fputc(ARG_SEVEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
				print_debug(_rec_play);
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
				fputc(ARG_SEVEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
				print_debug(_rec_play);
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
				fputc(ARG_SEVEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
				print_debug(_rec_play);
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
				fputc(ARG_SEVEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
				print_debug(_rec_play);
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
				fputc(ARG_SEVEN,EVR);
				ms25();
				if (kbhit(EVR)){_rec_play = fgetc(EVR);}
				print_debug(_rec_play);
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
// #DEVICE HIGH_INTS=TRUE
volatile unsigned int call_flag = FALSE;
#INT_EXT //this interrupt service routine would interrupt on change from high to low of the start bit of a serial transmission
void EXT_isr(void){
	clear_interrupt(INT_RB);	//Clear RB port change interrupt flag bit
	call_flag = TRUE;
}

/* #define BUFFER_SIZE 32 
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
		if (rx_buff_index < 2){
			buffer[rx_buff_index] = getc();
			// if (buffer[rx_buff_index] == (CR||LF)){
				// buffer[rx_buff_index] = NUL;
			// }
		}
		if (rx_buff_index >= 2){
			buffer[rx_buff_index] = getc();
			if (buffer[rx_buff_index] == (CR||LF)){
				got_string_flag = 1;
				rx_buff_index = 0;
				disable_interrupts(INT_RDA);
			}
		}
		++rx_buff_index;
	}
}


//this is a modified version of the function from INPUT.c
void bgets(char* s, unsigned int8 max){
	unsigned char _c;
	unsigned int8 len;
	unsigned int8 rd_buff_index;
	unsigned int eos = 0;

	--max;
	len = 0;
	rd_buff_index = 0;
	do{		
		_c = buffer[rd_buff_index];
		if (rd_buff_index < 2){
			if (_c == (CR||LF)) _c = NUL;
			if (_c != (CR||LF)) s[len] = _c;
		}
		if (rd_buff_index >= 2){
			if (_c == (CR||LF)) eos = 1;
			s[len] = _c;
		}
		++rd_buff_index;
		++len;
	}while(!eos);
	s[len] = 0;
	len = 0;
	rx_buff_index = 0;
	rd_buff_index = 0;
}
 */



//                   ========================                    //
//===================M A I N  F U N C T I O N====================//
//                   ========================                    //
void main(){
//************Setup*************

	////setup for interrupts
	ext_int_edge(H_TO_L);			//interrupt on falling edge of pin RB0
	clear_interrupt(INT_EXT);
	enable_interrupts(INT_EXT);
	// enable_interrupts(INT_RDA);
	enable_interrupts(GLOBAL);

	////some setup constants
	// char gsm_action;
	// char c,k;
	// fprintf(PC,"enter 1 for ON\r\nenter0 for OFF");
	// while(1){
		// c = fgetc(PC);
		// switch (c){
			// case '0': output_low(PIN_D1); break;
		// }
		
	// }
	
	////some setup functions
 	halt();						//stop or wake up device
	setTimeout(ARG_FIVE);		//set the timeout for 5 seconds
	setLang(ARG_ZERO);			//set the language to english
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
	wake_on_whistle();
	// if (call_flag == TRUE){
		// enable_interrupts(GLOBAL);
		// wake_on_whistle(); 	//this allows us to wait until a person whistles to wake the system
								//then the following instructions will follow.
		// recType = 0;
		// groupSD = 1;
		// groupSI = 0;
		//this would enable us to navigate to section of code--- 
		//that would be able to utalise voice recognition to answer the call.
	// }else
	if (recType == SI){
		disable_interrupts(GLOBAL);
		playback(SND_hello_please_say_a_command);
		receive = recSIcmd(groupLUT(groupSI));
		idxSI = getSIcmd(receive);		//this function gets the command index if the operation is successful (determined from receive variable)
		if (idxSI >= 0){
			actionSI();
		}else
		if (receive == STS_TIMEOUT){	//in the case there is a timeout then the process should start over by resetting the variables
			blinkA0(2);
			recType = 0;				//start recognition with SI commands
			groupSD = 1;				//start recognition with SD group 1
			groupSI = 0;				//start recognition with SI group 0 / trigger
			idxSD = 0;					//set SD command index to 0
			idxSI = 0;					//set SI command index to 0
		}else
		if (receive == STS_ERROR){		//if there is an error then the same should happen in the case of the timeout
			blinkA0(3);
			recType = 0;				//start recognition with SI commands
			groupSD = 1;				//start recognition with SD group 1
			groupSI = 0;				//start recognition with SI group 0 / trigger
			idxSD = 0;					//set SD command index to 0
			idxSI = 0;					//set SI command index to 0
		}
		enable_interrupts(GLOBAL);
	}else
	if (recType == SD){
		disable_interrupts(GLOBAL);
		playback(SND_hello_please_say_a_command);
		receive = recSDcmd(groupSI);
		idxSD = getSDcmd(receive);	//this function gets the command index if the operation is successful (determined from receive variable)
		if (idxSD >= 0){
			actionSD();
			blinkA0(1);
		}else
		if (receive == STS_TIMEOUT){	//in the case there is a timeout then the process should start over by resetting the variables
			blinkA0(2);
			recType = 0;				//start recognition with SI commands
			groupSD = 1;				//start recognition with SD group 1
			groupSI = 0;				//start recognition with SI group 0 / trigger
			idxSD = 0;					//set SD command index to 0
			idxSI = 0;					//set SI command index to 0
		}else
		if (receive == STS_ERROR){		//if there is an error then the same should happen in the case of the timeout
			blinkA0(3);
			recType = 0;				//start recognition with SI commands
			groupSD = ARG_ONE;			//start recognition with SD group 1
			groupSI = ARG_ZERO;			//start recognition with SI group 0 / trigger
			idxSD = 0;					//set SD command index to 0
			idxSI = 0;					//set SI command index to 0
		}
		enable_interrupts(GLOBAL);
	}
} //*/
}
//Hare Krsna Hare Krsna Krsna Krsna Hare Hare Hare Rama Hare Rama Rama Rama Hare Hare//