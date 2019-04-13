; Print.s
; Student names: change this to your names or look very silly
; Last modification date: change this to the last modification date or look very silly
; Runs on LM4F120 or TM4C123
; EE319K lab 7 device driver for any LCD
;
; As part of Lab 7, students need to implement these LCD_OutDec and LCD_OutFix
; This driver assumes two low-level LCD functions
; ST7735_OutChar   outputs a single 8-bit ASCII character
; ST7735_OutString outputs a null-terminated string 

    IMPORT   ST7735_OutChar
    IMPORT   ST7735_OutString
    EXPORT   LCD_OutDec
    EXPORT   LCD_OutFix

    AREA    |.text|, CODE, READONLY, ALIGN=2
    THUMB

  

;-----------------------LCD_OutDec-----------------------
; Output a 32-bit number in unsigned decimal format
; Input: R0 (call by value) 32-bit unsigned number
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutDec
n 	EQU 0
		CMP R0, #10
		BLO Base
		PUSH {R0, LR}  
		MOV R2, #10
		UDIV R0, R2
		BL LCD_OutDec
		LDR R0, [SP,#n]
		MOV R2, #10
		UDIV R1, R0, R2
		MUL	 R1, R2
		SUB  R0, R1
		STR R0, [SP,#n]
		ADD R0, #0x30
		BL ST7735_OutChar
		POP	{R0, PC}
	
Base	PUSH {R0, LR}
		ADD R0, #0x30
		BL ST7735_OutChar
		POP {R0, PC}
        
;* * * * * * * * End of LCD_OutDec * * * * * * * *

; -----------------------LCD _OutFix----------------------
; Output characters to LCD display in fixed-point format
; unsigned decimal, resolution 0.001, range 0.000 to 9.999
; Inputs:  R0 is an unsigned 32-bit number
; Outputs: none
; E.g., R0=0,    then output "0.000 "
;       R0=3,    then output "0.003 "
;       R0=89,   then output "0.089 "
;       R0=123,  then output "0.123 "
;       R0=9999, then output "9.999 "
;       R0>9999, then output "*.*** "
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutFix
VAL 	EQU 0
		PUSH {R0, LR}	
		SUB SP, #8
		
		MOV R1, #9999
		CMP R0, R1
		BHI stars
		STR R0, [SP,#VAL]
		
		MOV R2, #1000
		LDR R3, [SP, #VAL]
		UDIV R1,R3,R2
		ADD R0, R1, #0x30
		MUL R1, R2
		SUB R3, R1
		STR R3, [SP, #VAL]
		BL ST7735_OutChar
		
		
		MOV R0, #0x2E
		BL ST7735_OutChar
		
		MOV R2, #100
		LDR R3, [SP, #VAL]
		UDIV R1,R3,R2
		ADD R0, R1, #0x30
		MUL R1, R2
		SUB R3, R1
		STR R3, [SP, #VAL]
		BL ST7735_OutChar
		
		MOV R2, #10
		LDR R3, [SP, #VAL]
		UDIV R1,R3,R2
		ADD R0, R1, #0x30
		MUL R1, R2
		SUB R3, R1
		STR R3, [SP, #VAL]
		BL ST7735_OutChar
		
		LDR R0, [SP, #VAL]
		ADD R0,#0x30
		BL ST7735_OutChar 
		
		B done
		
stars 	MOV R0,#0x2A
		BL ST7735_OutChar 
		MOV R0, #0x2E
		BL ST7735_OutChar
		MOV R0,#0x2A
		BL ST7735_OutChar
		MOV R0,#0x2A
		BL ST7735_OutChar
		MOV R0,#0x2A
		BL ST7735_OutChar
		
done    ADD SP, #8
		POP {R0, LR}
		BX   LR
		
		
 
     ALIGN
;* * * * * * * * End of LCD_OutFix * * * * * * * *

     ALIGN                           ; make sure the end of this section is aligned
     END                             ; end of file

