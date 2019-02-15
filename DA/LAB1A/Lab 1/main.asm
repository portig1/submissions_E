;
; Lab 1.asm
;
; Created: 2/10/2019 5:31:18 AM
; Author : gausp
;


.include<m328pbdef.inc>
.cseg
.org 0x00
Task1:				;Itterative addition to multiply a 8-bit number to a 16-bit number

	LDI R22, 50		;Setting 8-bit multipler in R2, range is from 0-255
	LDI R24, 50		;Setting lower 8-bits of 16-bit multiplicand
	LDI R25, 0x00	;Setting higher 8-bits of 16-bit multiplicand
AddLoop:
	BREQ Task2		;Checks to see if the counter (
	ADD R18, R24	;Adds multiplicand to result register, specifically the lowest 8-bits when considering that result as 24-bits from R18 to R20 
	ADC R19, R25	;Adds next 8-bits of multiplicand to the register as well as adding a carry onto it, if there is one
	ADC R20, R12	;R12 remains as 0x00 and is used to add a carry with the highest 8-bits should there be one from adding R25 to R19
	DEC R22			;Uses the multiplier as a counter for how many times the multiplicand needs to be added to itself
	jmp AddLoop

Task2:				;Verify the algorithm using a mul instruction

	LDI R16, 50		;Setting test multiplicand values
	LDI R17, 50
	MUL R16, R17
	MOV R10, R0		;Result stored in R10:R11
	MOV R11, R1

end: rjmp end
