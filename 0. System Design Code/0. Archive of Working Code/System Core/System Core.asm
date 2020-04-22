
_blinkD1:

;System Core.c,86 :: 		void blinkD1(int a)
;System Core.c,88 :: 		int i = 0;
	CLRF        blinkD1_i_L0+0 
	CLRF        blinkD1_i_L0+1 
;System Core.c,89 :: 		for (i; i < a; i++ )
L_blinkD10:
	MOVLW       128
	XORWF       blinkD1_i_L0+1, 0 
	MOVWF       R0 
	MOVLW       128
	XORWF       FARG_blinkD1_a+1, 0 
	SUBWF       R0, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__blinkD150
	MOVF        FARG_blinkD1_a+0, 0 
	SUBWF       blinkD1_i_L0+0, 0 
L__blinkD150:
	BTFSC       STATUS+0, 0 
	GOTO        L_blinkD11
;System Core.c,91 :: 		LATD1_bit = 1;
	BSF         LATD1_bit+0, BitPos(LATD1_bit+0) 
;System Core.c,92 :: 		Delay_ms(50);
	MOVLW       65
	MOVWF       R12, 0
	MOVLW       238
	MOVWF       R13, 0
L_blinkD13:
	DECFSZ      R13, 1, 1
	BRA         L_blinkD13
	DECFSZ      R12, 1, 1
	BRA         L_blinkD13
	NOP
;System Core.c,93 :: 		LATD1_bit = 0;
	BCF         LATD1_bit+0, BitPos(LATD1_bit+0) 
;System Core.c,94 :: 		Delay_ms(100);
	MOVLW       130
	MOVWF       R12, 0
	MOVLW       221
	MOVWF       R13, 0
L_blinkD14:
	DECFSZ      R13, 1, 1
	BRA         L_blinkD14
	DECFSZ      R12, 1, 1
	BRA         L_blinkD14
	NOP
	NOP
;System Core.c,89 :: 		for (i; i < a; i++ )
	INFSNZ      blinkD1_i_L0+0, 1 
	INCF        blinkD1_i_L0+1, 1 
;System Core.c,95 :: 		}
	GOTO        L_blinkD10
L_blinkD11:
;System Core.c,96 :: 		}
L_end_blinkD1:
	RETURN      0
; end of _blinkD1

_blinkD0:

;System Core.c,98 :: 		void blinkD0(int a)
;System Core.c,100 :: 		int i = 0;
	CLRF        blinkD0_i_L0+0 
	CLRF        blinkD0_i_L0+1 
;System Core.c,101 :: 		for (i; i < a; i++ )
L_blinkD05:
	MOVLW       128
	XORWF       blinkD0_i_L0+1, 0 
	MOVWF       R0 
	MOVLW       128
	XORWF       FARG_blinkD0_a+1, 0 
	SUBWF       R0, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__blinkD052
	MOVF        FARG_blinkD0_a+0, 0 
	SUBWF       blinkD0_i_L0+0, 0 
L__blinkD052:
	BTFSC       STATUS+0, 0 
	GOTO        L_blinkD06
;System Core.c,103 :: 		LATD0_bit = 1;
	BSF         LATD0_bit+0, BitPos(LATD0_bit+0) 
;System Core.c,104 :: 		Delay_ms(50);
	MOVLW       65
	MOVWF       R12, 0
	MOVLW       238
	MOVWF       R13, 0
L_blinkD08:
	DECFSZ      R13, 1, 1
	BRA         L_blinkD08
	DECFSZ      R12, 1, 1
	BRA         L_blinkD08
	NOP
;System Core.c,105 :: 		LATD0_bit = 0;
	BCF         LATD0_bit+0, BitPos(LATD0_bit+0) 
;System Core.c,106 :: 		Delay_ms(100);
	MOVLW       130
	MOVWF       R12, 0
	MOVLW       221
	MOVWF       R13, 0
L_blinkD09:
	DECFSZ      R13, 1, 1
	BRA         L_blinkD09
	DECFSZ      R12, 1, 1
	BRA         L_blinkD09
	NOP
	NOP
;System Core.c,101 :: 		for (i; i < a; i++ )
	INFSNZ      blinkD0_i_L0+0, 1 
	INCF        blinkD0_i_L0+1, 1 
;System Core.c,107 :: 		}
	GOTO        L_blinkD05
L_blinkD06:
;System Core.c,108 :: 		}
L_end_blinkD0:
	RETURN      0
; end of _blinkD0

_halt:

