#line 1 "C:/Users/nitai/OneDrive/Documents/Year 3- ECNG 3020- Special Project/3. Designs/0. System Design Code/System Core/System Core.c"
#line 1 "c:/program files/mikroc/mikroc pro for pic/include/protocol.h"
#line 12 "C:/Users/nitai/OneDrive/Documents/Year 3- ECNG 3020- Special Project/3. Designs/0. System Design Code/System Core/System Core.c"
enum ErrorCode
{

 ERR_DATACOL_TOO_LONG = 0x02,
 ERR_DATACOL_TOO_NOISY = 0x03,
 ERR_DATACOL_TOO_SOFT = 0x04,
 ERR_DATACOL_TOO_LOUD = 0x05,
 ERR_DATACOL_TOO_SOON = 0x06,
 ERR_DATACOL_TOO_CHOPPY = 0x07,
 ERR_DATACOL_BAD_WEIGHTS = 0x08,
 ERR_DATACOL_BAD_SETUP = 0x09,


 ERR_RECOG_FAIL = 0x11,
 ERR_RECOG_LOW_CONF = 0x12,
 ERR_RECOG_MID_CONF = 0x13,
 ERR_RECOG_BAD_TEMPLATE = 0x14,
 ERR_RECOG_BAD_WEIGHTS = 0x15,
 ERR_RECOG_DURATION = 0x17,


 ERR_T2SI_EXCESS_STATES = 0x21,
 ERR_T2SI_BAD_VERSION = 0x22,
 ERR_T2SI_OUT_OF_RAM = 0x23,
 ERR_T2SI_UNEXPECTED = 0x24,
 ERR_T2SI_OVERFLOW = 0x25,
 ERR_T2SI_PARAMETER = 0x26,

 ERR_T2SI_NN_TOO_BIG = 0x29,
 ERR_T2SI_NN_BAD_VERSION = 0x2A,
 ERR_T2SI_NN_NOT_READY = 0x2B,
 ERR_T2SI_NN_BAD_LAYERS = 0x2C,

 ERR_T2SI_TRIG_OOV = 0x2D,
 ERR_T2SI_TOO_SHORT = 0x2F,


 ERR_RP_BAD_LEVEL = 0x31,
 ERR_RP_NO_MSG = 0x38,
 ERR_RP_MSG_EXISTS = 0x39,


 ERR_SYNTH_BAD_VERSION = 0x4A,
 ERR_SYNTH_ID_NOT_SET = 0x4B,
 ERR_SYNTH_TOO_MANY_TABLES = 0x4C,
 ERR_SYNTH_BAD_SEN = 0x4D,
 ERR_SYNTH_BAD_MSG = 0x4E,


 ERR_CUSTOM_NOTA = 0x80,
 ERR_CUSTOM_INVALID = 0x81,


 ERR_SW_STACK_OVERFLOW = 0xC0,
 ERR_INTERNAL_T2SI_BAD_SETUP = 0xCC,
};

enum wordSet1
{
 ACTION, MOVE, TURN, RUN, LOOK, ATTACK, STOP, HELLO,
};

enum wordset2
{
 LEFT, RIGHT, UP, DOWN, FORWARD, BACKWARD,
};

enum wordset3
{
 ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN
};



void blinkD1(int a)
{
 int i = 0;
 for (i; i < a; i++ )
 {
 LATD1_bit = 1;
 Delay_ms(50);
 LATD1_bit = 0;
 Delay_ms(100);
 }
}

void blinkD0(int a)
{
 int i = 0;
 for (i; i < a; i++ )
 {
 LATD0_bit = 1;
 Delay_ms(50);
 LATD0_bit = 0;
 Delay_ms(100);
 }
}

int halt()
{
 char rec =  0x00 ;
 UART1_Write( 'b' );
 Delay_ms(25);
 while(rec !=  'o' )
 {
 if (UART1_Data_Ready())
 {
 rec = UART1_Read();
 }
 }
 rec =  0x00 ;
 return  1 ;
}

void setLang(char i)
{
 char rec =  0x00 ;
 UART1_Write( 'l' );
 Delay_ms(25);
 UART1_Write(i);
 Delay_ms(25);
 while(rec !=  'o' )
 {
 if (UART1_Data_Ready())
 {
 rec = UART1_Read();
 }
 }
 rec =  0x00 ;
}

void timeout(char i)
{
 char rec =  0x00 ;
 UART1_Write( 'o' );
 Delay_ms(25);
 UART1_Write(i);
 Delay_ms(25);
 while(rec !=  'o' )
 {
 if (UART1_Data_Ready())
 {
 rec = UART1_Read();
 }
 }
 rec =  0x00 ;
}

int error()
{

}

int listenSI(char i)
{
 char receive =  0x00 ;
 char command =  0x00 ;
 UART1_Write( 'i' );
 Delay_ms(25);
 UART1_Write(i);
 Delay_ms(25);
 if (UART1_Data_Ready())
 {
 receive = UART1_Read();
 }
 if (receive ==  's' )
 {
 UART1_Write( 0x20 );
 Delay_ms(25);
 if (UART1_Data_Ready())
 {
 receive = UART1_Read();
 }
 command = receive -  0x41 ;
 switch (command)
 {
 case (ACTION):
 break;
 case (MOVE):
 break;
 case (TURN):
 break;
 case (RUN):
 break;
 case (LOOK):
 break;
 case (ATTACK):
 break;
 case (STOP):
 break;
 case (HELLO):
 break;
 default:
 break;
 }
 } else
 if (receive ==  't' )
 {
 blinkD0(1);
 return  1 ;
 } else
 if (receive ==  'e' )
 {
 error();
 }
receive =  0x00 ;
}




void MAIN()
{
 char receive =  0x00 ;
 char error;
 TRISD0_bit = 0;
 TRISD1_bit = 0;
 UART1_Init(9600);
 Delay_ms(100);
 error = Soft_UART_Init(&PORTD, 3, 2, 9600, 0);
 Delay_ms(100);

 halt();
 setLang( 0x41 );
 timeout( 0x46 );
 listenSI( 0x42 );

while(1)
{

}

}
