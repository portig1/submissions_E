;
; Task2_Assembly.asm
;
; Created: 3/2/2019 7:28:44 AM
; Author : gausp
;


.org 0
	;Set PORTB.2 For Output
	SBI DDRB,2 
	LDI R16, 4
	OUT PORTB, R16

	;Set PORTC.2 For Input
	CBI DDRC, 2
	LDI R16, 4
	OUT PORTC, R16

	;If the switch connected to PINC.2 is pushed (Set to Low), PORTB.2 will be set to 0 and turn on the LED for 1.25s

L1:	
	SBIC PINC, 2	;If PINC isn't cleared (Switch is pushed), RJMP LEDoff is exectued and the LED will be turned off, if not already
	RJMP LEDoff
	LDI R16, 0
	OUT PORTB, R16
	RCALL delay1250ms
	RJMP L1
	LEDoff: LDI R16, 4
	OUT PORTB, R16
	RJMP L1

delay1250ms:
					;Delay function is meant to have a delay of approximately 435ms for a clock at 16MHz, actual time is about 437ms
					;Identical to delay function used in Task 1, clock cycles needed are 20,000,000 and require 165.8 iterations rounded up
	LDI R19, 166		
delayL0_A: LDI R20, 200
delayL1_A: LDI R21, 200
delayL2_A: DEC R21
	BRNE delayL2_A
	DEC R20
	BRNE delayL1_A
	DEC R19
	BRNE delayL0_A
	RET