;System Core.c,110 :: 		int halt()//this stops the current process and can be used to wake the module, returns TRUE OR '1' if the process is successful
;System Core.c,112 :: 		char rec = null;
	CLRF        halt_rec_L0+0 
;System Core.c,113 :: 		UART1_Write(CMD_BREAK);
	MOVLW       98
	MOVWF       FARG_UART1_Write_data_+0 
	CALL        _UART1_Write+0, 0
;System Core.c,114 :: 		Delay_ms(25);
	MOVLW       33
	MOVWF       R12, 0
	MOVLW       118
	MOVWF       R13, 0
L_halt10:
	DECFSZ      R13, 1, 1
	BRA         L_halt10
	DECFSZ      R12, 1, 1
	BRA         L_halt10
	NOP
;System Core.c,115 :: 		while(rec != STS_SUCCESS)
L_halt11:
	MOVF        halt_rec_L0+0, 0 
	XORLW       111
	BTFSC       STATUS+0, 2 
	GOTO        L_halt12
;System Core.c,117 :: 		if (UART1_Data_Ready())
	CALL        _UART1_Data_Ready+0, 0
	MOVF        R0, 1 
	BTFSC       STATUS+0, 2 
	GOTO        L_halt13
;System Core.c,119 :: 		rec = UART1_Read();
	CALL        _UART1_Read+0, 0
	MOVF        R0, 0 
	MOVWF       halt_rec_L0+0 
;System Core.c,120 :: 		}
L_halt13:
;System Core.c,121 :: 		}
	GOTO        L_halt11
L_halt12:
;System Core.c,122 :: 		rec = null;
	CLRF        halt_rec_L0+0 
;System Core.c,123 :: 		return TRUE; //return 1 if there is success in stopping
	MOVLW       1
	MOVWF       R0 
	MOVLW       0
	MOVWF       R1 
;System Core.c,124 :: 		}
L_end_halt:
	RETURN      0
; end of _halt

_setLang:

;System Core.c,126 :: 		void setLang(char i)//the argument is based on the communication protocol and the respective language numbering
;System Core.c,128 :: 		char rec = null;
	CLRF        setLang_rec_L0+0 
;System Core.c,129 :: 		UART1_Write(CMD_LANGUAGE);
	MOVLW       108
	MOVWF       FARG_UART1_Write_data_+0 
	CALL        _UART1_Write+0, 0
;System Core.c,130 :: 		Delay_ms(25);
	MOVLW       33
	MOVWF       R12, 0
	MOVLW       118
	MOVWF       R13, 0
L_setLang14:
	DECFSZ      R13, 1, 1
	BRA         L_setLang14
	DECFSZ      R12, 1, 1
	BRA         L_setLang14
	NOP
;System Core.c,131 :: 		UART1_Write(i);
	MOVF        FARG_setLang_i+0, 0 
	MOVWF       FARG_UART1_Write_data_+0 
	CALL        _UART1_Write+0, 0
;System Core.c,132 :: 		Delay_ms(25);
	MOVLW       33
	MOVWF       R12, 0
	MOVLW       118
	MOVWF       R13, 0
L_setLang15:
	DECFSZ      R13, 1, 1
	BRA         L_setLang15
	DECFSZ      R12, 1, 1
	BRA         L_setLang15
	NOP
;System Core.c,133 :: 		while(rec != STS_SUCCESS)
L_setLang16:
	MOVF        setLang_rec_L0+0, 0 
	XORLW       111
	BTFSC       STATUS+0, 2 
	GOTO        L_setLang17
;System Core.c,135 :: 		if (UART1_Data_Ready())
	CALL        _UART1_Data_Ready+0, 0
	MOVF        R0, 1 
	BTFSC       STATUS+0, 2 
	GOTO        L_setLang18
;System Core.c,137 :: 		rec = UART1_Read();
	CALL        _UART1_Read+0, 0
	MOVF        R0, 0 
	MOVWF       setLang_rec_L0+0 
;System Core.c,138 :: 		}
L_setLang18:
;System Core.c,139 :: 		}
	GOTO        L_setLang16
L_setLang17:
;System Core.c,140 :: 		rec = null;
	CLRF        setLang_rec_L0+0 
;System Core.c,141 :: 		}
L_end_setLang:
	RETURN      0
; end of _setLang

_timeout:

;System Core.c,143 :: 		void timeout(char i)//the argument based on the communication protocol sets the respective time numbering
;System Core.c,145 :: 		char rec = null;
	CLRF        timeout_rec_L0+0 
;System Core.c,146 :: 		UART1_Write(CMD_TIMEOUT);
	MOVLW       111
	MOVWF       FARG_UART1_Write_data_+0 
	CALL        _UART1_Write+0, 0
