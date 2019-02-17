;
; LAB1B.asm
;
; Created: 2/16/2019 1:06:49 PM
; Author : gausp
;


.include<m328pdef.inc>

.cseg
.org 0x00

.DEF ZERO = R10			;Will be to add cary for the upper 8-bits of the sums
.DEF COUNTER = R25		;R25 will be used as the counter
.DEF COPYVALUE = R20
.DEF NUM = R21				;Defined variable for the division segment of code
.DEF DENOMINATOR = R22
.DEF QUOTIENT = R23

	  CLR ZERO	
	  CLR R16				;Clearing Registers R16:R19 since they will be used for the sum results
	  CLR R17
	  CLR R18
	  CLR R19
	  LDI	COUNTER ,0x63	;COUNTER = 99 
	  LDI	COPYVALUE,0x0A	;R20 = 10(initial value to be copied), will be incremented in till it is 109 but the last stored value will be 108

	  LDI	XL,LOW(0x200)	;load the low byte of X with value STARTADS = 0x0200
	  LDI	XH,HIGH(0x200)	;load the high byte of X with value STARTADS = 0x0200
	  LDI	YL,LOW(0x400)	;load the low byte of Y with 0x0400
	  LDI	YH,HIGH(0x400)	;load the high byte of Y with 0x0400
	  LDI	ZL,LOW(0x600)	;load the low byte of Z with  0x0600
	  LDI	ZH,HIGH(0x600)	;load the high byte of Z with 0x0600	

L1: 
	ST	X+, COPYVALUE		;Store R20 to memory location X
	MOV NUM, COPYVALUE		;Copy R20 to NUM
	LDI DENOMINATOR, 3
	DIVLOOP1: 
		INC QUOTIENT
		SUB NUM, DENOMINATOR
		BRCC DIVLOOP1
		DEC QUOTIENT
		ADD NUM, DENOMINATOR
	DIVCHECK:
		CPI NUM, 0
		BRNE NOTDIV3		;If NUM = 0, R20 is divisible by 3 and will be stored in Y
		ST Y+,COPYVALUE
		ADD R16, COPYVALUE	;Adds value that's divisible by 3 to the appropriate sum
		ADC R17, ZERO
		RJMP NEXT
	NOTDIV3:				;Stores value of R20 at Z since it is not divisible by 3
		ST Z+, COPYVALUE
		ADD R18, COPYVALUE	;Adds value that's not divisible by 3 to the appropriate sum
		ADC R19, ZERO
	NEXT:
	INC COPYVALUE			;Increment value to store
	DEC	COUNTER				;decrement the counter
	BRNE  L1				;loop until counter = zero

END: RJMP END
