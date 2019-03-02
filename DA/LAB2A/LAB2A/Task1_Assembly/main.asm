;
; Task1_Assembly.asm
;
; Created: 2/28/2019 8:05:33 PM
; Author : gausp
;


.org 0
	;Set PORTB.2 For Output
	SBI DDRB,2 
	LDI R16, 0
	OUT PORTB, R16

	; Toggle PORTB.2 on and off. On for 435ms and off for 290ms to achieve a period of 725ms with a duty cycle of 60%
	; The LED connected to PORTB is active low so by setting PORTB.2 to 0, it is actually being turned on 
L1:	LDI R16, 0
	OUT PORTB, R16
	RCALL delay435ms
	LDI R16, 4
	OUT PORTB, R16
	RCALL delay290ms
	RJMP L1

delay435ms:
			;Delay function is meant to have a delay of approximately 435ms for a clock at 16MHz, actual time is about 437ms
			;Delay function from delayL0 and below takes 120,603 clock cycles
			;So using the clock's ratio of 16 x 10^3 calculations per second and knowing that the period we need is 0.435s
			;I found the clock cycles needed to be 6,960,000. 57.7 Iterations of the delay loop is needed so I rounded up to 58
	LDI R19, 58		
delayL0_A: LDI R20, 200
delayL1_A: LDI R21, 200
delayL2_A: DEC R21
	BRNE delayL2_A
	DEC R20
	BRNE delayL1_A
	DEC R19
	BRNE delayL0_A
	RET

delay290ms:
			;Delay function is meant to have a delay of approximately 290ms for a clock at 16MHz but is closer to about 294ms
			;Process is identical to that of the function above. Clock cycles needed are 4,640,000 and require 38.5 iterations rounded up
	LDI R19, 39		
delayL0_B: LDI R20, 200
delayL1_B: LDI R21, 200
delayL2_B: DEC R21
	BRNE delayL2_B
	DEC R20
	BRNE delayL1_B
	DEC R19
	BRNE delayL0_B
	RET