;System Core.c,147 :: 		Delay_ms(25);
	MOVLW       33
	MOVWF       R12, 0
	MOVLW       118
	MOVWF       R13, 0
L_timeout19:
	DECFSZ      R13, 1, 1
	BRA         L_timeout19
	DECFSZ      R12, 1, 1
	BRA         L_timeout19
	NOP
;System Core.c,148 :: 		UART1_Write(i);
	MOVF        FARG_timeout_i+0, 0 
	MOVWF       FARG_UART1_Write_data_+0 
	CALL        _UART1_Write+0, 0
;System Core.c,149 :: 		Delay_ms(25);
	MOVLW       33
	MOVWF       R12, 0
	MOVLW       118
	MOVWF       R13, 0
L_timeout20:
	DECFSZ      R13, 1, 1
	BRA         L_timeout20
	DECFSZ      R12, 1, 1
	BRA         L_timeout20
	NOP
;System Core.c,150 :: 		while(rec != STS_SUCCESS)
L_timeout21:
	MOVF        timeout_rec_L0+0, 0 
	XORLW       111
	BTFSC       STATUS+0, 2 
	GOTO        L_timeout22
;System Core.c,152 :: 		if (UART1_Data_Ready())
	CALL        _UART1_Data_Ready+0, 0
	MOVF        R0, 1 
	BTFSC       STATUS+0, 2 
	GOTO        L_timeout23
;System Core.c,154 :: 		rec = UART1_Read();
	CALL        _UART1_Read+0, 0
	MOVF        R0, 0 
	MOVWF       timeout_rec_L0+0 
;System Core.c,155 :: 		}
L_timeout23:
;System Core.c,156 :: 		}
	GOTO        L_timeout21
L_timeout22:
;System Core.c,157 :: 		rec = null;
	CLRF        timeout_rec_L0+0 
;System Core.c,158 :: 		}
L_end_timeout:
	RETURN      0
; end of _timeout

_error:

;System Core.c,160 :: 		int error()
;System Core.c,163 :: 		}
L_end_error:
	RETURN      0
; end of _error

_listenSI:

;System Core.c,165 :: 		int listenSI(char i)//the argument is based on which wordset is selected for recognition
;System Core.c,167 :: 		char receive = null;
	CLRF        listenSI_receive_L0+0 
	CLRF        listenSI_command_L0+0 
;System Core.c,169 :: 		UART1_Write(CMD_RECOG_SI);
	MOVLW       105
	MOVWF       FARG_UART1_Write_data_+0 
	CALL        _UART1_Write+0, 0
;System Core.c,170 :: 		Delay_ms(25);
	MOVLW       33
	MOVWF       R12, 0
	MOVLW       118
	MOVWF       R13, 0
L_listenSI24:
	DECFSZ      R13, 1, 1
	BRA         L_listenSI24
	DECFSZ      R12, 1, 1
	BRA         L_listenSI24
	NOP
;System Core.c,171 :: 		UART1_Write(i);//start recognition in wordset number 1 based on the argument passed into the function
	MOVF        FARG_listenSI_i+0, 0 
	MOVWF       FARG_UART1_Write_data_+0 
	CALL        _UART1_Write+0, 0
;System Core.c,172 :: 		Delay_ms(25);
	MOVLW       33
	MOVWF       R12, 0
	MOVLW       118
	MOVWF       R13, 0
L_listenSI25:
	DECFSZ      R13, 1, 1
	BRA         L_listenSI25
	DECFSZ      R12, 1, 1
	BRA         L_listenSI25
	NOP
;System Core.c,173 :: 		if (UART1_Data_Ready())// If data is received,
	CALL        _UART1_Data_Ready+0, 0
	MOVF        R0, 1 
	BTFSC       STATUS+0, 2 
	GOTO        L_listenSI26
;System Core.c,175 :: 		receive = UART1_Read();//store it in receive
	CALL        _UART1_Read+0, 0
	MOVF        R0, 0 
	MOVWF       listenSI_receive_L0+0 
;System Core.c,176 :: 		}
L_listenSI26:
;System Core.c,177 :: 		if (receive == STS_SIMILAR)//if there is successful recognition
	MOVF        listenSI_receive_L0+0, 0 
	XORLW       115
	BTFSS       STATUS+0, 2 
	GOTO        L_listenSI27
;System Core.c,179 :: 		UART1_Write(ARG_ACK);//send acknowledgement
	MOVLW       32
	MOVWF       FARG_UART1_Write_data_+0 
	CALL        _UART1_Write+0, 0
