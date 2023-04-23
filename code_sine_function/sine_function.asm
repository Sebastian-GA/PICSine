
_main:

;sine_function.c,15 :: 		void main()
;sine_function.c,17 :: 		unsigned int deg = 0;
	CLRF       main_deg_L0+0
	CLRF       main_deg_L0+1
;sine_function.c,23 :: 		OSCCON = 0xF0;
	MOVLW      240
	MOVWF      OSCCON+0
;sine_function.c,24 :: 		TRISA = 0x00;  // Set PORTA as output
	CLRF       TRISA+0
;sine_function.c,25 :: 		ANSELA = 0x00; // Set PORTA as digital
	CLRF       ANSELA+0
;sine_function.c,27 :: 		PORTA.F4 = 1; // Deselect DAC chip
	BSF        PORTA+0, 4
;sine_function.c,28 :: 		SPI1_Init();
	CALL       _SPI1_Init+0
;sine_function.c,30 :: 		while (1)
L_main0:
;sine_function.c,32 :: 		valueDAC = 819 + (2866.5 * sin(0.0174533*deg)); // 1.0 + abs(3.5 * sin((PI/180)*deg))
	MOVF       main_deg_L0+0, 0
	MOVWF      R0
	MOVF       main_deg_L0+1, 0
	MOVWF      R1
	CALL       _word2double+0
	MOVLW      57
	MOVWF      R4
	MOVLW      250
	MOVWF      R5
	MOVLW      14
	MOVWF      R6
	MOVLW      121
	MOVWF      R7
	CALL       _Mul_32x32_FP+0
	MOVF       R0, 0
	MOVWF      FARG_sin_f+0
	MOVF       R1, 0
	MOVWF      FARG_sin_f+1
	MOVF       R2, 0
	MOVWF      FARG_sin_f+2
	MOVF       R3, 0
	MOVWF      FARG_sin_f+3
	CALL       _sin+0
	MOVLW      0
	MOVWF      R4
	MOVLW      40
	MOVWF      R5
	MOVLW      51
	MOVWF      R6
	MOVLW      138
	MOVWF      R7
	CALL       _Mul_32x32_FP+0
	MOVLW      0
	MOVWF      R4
	MOVLW      192
	MOVWF      R5
	MOVLW      76
	MOVWF      R6
	MOVLW      136
	MOVWF      R7
	CALL       _Add_32x32_FP+0
	CALL       _double2int+0
;sine_function.c,35 :: 		temp1 = (valueDAC >> 8) & 0x0F; // Store valueDAC[11..8] to temp[3..0]
	MOVF       R1, 0
	MOVWF      R2
	MOVLW      0
	BTFSC      R1, 7
	MOVLW      255
	MOVWF      R3
	MOVLW      15
	ANDWF      R2, 0
	MOVWF      FARG_SPI1_Write_data_+0
;sine_function.c,36 :: 		temp1 |= 0x30;                  // Define DAC setting, see MCP4921 datasheet
	MOVLW      48
	IORWF       FARG_SPI1_Write_data_+0, 1
;sine_function.c,38 :: 		temp2 = valueDAC; // Store valueDAC[7..0] to temp[7..0]
	MOVF       R0, 0
	MOVWF      main_temp2_L0+0
;sine_function.c,40 :: 		PORTA.F4 = 0;      // Select DAC chip
	BCF        PORTA+0, 4
;sine_function.c,41 :: 		SPI1_Write(temp1); // Send high byte via SPI
	CALL       _SPI1_Write+0
;sine_function.c,42 :: 		SPI1_Write(temp2); // Send low byte via SPI
	MOVF       main_temp2_L0+0, 0
	MOVWF      FARG_SPI1_Write_data_+0
	CALL       _SPI1_Write+0
;sine_function.c,43 :: 		PORTA.F4 = 1;      // Deselect DAC chip
	BSF        PORTA+0, 4
;sine_function.c,45 :: 		deg++;
	INCF       main_deg_L0+0, 1
	BTFSC      STATUS+0, 2
	INCF       main_deg_L0+1, 1
;sine_function.c,46 :: 		if (deg >= 180)
	MOVLW      0
	SUBWF      main_deg_L0+1, 0
	BTFSS      STATUS+0, 2
	GOTO       L__main4
	MOVLW      180
	SUBWF      main_deg_L0+0, 0
L__main4:
	BTFSS      STATUS+0, 0
	GOTO       L_main2
;sine_function.c,47 :: 		deg = 0;
	CLRF       main_deg_L0+0
	CLRF       main_deg_L0+1
L_main2:
;sine_function.c,48 :: 		}
	GOTO       L_main0
;sine_function.c,49 :: 		}
L_end_main:
	GOTO       $+0
; end of _main
