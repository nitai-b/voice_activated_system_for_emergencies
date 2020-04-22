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
#include <groups.h>
#fuses HS,NOLVP,NOWDT,PUT,NOPROTECT
#use delay(clock=10000000)
#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,stream=EVR,bits=8)		//HW serial pins EasyVR	||	EVR
#use rs232(baud=9600,parity=N,xmit=PIN_B1,rcv=PIN_B0,stream=GSM,bits=8, SAMPLE_EARLY)		//SW serial pins Fona	||	GSM
#use rs232(baud=9600,parity=N,xmit=PIN_A4,rcv=PIN_A3,stream=PC,bits=8)		//SW serial pins Debug	||	DB
#include <input.c> //this file must be ONLY included after #use rs232



//                    =======================                    //
//====================E N U M E R A T I O N S====================//
//                    =======================                    //
//********Variable Def**********//
	volatile int recType = NUL;
	///////////////////
	volatile char groupSD = NUL;
	volatile int idxSD = NUL;
	///////////////////
	volatile char groupSI = NUL;
	volatile int idxSI = NUL;
	volatile int reset = FALSE;
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
	SD = 0, SI = 1,
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
void ms25(){
	delay_ms(30);}		//easy way to call delay_ms(25)
void print_debug(char dbg){
	fputc(dbg,PC);
}
	
