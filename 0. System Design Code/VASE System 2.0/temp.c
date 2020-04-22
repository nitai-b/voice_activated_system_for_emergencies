//                    =======================                    //
//====================I N T R O D U C T I O N====================//
//                    =======================                    //
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
#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,stream=EVR,bits=8)		//HW serial pins EasyVR	||	EVR
#use rs232(baud=9600,parity=N,xmit=PIN_C4,rcv=PIN_B0,stream=GSM,bits=8,SAMPLE_EARLY)		//SW serial pins Fona	||	GSM
#use rs232(baud=9600,parity=N,xmit=PIN_A4,rcv=PIN_A3,stream=PC,bits=8)		//SW serial pins Debug	||	DB
#include <input.c> //this file must be ONLY included after #use rs232



//                       =================                       //
//=======================C O N S T A N T S=======================//
//                       =================                       //
//********Variable Def**********//
 	char AT[] = "AT\r\n";// To initialize mode
	char save[] = "AT&W\r\n";
	char noecho[] = "ATE0\r\n";
	char mode_text[] = "AT+CMGF=1\r\n";   // to set text mode
	char endCall[] = "ATH\r\n";
	char ansCall[] = "ATA\r\n";
	char dial[20] = "ATD+1868";
	char end[2] = ";";

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
/*enum trigger0{				//constants for trigger
	Robot,
};*/
enum wordSet1{					//constants for wordset1
	WS1_ACTION, WS1_MOVE, WS1_TURN, WS1_RUN, WS1_LOOK, WS1_ATTACK, WS1_STOP, WS1_HELLO,
};
/*enum wordset2{				//constants for wordset2
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
 */
enum group1{					//constants for action words
	G1_CALL, G1_ANSWER, G1_LIGHTS, G1_LED, G1_OTHER, G1_END
};
enum group2{					//constants for target of action words
	G2_DOCTOR, G2_PARENT, G2_MOM, G2_DAD, G2_SON, G2_DAUGHTER, G2_ON, G2_OFF, G2_OTHER,
};
enum group3{
	G3_ZERO, G3_ONE, G3_TWO, G3_THREE, G3_FOUR, G3_FIVE, G3_SIX, G3_SEVEN, G3_EIGHT, G3_NINE, G3_TEN
};



//                   ========================                    //
//===================S T A T E  M A C H I N E====================//
//                   ========================                    //
//						    DEFINITIONS				    		 //
enum states {	//State definitions
	STATE1, STATE2, STATE3, S_GP2, S_NUMBERS,
};
enum events {	//Event definitions 
	E_LIS_WS1, E_LIS_GP1, E_LIS_GP2, E_LIS_NUMS, E_CALL_SENT, E_INCOMING_CALL, E_ANS_CALL, E_END_CALL, E_TASK_FINISHED, E_END,
};
//a golbal variable for sending messages to another state
//particularly messages from GP1 to GP2
volatile int messages;
const char error = NUL;



//                   ========================                    //
//===================F U N C T I O N  D E F N====================//
//                   ========================                    //
//**********System Fns**********//
/* might not need these blinking functions
void blinkA0(int _a){			//LED indicator
	int _i = 0;
	for (_i; _i < _a; _i++ ){
		output_high(PIN_A0);
		delay_ms(50);
		output_low(PIN_A0);
		delay_ms(100);
	}
} */
void blinkD0(int _a){			//LED indicator
	int _i = 0;	
	for (_i; _i < _a; _i++ ){
		output_high(PIN_D0);
		delay_ms(50);
		output_low(PIN_D0);
		delay_ms(100);
	}
} 
void blinkD2(int _a){			//LED indicator
	int _i = 0;	
	for (_i; _i < _a; _i++ ){
		output_high(PIN_D2);
		delay_ms(100);
		output_low(PIN_D2);
		delay_ms(200);
	}
} 
void ms25(){
	delay_ms(30);}		//easy way to call ms25(25)
void print_debug(char dbg){
	fputc(dbg,PC);
}
	