;System Core.c,180 :: 		Delay_ms(25);
	MOVLW       33
	MOVWF       R12, 0
	MOVLW       118
	MOVWF       R13, 0
L_listenSI28:
	DECFSZ      R13, 1, 1
	BRA         L_listenSI28
	DECFSZ      R12, 1, 1
	BRA         L_listenSI28
	NOP
;System Core.c,181 :: 		if (UART1_Data_Ready())// If data is received,
	CALL        _UART1_Data_Ready+0, 0
	MOVF        R0, 1 
	BTFSC       STATUS+0, 2 
	GOTO        L_listenSI29
;System Core.c,183 :: 		receive = UART1_Read();//store it in receive
	CALL        _UART1_Read+0, 0
	MOVF        R0, 0 
	MOVWF       listenSI_receive_L0+0 
;System Core.c,184 :: 		}
L_listenSI29:
;System Core.c,185 :: 		command = receive - ARG_ZERO;//subtract A to find the command spoken
	MOVLW       65
	SUBWF       listenSI_receive_L0+0, 0 
	MOVWF       listenSI_command_L0+0 
;System Core.c,186 :: 		switch (command)
	GOTO        L_listenSI30
;System Core.c,188 :: 		case (ACTION):
L_listenSI32:
;System Core.c,189 :: 		break;
	GOTO        L_listenSI31
;System Core.c,190 :: 		case (MOVE):
L_listenSI33:
;System Core.c,191 :: 		break;
	GOTO        L_listenSI31
;System Core.c,192 :: 		case (TURN):
L_listenSI34:
;System Core.c,193 :: 		break;
	GOTO        L_listenSI31
;System Core.c,194 :: 		case (RUN):
L_listenSI35:
;System Core.c,195 :: 		break;
	GOTO        L_listenSI31
;System Core.c,196 :: 		case (LOOK):
L_listenSI36:
;System Core.c,197 :: 		break;
	GOTO        L_listenSI31
;System Core.c,198 :: 		case (ATTACK):
L_listenSI37:
;System Core.c,199 :: 		break;
	GOTO        L_listenSI31
;System Core.c,200 :: 		case (STOP):
L_listenSI38:
;System Core.c,201 :: 		break;
	GOTO        L_listenSI31
;System Core.c,202 :: 		case (HELLO):
L_listenSI39:
;System Core.c,203 :: 		break;
	GOTO        L_listenSI31
;System Core.c,204 :: 		default:
L_listenSI40:
;System Core.c,205 :: 		break;
	GOTO        L_listenSI31
;System Core.c,206 :: 		}
L_listenSI30:
	MOVF        listenSI_command_L0+0, 0 
	XORLW       0
	BTFSC       STATUS+0, 2 
	GOTO        L_listenSI32
	MOVF        listenSI_command_L0+0, 0 
	XORLW       1
	BTFSC       STATUS+0, 2 
	GOTO        L_listenSI33
	MOVF        listenSI_command_L0+0, 0 
	XORLW       2
	BTFSC       STATUS+0, 2 
	GOTO        L_listenSI34
	MOVF        listenSI_command_L0+0, 0 
	XORLW       3
	BTFSC       STATUS+0, 2 
	GOTO        L_listenSI35
	MOVF        listenSI_command_L0+0, 0 
	XORLW       4
	BTFSC       STATUS+0, 2 
	GOTO        L_listenSI36
	MOVF        listenSI_command_L0+0, 0 
	XORLW       5
	BTFSC       STATUS+0, 2 
	GOTO        L_listenSI37
	MOVF        listenSI_command_L0+0, 0 
	XORLW       6
	BTFSC       STATUS+0, 2 
	GOTO        L_listenSI38
	MOVF        listenSI_command_L0+0, 0 
	XORLW       7
	BTFSC       STATUS+0, 2 
	GOTO        L_listenSI39
	GOTO        L_listenSI40
L_listenSI31:
;System Core.c,207 :: 		} else
	GOTO        L_listenSI41
L_listenSI27:
;System Core.c,208 :: 		if (receive == STS_TIMEOUT)//if there is a timeout
	MOVF        listenSI_receive_L0+0, 0 
	XORLW       116
	BTFSS       STATUS+0, 2 
	GOTO        L_listenSI42
;System Core.c,210 :: 		blinkD0(1);
	MOVLW       1
	MOVWF       FARG_blinkD0_a+0 
	MOVLW       0
	MOVWF       FARG_blinkD0_a+1 
	CALL        _blinkD0+0, 0
