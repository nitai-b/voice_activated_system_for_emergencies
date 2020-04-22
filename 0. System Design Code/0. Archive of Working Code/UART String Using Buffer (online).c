#include <16F877.H> 
#fuses XT, NOWDT, NOPROTECT, BROWNOUT, PUT, NOLVP 
#use delay(clock=4000000) 
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7, ERRORS) 

#define BUFFER_SIZE 32 
BYTE buffer[BUFFER_SIZE]; 
BYTE next_in = 0; 
BYTE next_out = 0; 


#int_rda 
void serial_isr() { 
   int t; 

   buffer[next_in]=getc(); 
   t=next_in; 
   next_in=(next_in+1) % BUFFER_SIZE; 
   if(next_in==next_out) 
     next_in=t;           // Buffer full !! 
} 

#define bkbhit (next_in!=next_out) 

BYTE bgetc() { 
   BYTE c; 

   while(!bkbhit) ; 
   c=buffer[next_out]; 
   next_out=(next_out+1) % BUFFER_SIZE; 
   return(c); 
} 


void my_get_string(char* s, unsigned int8 max) { 
   unsigned int8 len; 
   char c; 

   --max; 
   len=0; 
   do { 
     //c=getc();  // Commented out. 
       c = bgetc();   // Call bgetc() instead of getc() 
      
       if(c==8) {  // Backspace 
        if(len>0) { 
          len--; 
          putc(c); 
          putc(' '); 
          putc(c); 
        } 
     } else if ((c>=' ')&&(c<='~')) 
       if(len<=max) { 
         s[len++]=c; 
         putc(c); 
       } 
   } while(c!=13); 
   s[len]=0; 
} 

#define STRING_SIZE 40 

//=================================== 
void main() 
{ 
char input_str[STRING_SIZE]; 

enable_interrupts(int_rda); 
enable_interrupts(global); 

while(1) 
  {  
   printf("Enter a string of text: "); 
   my_get_string(input_str, STRING_SIZE-1);      
   printf("\n\rYou typed: %s \n\r\n\r", input_str); 

  } 

}