//***********Fona Fns***********//
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
//NOTE: we do not necessarily need to check to see if the data received is "OK". we can assume that it will happen
	fprintf(GSM,AT);
	delay_us(50);
	fprintf(GSM,noecho);
	delay_us(50);
	fprintf(GSM,mode_text);
	delay_us(50);
	fprintf(GSM,save);
	delay_us(50);
	
}
/* void gsmAction(char _a){
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
} */
//***********EVR Fns***********//
void playback(int _playback){//plays back custom sound
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
void stop_EVR(){				//stop current action
	char _rec_halt = NUL;
	while (_rec_halt != STS_SUCCESS){
		fputc(CMD_BREAK,EVR);
		ms25();
		if (kbhit(EVR)) _rec_halt = fgetc(EVR);
		print_debug(_rec_halt);
	}
	_rec_halt = NUL;
}
void go_to_sleep(){				//put device to sleep and wake on double clap or received a character
	char _rec_sleep = NUL;
	//this here is to put device to sleep and set it to wake on double clap
	while (_rec_sleep != STS_SUCCESS){
		fputc(CMD_SLEEP,EVR);
		print_debug(CMD_SLEEP);
		ms25();
		fputc(ARG_THREE,EVR);
		print_debug(ARG_THREE);
		ms25();
		_rec_sleep = fgetc(EVR);
		print_debug(_rec_sleep);
	}
}
int wait(){						//waits to be waken by the user
	// char rec_wait;
	char back_from_power_down = NUL;
	int event_wake = 0;
	//this here is to wait until the user activates it by double clapping.
	while (back_from_power_down != STS_AWAKEN){
		back_from_power_down = fgetc(EVR);
		print_debug(back_from_power_down);
	}
	// playback(SND_one);
	back_from_power_down = NUL;
	// rec_wait = NUL;
	event_wake = E_LIS_WS1;
	print_debug(event_wake);
	return event_wake;
}
void setLang(char _whatLang){	//set the language based on the argument passed
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
void setTimeout(char _period){	//set a period of the timeout based on the argument passed
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

//***********State Fns**********//
/* int listenWS1(){	//listens to wordset1 and determines which word was spoken
	char rec_WS1 = NUL;
	char cmd_WS1 = NUL;
	int event_WS1 = 0;
	fputc(CMD_RECOG_SI,EVR);
	ms25();
	fputc(ARG_ONE,EVR);
	ms25();
	output_high(PIN_A0);	//indicate that the device is listening
	if (kbhit()) rec_WS1 = fgetc(EVR);
	output_low(PIN_A0);		//indicate that the device has stopped listening
	if (rec_WS1 == STS_SIMILAR){
		fputc(ARG_ACK,EVR);
		ms25();
		rec_WS1 = NUL;
		rec_WS1 = fgetc(EVR);
		cmd_WS1 = rec_WS1 - ARG_ZERO;
		print_debug(cmd_WS1);
		switch (cmd_WS1){
			case WS1_ACTION:
					event_WS1 = E_LIS_GP1;
				break;
			case WS1_MOVE:
					event_WS1 = E_LIS_GP1;
				break;
			case WS1_TURN:
					event_WS1 = E_LIS_GP1;
				break;
			case WS1_RUN:
					event_WS1 = E_LIS_GP1;
				break;
			case WS1_LOOK:
					event_WS1 = E_LIS_GP1;
				break;
			case WS1_ATTACK:
					event_WS1 = E_LIS_GP1;
				break;
			case WS1_STOP:
					event_WS1 = E_LIS_GP1;
				break;
			case WS1_HELLO:
					event_WS1 = E_LIS_GP1;
				break;
			default:	
					event_WS1 = E_LIS_GP1;
				break;
		}
		return event_WS1;
	}else
	if (rec_WS1 == STS_TIMEOUT){
			blinkD2(3); //timeout indicator
			event_WS1 = E_LIS_WS1; //restart listening procedure
		return event_WS1;
	}else
	if (rec_WS1 == STS_ERROR){
			blinkD2(3); //error indicator
			event_WS1 = E_LIS_WS1; //restart listening procedure
		return event_WS1;
	}
}
 */
/*void listenWS2(){	//listens to wordset2 and determines which word was spoken
	char rec_WS2 = NUL;
	char cmd_WS2 = NUL;
	fputc(CMD_RECOG_SI,EVR);
	ms25();
	fputc(ARG_TWO,EVR);
	ms25();
	output_high(PIN_A0);	//indicate that the device is listening
	if (kbhit()) rec_WS2 = fgetc(EVR);
	output_low(PIN_A0);		//indicate that the device has stopped listening
	if (rec_WS2 == STS_SIMILAR){
		fputc(ARG_ACK,EVR);
		ms25();
		rec_WS2 = NUL;
		rec_WS2 = fgetc(EVR);
		cmd_WS2 = rec_WS2 - ARG_ZERO;
		switch (cmd_WS2){
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
			default:	rec_WS2 = STS_SIMILAR;
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
void listenWS3(){	//listens to wordset3 and determines which word was spoken
	char rec_WS3 = NUL;
	char cmd_WS3 = NUL;
	fputc(CMD_RECOG_SI,EVR);
	ms25();
	fputc(ARG_THREE,EVR);
	ms25();
	output_high(PIN_A0);	//indicate that the device is listening
	if (kbhit()) rec_WS3 = fgetc(EVR);
	output_low(PIN_A0);		//indicate that the device has stopped listening
	if (rec_WS3 == STS_SIMILAR){
		fputc(ARG_ACK,EVR);
		ms25();
		rec_WS3 = NUL;
		rec_WS3 = fgetc(EVR);
		cmd_WS3 = rec_WS3 - ARG_ZERO;
		switch (cmd_WS3){
			case WS3_ZERO:
				break;
			case WS3_ONE:
				blinkA0(1);
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
*/
/* int listenGP1(){	//listens to group1 and determines which word was spoken
	char rec_GP1 = NUL;
	char cmd_GP1 = NUL;
	int event_GP1 = 0;
	fputc(CMD_RECOG_SD,EVR);
	ms25();
	fputc(ARG_ONE,EVR);
	ms25();
	output_high(PIN_A0);//indicate that the device is listening
	if (kbhit()) rec_GP1 = fgetc(EVR);
	output_low(PIN_A0);		//indicate that the device has stopped listening
	if (rec_GP1 == STS_SIMILAR){
		fputc(ARG_ACK,EVR);
		ms25();
		rec_GP1 = NUL;
		rec_GP1 = fgetc(EVR);
		cmd_GP1 = rec_GP1 - ARG_ZERO;
		switch (cmd_GP1){
			case G1_CALL:
					event_GP1 = E_LIS_GP2;
				break;
			case G1_ANSWER:
					fprintf(GSM,ansCall); //send a command to the GSM module to answer the call
					blinkD2(1);
					event_GP1 = E_LIS_GP2;
				break;
			case G1_LIGHTS:
					event_GP1 = E_LIS_GP2;
					messages = G1_LIGHTS;
					//go to group 2 to perform action
				break;
			case G1_LED:
					event_GP1 = E_LIS_GP2;
					messages = G1_LED;
					//go to group 2 to perform action
				break;
			case G1_OTHER:
					event_GP1 = E_LIS_GP2;
					//go to group 2 to perform action
				break;
			case G1_END:
					fprintf(GSM,endCall); //send a command to the GSM module to end the call
					event_GP1 = E_END;
				break;
			default:
					event_GP1 = E_LIS_GP2;
				break;
		}
		return event_GP1;
	}else
	if (rec_GP1 == STS_TIMEOUT){
			blinkD2(3); //timeout indicator
			event_GP1 = E_LIS_GP1; //restart listening procedure
		return event_GP1;
	}else
	if (rec_GP1 == STS_ERROR){
			blinkD2(3); //error indicator
			event_GP1 = E_LIS_GP1; //restart listening procedure
		return event_GP1;
	}

}
int listenGP2(){	//listens to group2 and determines which word was spoken
	char rec_GP2 = NUL;
	char cmd_GP2 = NUL;
	int event_GP2 = 0;
	fputc(CMD_RECOG_SD,EVR);
	ms25();
	fputc(ARG_TWO,EVR);
	ms25();
	output_high(PIN_A0);//indicate that the device is listening
	if (kbhit()) rec_GP2 = fgetc(EVR);
	output_low(PIN_A0);		//indicate that the device has stopped listening
	if (rec_GP2 == STS_SIMILAR){
		fputc(ARG_ACK,EVR);
		ms25();
		rec_GP2 = NUL;
		rec_GP2 = fgetc(EVR);
		cmd_GP2 = rec_GP2 - ARG_ZERO;
		switch (cmd_GP2){
			case G2_DOCTOR:
					playback(SND_calling);
					blinkD2(2);
					event_GP2 = E_CALL_SENT;
				break;
			case G2_PARENT:
					playback(SND_calling);
					blinkD2(2);
					event_GP2 = E_CALL_SENT;
				break;
			case G2_MOM:
					playback(SND_calling);
					blinkD2(2);
					event_GP2 = E_CALL_SENT;
				break;
			case G2_DAD:
					playback(SND_calling);
					blinkD2(2);
					event_GP2 = E_CALL_SENT;
				break;
			case G2_SON:
					playback(SND_calling);
					blinkD2(2);
					event_GP2 = E_CALL_SENT;
				break;
			case G2_DAUGHTER:
					playback(SND_calling);
					blinkD2(2);
					event_GP2 = E_CALL_SENT;
				break;
			case G2_ON:
					if (messages == G1_LIGHTS){
						playback(SND_turning_on_light_bulb);
						output_high(PIN_D2);
					}
					else
					if (messages == G1_LED){
						playback(SND_turning_on_LED);
						output_high(PIN_A1);
					}
					messages = 0; //clear messages
					event_GP2 = E_TASK_FINISHED;
				break;
			case G2_OFF:
					if (messages == G1_LIGHTS){
						playback(SND_turning_off_light_bulb);
						output_low(PIN_D2);
					}
					else
					if (messages == G1_LED){
						playback(SND_turning_off_LED);
						output_low(PIN_A1);
					}
					messages = 0; //clear messages
					event_GP2 = E_TASK_FINISHED;
				break;
			case G2_OTHER:
					event_GP2 = E_LIS_NUMS;
				break;
			default:
					event_GP2 = E_TASK_FINISHED;
				break;
		}
		return event_GP2;
	}else
	if (rec_GP2 == STS_TIMEOUT){
			blinkD2(3);
			event_GP2 = E_LIS_GP2;
		return event_GP2;
	}else
	if (rec_GP2 == STS_ERROR){
			blinkD2(3);
			event_GP2 = E_LIS_GP2;
		return event_GP2;
	}
}
int listenGP3(){	//listens to group3 and determines which word was spoken
	char rec_GP3 = NUL;
	char cmd_GP3 = NUL;
	int number;
	fputc(CMD_RECOG_SD,EVR);
	ms25();
	fputc(ARG_THREE,EVR);
	ms25();
	output_high(PIN_A0);	//indicate that the device is listening
	if (kbhit()) rec_GP3 = fgetc(EVR);
	output_low(PIN_A0);		//indicate that the device has stopped listening
	if (rec_GP3 == STS_SIMILAR){
		fputc(ARG_ACK,EVR);
		ms25();
		rec_GP3 = NUL;
		rec_GP3 = fgetc(EVR);
		cmd_GP3 = rec_GP3 - ARG_ZERO;
		switch (cmd_GP3){
			case G3_ZERO:
				number = 0x30;
				break;
			case G3_ONE:
				number = 0x31;
				break;
			case G3_TWO:
				number = 0x32;
				break;
			case G3_THREE:
				number = 0x33;
				break;
			case G3_FOUR:
				number = 0x34;
				break;
			case G3_FIVE:
				number = 0x35;
				break;
			case G3_SIX:
				number = 0x36;
				break;
			case G3_SEVEN:
				number = 0x37;
				break;
			case G3_EIGHT:
				number = 0x38;
				break;
			case G3_NINE:
				number = 0x39;
				break;
			// case G3_TEN:
				// number = 10;
				// break;
			default:
				number = error;
				break;
		}
		return number;
	}else
	if (rec_GP3 == STS_TIMEOUT){
		return error;
	}else
	if (rec_GP3 == STS_ERROR){
		return error;
	}
}
 */

//***********State Fns**********// these are some new functions based on what I realised last night
char listenWS1(){
	char rec_WS1 = NUL;
	fputc(CMD_RECOG_SI,EVR);
	ms25();
	fputc(ARG_ONE,EVR);
	ms25();
	output_high(PIN_D2);	//indicate that the device is listening
	if (kbhit()) rec_WS1 = fgetc(EVR);
	output_low(PIN_D2);		//indicate that the device has stopped listening
	return rec_WS1;
}

char listenGP1(){
	char rec_GP1 = NUL;
	fputc(CMD_RECOG_SD,EVR);
	ms25();
	fputc(ARG_ONE,EVR);
	ms25();
	output_high(PIN_D2);//indicate that the device is listening
	if (kbhit()) rec_GP1 = fgetc(EVR);
	output_low(PIN_D2);		//indicate that the device has stopped listening
	return rec_GP1;
}
char listenGP2(){
	char rec_GP2 = NUL;
	fputc(CMD_RECOG_SD,EVR);
	ms25();
	fputc(ARG_TWO,EVR);
	ms25();
	output_high(PIN_D2);//indicate that the device is listening
	if (kbhit()) rec_GP2 = fgetc(EVR);
	output_low(PIN_D2);		//indicate that the device has stopped listening
	return rec_GP2;
}
char listenGP3(){
	char rec_GP3 = NUL;
	fputc(CMD_RECOG_SD,EVR);
	ms25();
	fputc(ARG_THREE,EVR);
	ms25();
	output_high(PIN_D2);	//indicate that the device is listening
	if (kbhit()) rec_GP3 = fgetc(EVR);
	output_low(PIN_D2);		//indicate that the device has stopped listening
	return rec_GP3;
}



//                   ========================                    //
//===================S T A T E  M A C H I N E====================//
//                   ========================                    //
//						     STRUCTURE	     					 //
/* void StateMachine(){
	int current_state = S_WAIT;
	int event_outcome;
	char ph_number[8];
	int num_idx = 0;
	int count;
	char temp;
	
	switch(current_state){
		case S_WAIT:
				event_outcome = wait();
				switch(event_outcome){
					case E_LIS_WS1:
							current_state = S_WS1;
						break;
					case E_ANS_CALL:
							current_state = S_GP1;
						break;
					case E_END_CALL:
							current_state = S_GP1;
						break;
					default:
						blinkD2(4); //severe error if this happens, will restart this state
						current_state = S_WAIT;
						break;
				}
			break;
		case S_WS1:
				event_outcome = listenWS1();
				switch(event_outcome){
					case E_LIS_GP1:
						current_state = S_GP2;
						break;
					default:
						blinkD2(4); //severe error if this happens, will restart this state
						current_state = S_WS1;
						break;
				}
			break;
		case S_GP1:
				event_outcome = listenGP1();
				switch(event_outcome){
					case E_LIS_GP2:
						current_state = S_GP2;
						break;
					case E_END:
						current_state = S_WAIT;
						break;
					default:
						blinkD2(4); //severe error if this happens, will restart this state
						current_state = S_GP1;
						break;
				}
			break;
		case S_GP2:
				event_outcome = listenGP2();
				switch(event_outcome){
					case E_TASK_FINISHED:
						current_state = S_WAIT;
						break;
					case E_CALL_SENT:
						current_state = S_WAIT;
						break;
					case E_LIS_NUMS:
						current_state = S_NUMBERS;
						break;
					default:
						blinkD2(4); //severe error if this happens, will restart this state
						current_state = S_GP2;
						break;
				}
			break;
		case S_NUMBERS:
				for (count = 0; count == 6; count++){
					temp = listenGP3();
					ph_number[num_idx] = temp;
				}
				ph_number[7] = "\0"; //add the null character 
				strcat(dial,ph_number);
				strcat(dial,end);
				fputs(dial,GSM);
				count = 0; //reset the counter variable
				event_outcome = E_CALL_SENT;
			break;
	}
}
 */


//                      =================                       //
//======================I N T E R U P T S=======================//
//                      =================                       //
volatile int incoming_call = FALSE;
#INT_EXT
void detect_call(void){
	enable_interrupts(INT_TIMER1);
	incoming_call = TRUE;
}

#INT_EXT1
void button2(void){
	int16 b = 0;
	disable_interrupts(INT_TIMER1);
	fputs("ATD+18686822015;",GSM);
	for (b;b<500;b++)delay_cycles(255);
}

#INT_EXT2
void button1(void){ // answering and ending calls
	int16 a = 0;
	switch (incoming_call){
		case TRUE:
			disable_interrupts(INT_TIMER1);
			fputs("ATA\r\n",GSM);
			incoming_call = FALSE; // clear the incoming flag
		break;
		case FALSE:
			disable_interrupts(INT_TIMER1);
			fputs("ATH\r\n",GSM);
		break;
	}
	for (a;a<500;a++)delay_cycles(255);
}

#INT_TIMER0
void live(){
	output_toggle(PIN_A0);
}

#INT_TIMER1
void call_signal(void){
	int16 c = 0;
	output_high(PIN_D0); //pulse buzzer for incoming call
	for (c;c<1000;c++)delay_cycles(255);
	output_low(PIN_D0);
	// output_toggle(PIN_A0);
}



//                            =======                           //
//============================M A I N===========================//
//                            =======                           //
void main(){
//INTERRUPT SETUP	
	SET_TRIS_B(0x0F);				// set lower half of port B as input
	ext_int_edge(H_TO_L);			//
	ext_int_edge(1,H_TO_L);			// set up the trigger level for interrupts
	ext_int_edge(2,H_TO_L);			//
	
	clear_interrupt(INT_EXT);		//
	clear_interrupt(INT_EXT1);		// clear interrupts on start up...so no more problems would happen
	clear_interrupt(INT_EXT2);		//
	clear_interrupt(INT_TIMER0);	//
	clear_interrupt(INT_TIMER1);	//
	
	// setup_timer_0(RTCC_INTERNAL|RTCC_DIV_64);//1.6 s overflow
	setup_timer_0(RTCC_INTERNAL|RTCC_DIV_32);//840 ms overflow
	setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);//209 ms overflow
	enable_interrupts(INT_TIMER0);
	enable_interrupts(INT_EXT);		//
	enable_interrupts(INT_EXT1);	// enable all the external interrupts
	enable_interrupts(INT_EXT2);	//
	enable_interrupts(GLOBAL);		// enable golbal interrutps
	
//SOME VARIABLES
	char listening_status = NUL;
	char command_is = NUL;
	char receive = NUL;
	int test = FALSE;
	
	int count = 0;
	char ph_number[8];
	
//GSM SETUP
	gsmSetup();
	
//VOICE RECOG SETUP
	stop_EVR();
	setLang(ARG_ONE);//set language recog to english
	setTimeout(ARG_FIVE);//set timeout to be 5 seconds
	
	
//PROGRAM FLOW
while(TRUE){
	
	// go_to_sleep();
	// wait();
//start of first state
if (test == STATE1){
	//calling function to listen to wordset 1
 	listening_status = listenWS1();
	switch(listening_status){
		case STS_SIMILAR: // this must be used to for listening for SI commands
		fputc(ARG_ACK,EVR);
		ms25();
		receive = fgetc(EVR);
		command_is = receive - ARG_ZERO;
		switch (command_is){
			case WS1_ACTION:
					// event_WS1 = E_LIS_GP1;
				break;
			case WS1_MOVE:
					// event_WS1 = E_LIS_GP1;
				break;
			case WS1_TURN:
					// event_WS1 = E_LIS_GP1;
				break;
			case WS1_RUN:
					// event_WS1 = E_LIS_GP1;
				break;
			case WS1_LOOK:
					// event_WS1 = E_LIS_GP1;
				break;
			case WS1_ATTACK:
					// event_WS1 = E_LIS_GP1;
				break;
			case WS1_STOP:
					// event_WS1 = E_LIS_GP1;
				break;
			case WS1_HELLO:
					blinkD2(1);
					test = STATE2;
					// playback(SND_two); // responds to the user
					// blinkD2(4);
					// event_WS1 = E_LIS_GP1;
				break;
			default:
					playback(SND_please_say_a_command);
					// event_WS1 = E_LIS_GP1;
					test = STATE2;
				break;
		}
		break;
		case STS_TIMEOUT:
				blinkD2(2);
		break;
		case STS_ERROR:
				blinkD2(3);
		break;
	} 
}	
//end of first state
//start of second state
if (test == STATE2){
	// calling function to listen to group 1
	listening_status = listenGP1();
	switch(listening_status){
		case STS_RESULT: // this must be used for listening for SD commands
		fputc(ARG_ACK,EVR);
		ms25();
		receive = fgetc(EVR);
		command_is = receive - ARG_ZERO;
		switch (command_is){
			case G1_CALL:
					blinkD0(1);
					test = STATE3;
				break;
			case G1_ANSWER:					
					// blinkD2(1);
				break;
			case G1_LIGHTS:
					
				break;
			case G1_LED:
					
				break;
			case G1_OTHER:
					
				break;
			case G1_END:
					
				break;
			default:
					
				break;
		}
		break;
		case STS_TIMEOUT:
			blinkD0(2);
		break;
		case STS_ERROR:
			blinkD0(3);
		break;
	}
}
//end of second state
//start of third state
if (test == STATE3){	
	// calling function to listen to group 1
	listening_status = listenGP2();
	switch(listening_status){	
		case STS_RESULT: // this must be used for listening for SD comands
		fputc(ARG_ACK,EVR);
		ms25();
		receive = fgetc(EVR);
		command_is = receive - ARG_ZERO;
		switch (command_is){
			case G2_DOCTOR:
					// playback(SND_calling);
					// blinkD2(2);
					// event_GP2 = E_CALL_SENT;
				break;
			case G2_PARENT:
					// playback(SND_calling);
					// blinkD2(2);
					// event_GP2 = E_CALL_SENT;
				break;
			case G2_MOM:
					// playback(SND_calling);
					// blinkD2(2);
					// event_GP2 = E_CALL_SENT;
				break;
			case G2_DAD:
					// playback(SND_calling);
					// blinkD2(2);
					// event_GP2 = E_CALL_SENT;
				break;
			case G2_SON:
					// playback(SND_calling);
					// blinkD2(2);
					// event_GP2 = E_CALL_SENT;
				break;
			case G2_DAUGHTER:
					// playback(SND_calling);
					// blinkD2(2);
					// event_GP2 = E_CALL_SENT;
				break;
			case G2_ON:
					// if (messages == G1_LIGHTS){
						// playback(SND_turning_on_light_bulb);
						// output_high(PIN_D2);
					// }
					// else
					// if (messages == G1_LED){
						// playback(SND_turning_on_LED);
						// output_high(PIN_A1);
					// }
					// messages = 0; //clear messages
					// event_GP2 = E_TASK_FINISHED;
				break;
			case G2_OFF:
					// if (messages == G1_LIGHTS){
						// playback(SND_turning_off_light_bulb);
						// output_low(PIN_D2);
					// }
					// else
					// if (messages == G1_LED){
						// playback(SND_turning_off_LED);
						// output_low(PIN_A1);
					// }
					// messages = 0; //clear messages
					// event_GP2 = E_TASK_FINISHED;
				break;
			case G2_OTHER:
					blinkD2(1);
					blinkD0(1);
					fputs("ATD+18686822015;",GSM);
					// event_GP2 = E_LIS_NUMS;
					playback(SND_calling);
					for (count = 0; count == 6; count++){
							// calling function to listen to group 3
						listening_status = listenGP3();
						switch(listening_status){
							case STS_RESULT:
							fputc(ARG_ACK,EVR);
							ms25();
							receive = fgetc(EVR);
							command_is = receive - ARG_ZERO;
							switch (command_is){
								case G3_ZERO:
									playback(SND_zero);
									ph_number[count] = 0x30;									
									break;
								case G3_ONE:
									playback(SND_one);
									ph_number[count] = 0x31;
									break;
								case G3_TWO:
									playback(SND_two);
									ph_number[count] = 0x32;
									break;
								case G3_THREE:
									playback(SND_three);
									ph_number[count] = 0x33;
									break;
								case G3_FOUR:
									playback(SND_four);
									ph_number[count] = 0x34;
									break;
								case G3_FIVE:
									playback(SND_five);
									ph_number[count] = 0x35;
									break;
								case G3_SIX:
									playback(SND_six);
									ph_number[count] = 0x36;
									break;
								case G3_SEVEN:
									playback(SND_seven);
									ph_number[count] = 0x37;
									break;
								case G3_EIGHT:
									playback(SND_eight);
									ph_number[count] = 0x38;
									break;
								case G3_NINE:
									playback(SND_nine);
									ph_number[count] = 0x39;
									break;
								default:
									blinkD0(4);
									ph_number[count] = 0x00;
									break;
							}
							break;
							case STS_TIMEOUT:
								blinkD0(2);
							break;
							case STS_ERROR:
								blinkD0(3);
							break;
						}//end of listening to commands
					}
					ph_number[7] = 0x00; //add the null character
					strcat(dial,ph_number);
					strcat(dial,end);
					fputs(dial,GSM);
					count = 0;
					test = STATE1;
				break;
			default:
					blinkD2(4);
					// event_GP2 = E_TASK_FINISHED;
				break;
		}
		break;
		case STS_TIMEOUT:
			blinkD2(2);
		break;
		case STS_ERROR:
			blinkD2(3);
		break;
	}
}
//end of third state
}//end of while(TRUE) loop

}//<<end of MAIN FUNCTION