;System Core.c,211 :: 		return TRUE;//then return TRUE (a value of 1)
	MOVLW       1
	MOVWF       R0 
	MOVLW       0
	MOVWF       R1 
	GOTO        L_end_listenSI
;System Core.c,212 :: 		} else
L_listenSI42:
;System Core.c,213 :: 		if (receive == STS_ERROR)
	MOVF        listenSI_receive_L0+0, 0 
	XORLW       101
	BTFSS       STATUS+0, 2 
	GOTO        L_listenSI44
;System Core.c,215 :: 		error();
	CALL        _error+0, 0
;System Core.c,216 :: 		}
L_listenSI44:
L_listenSI41:
;System Core.c,217 :: 		receive = null;
	CLRF        listenSI_receive_L0+0 
;System Core.c,218 :: 		}
L_end_listenSI:
	RETURN      0
; end of _listenSI

_MAIN:

;System Core.c,223 :: 		void MAIN()
;System Core.c,225 :: 		char receive = null;
	CLRF        MAIN_receive_L0+0 
;System Core.c,227 :: 		TRISD0_bit = 0;                                                            //Set port D pin 0 as output
	BCF         TRISD0_bit+0, BitPos(TRISD0_bit+0) 
;System Core.c,228 :: 		TRISD1_bit = 0;                                                            //set port D pin 1 as output
	BCF         TRISD1_bit+0, BitPos(TRISD1_bit+0) 
;System Core.c,229 :: 		UART1_Init(9600);                                                          // Initialize UART module at 9600 bps
	BSF         BAUDCON+0, 3, 0
	CLRF        SPBRGH+0 
	MOVLW       103
	MOVWF       SPBRG+0 
	BSF         TXSTA+0, 2, 0
	CALL        _UART1_Init+0, 0
;System Core.c,230 :: 		Delay_ms(100);                                                             // Wait for UART module to stabilize
	MOVLW       130
	MOVWF       R12, 0
	MOVLW       221
	MOVWF       R13, 0
L_MAIN45:
	DECFSZ      R13, 1, 1
	BRA         L_MAIN45
	DECFSZ      R12, 1, 1
	BRA         L_MAIN45
	NOP
	NOP
;System Core.c,231 :: 		error = Soft_UART_Init(&PORTD, 3, 2, 9600, 0);                             // Initialize Soft UART at 9600 bps
	MOVLW       PORTD+0
	MOVWF       FARG_Soft_UART_Init_port+0 
	MOVLW       hi_addr(PORTD+0)
	MOVWF       FARG_Soft_UART_Init_port+1 
	MOVLW       3
	MOVWF       FARG_Soft_UART_Init_rx_pin+0 
	MOVLW       2
	MOVWF       FARG_Soft_UART_Init_tx_pin+0 
	MOVLW       128
	MOVWF       FARG_Soft_UART_Init_baud_rate+0 
	MOVLW       37
	MOVWF       FARG_Soft_UART_Init_baud_rate+1 
	MOVLW       0
	MOVWF       FARG_Soft_UART_Init_baud_rate+2 
	MOVWF       FARG_Soft_UART_Init_baud_rate+3 
	CLRF        FARG_Soft_UART_Init_inverted+0 
	CALL        _Soft_UART_Init+0, 0
	MOVF        R0, 0 
	MOVWF       MAIN_error_L0+0 
;System Core.c,232 :: 		Delay_ms(100);                                                             // wait for software UART to stabalize
	MOVLW       130
	MOVWF       R12, 0
	MOVLW       221
	MOVWF       R13, 0
L_MAIN46:
	DECFSZ      R13, 1, 1
	BRA         L_MAIN46
	DECFSZ      R12, 1, 1
	BRA         L_MAIN46
	NOP
	NOP
;System Core.c,234 :: 		halt();
	CALL        _halt+0, 0
;System Core.c,235 :: 		setLang(ARG_ZERO); //selects the english language
	MOVLW       65
	MOVWF       FARG_setLang_i+0 
	CALL        _setLang+0, 0
;System Core.c,236 :: 		timeout(ARG_FIVE);
	MOVLW       70
	MOVWF       FARG_timeout_i+0 
	CALL        _timeout+0, 0
;System Core.c,237 :: 		listenSI(ARG_ONE);
	MOVLW       66
	MOVWF       FARG_listenSI_i+0 
	CALL        _listenSI+0, 0
;System Core.c,239 :: 		while(1)
L_MAIN47:
;System Core.c,242 :: 		}//end     of neverending loop
	GOTO        L_MAIN47
;System Core.c,244 :: 		}
L_end_MAIN:
	GOTO        $+0
; end of _MAIN