//***********Fona Fns***********//
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
void wake_EVR(){				//put device to sleep and wake on whistle or received a character
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
char recSIcmd(char _SIgroup){		//starts recognition of SI commands or Custom Grammars
	char _SIrec = NUL;
	fputc(CMD_RECOG_SI,EVR);
	ms25();
	fputc(_SIgroup,EVR);
	ms25();
	if (kbhit(EVR)) _SIrec = fgetc(EVR);
	print_debug(_SIrec);
return _SIrec;				//this should return success or failure
}
int getSIcmd(char _getSIrec){			//gets the index of the SI commands or Custom Grammars
	char _SIcmd = NUL;
	fputc(ARG_ACK,EVR);
	ms25();
	if (kbhit(EVR)) _getSIrec = fgetc(EVR);
	_SIcmd = _getSIrec - ARG_ZERO;
	print_debug(_SIcmd);
return _SIcmd;
}
char recSDcmd(char _SDgroup){		//starts recognition of SD commands or Custom Commands
	char _SDrec = NUL;
	fputc(CMD_RECOG_SD,EVR);
	ms25();
	fputc(_SDgroup,EVR);
	ms25();
	if (kbhit(EVR)) _SDrec = fgetc(EVR);
	print_debug(_SDrec);
return _SDrec;
}
int getSDcmd(char _getSDrec){			//gets the index of the SD commands or Custom Commands
	char _SDcmd = NUL;
	fputc(ARG_ACK,EVR);
	ms25();
	if (kbhit(EVR)) _getSDrec = fgetc(EVR);
	_SDcmd = _getSDrec - ARG_ZERO;
	print_debug(_SDcmd);
return _SDcmd;
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
				if (kbhit(EVR)) _rec_play = fgetc(EVR);
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
				if (kbhit(EVR)) _rec_play = fgetc(EVR);
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
				if (kbhit(EVR)) _rec_play = fgetc(EVR);
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
				if (kbhit(EVR)) _rec_play = fgetc(EVR);
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
				if (kbhit(EVR)) _rec_play = fgetc(EVR);
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
				if (kbhit(EVR)) _rec_play = fgetc(EVR);
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
				if (kbhit(EVR)) _rec_play = fgetc(EVR);
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
				if (kbhit(EVR)) _rec_play = fgetc(EVR);
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
				if (kbhit(EVR)) _rec_play = fgetc(EVR);
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
				if (kbhit(EVR)) _rec_play = fgetc(EVR);
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
				if (kbhit(EVR)) _rec_play = fgetc(EVR);
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
				if (kbhit(EVR)) _rec_play = fgetc(EVR);
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
				ms25();
				if (kbhit(EVR)) _rec_play = fgetc(EVR);
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
				if (kbhit(EVR)) _rec_play = fgetc(EVR);
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
				if (kbhit(EVR)) _rec_play = fgetc(EVR);
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
				if (kbhit(EVR)) _rec_play = fgetc(EVR);
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
				if (kbhit(EVR)) _rec_play = fgetc(EVR);
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
				if (kbhit(EVR)) _rec_play = fgetc(EVR);
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
				if (kbhit(EVR)) _rec_play = fgetc(EVR);
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
				if (kbhit(EVR)) _rec_play = fgetc(EVR);
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
				if (kbhit(EVR)) _rec_play = fgetc(EVR);
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
				if (kbhit(EVR)) _rec_play = fgetc(EVR);
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
				if (kbhit(EVR)) _rec_play = fgetc(EVR);
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
				if (kbhit(EVR)) _rec_play = fgetc(EVR);
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
				if (kbhit(EVR)) _rec_play = fgetc(EVR);
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
				if (kbhit(EVR)) _rec_play = fgetc(EVR);
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
#DEVICE HIGH_INTS=TRUE
volatile unsigned int call_flag = FALSE;
#define RX_SIZE 20
volatile char rx_buff[RX_SIZE];
volatile char temp;
volatile int serial_idx = 0;
volatile int command_ready = FALSE;
#INT_EXT HIGH
void EXT_isr(void){
	temp = fgetc(GSM);
//if we got a CR, then the command is completed
	if ((temp == CR) && (serial_idx > 1)) command_ready = TRUE;
//save the character to the receive buffer
	rx_buff[serial_idx] = temp;
	++serial_idx;
//if the command is ready, then reset the buffer
	if (command_ready == TRUE) serial_idx = 0;
}

void bgets(char* s, unsigned int8 max){
	unsigned char _c;
	unsigned int8 len;
	unsigned int8 read_idx;
	unsigned int eos = 0;

	--max;
	len = 0;
	read_idx = 0;
	do{		
		_c = rx_buff[read_idx];
		if ((_c == CR) && (read_idx > 1)){
			eos = 1;
			s[len] = _c;
		}
		++read_idx;
		++len;
	}while(!eos);
	s[len] = 0;
	len = 0;
	serial_idx = 0;
	read_idx = 0;
}



//                   ========================                    //
//===================M A I N  F U N C T I O N====================//
//                   ========================                    //
void main(){
//************Setup*************
////some setup constants
	// char gsm_action;
	char c,k;
	char receive = NUL;
	
////some setup functions
 	halt();						//stop or wake up device
	setTimeout(ARG_FIVE);		//set the timeout for 5 seconds
	setLang(ARG_ZERO);			//set the language to english
	gsmSetup();
	
////setup for interrupts
	// ext_int_edge(H_TO_L);			//interrupt on falling edge of pin RB0
	// enable_interrupts(INT_EXT);
	// enable_interrupts(GLOBAL);
	
//*
//************Loop**************
while(TRUE){
	if (reset == TRUE){
		wake_EVR();		//this is the initial state	
		recType = 0;				//start recognition with SI commands
		groupSD = GROUP_1;				//start recognition with SD group 1
		groupSI = TRIGGER;				//start recognition with SI group 0 / trigger
		idxSD = 0;					//set SD command index to 0
		idxSI = 0;					//set SI command index to 0
	}	
	if (recType == SD){
		disable_interrupts(GLOBAL);
		playback(SND_hello_please_say_a_command);
		receive = recSDcmd(groupSD);
		if (receive == STS_SUCCESS){
			idxSD = getSDcmd(receive);	//this function gets the command index if the operation is successful (determined from receive variable)
			if (idxSD >= 0){
				actionSD();
			}
		}else
		if (receive == STS_TIMEOUT){	//in the case there is a timeout then the process should start over by resetting the variables
			blinkA0(2);
			recType = 0;				//start recognition with SI commands
			groupSD = ARG_ONE;			//start recognition with SD group 1
			groupSI = ARG_ZERO;			//start recognition with SI group 0 / trigger
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
	}else
	if (recType == SI){
		disable_interrupts(GLOBAL);
		receive = recSIcmd(groupSI);
		if (receive == STS_SUCCESS){
			idxSI = getSIcmd(receive);		//this function gets the command index if the operation is successful (determined from receive variable)
			if (idxSI >= 0){
				actionSI();
			}
		}else
		if (receive == STS_TIMEOUT){	//in the case there is a timeout then the process should start over by resetting the variables
			blinkA0(2);
			recType = 0;				//start recognition with SI commands
			groupSD = ARG_ONE;			//start recognition with SD group 1
			groupSI = ARG_ZERO;			//start recognition with SI group 0 / trigger
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
	}
}  //*/
}