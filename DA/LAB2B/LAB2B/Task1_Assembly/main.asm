;
; Task1_Assembly.asm
;
; Created: 3/8/2019 6:57:38 PM
; Author : gausp
;


.ORG 0 ;location for reset
	JMP MAIN
.ORG 0x02 ;location for external interrupt 0
	JMP EX0_ISR
MAIN:
	LDI R20, HIGH(RAMEND)
	OUT SPH, R20
	LDI R20, LOW(RAMEND)
	OUT SPL, R20 ;initialize stack

	SBI DDRB, 2 ;PORTB.2 = output
	SBI PORTB, 2 ;Turn off LED (reverse logic)
	SBI PORTD, 2 ;pull-up activated

	LDI R20, 0x2 ;make INT0 falling edge triggered
	STS EICRA, R20

	LDI R20, 1<<INT0 ;enable INT0
	OUT EIMSK, R20
	SEI ;enable interrupts

HERE:JMP HERE

EX0_ISR:
	CBI PORTB, 2
	RCALL delay1250ms
	SBI PORTB, 2
RETI

delay1250ms:
					;Taken from DA2A, Task 2
					;Delay function is meant to have a delay of approximately 1250ms for a clock at 16MHz
					;Delay function from delayL0 and below takes 120,603 clock cycles
					;Clock cycles total needed are 20,000,000 and require 165.8 iterations rounded up
					;However, when running on hardware the time it takes for the delay is about twice as long when the delay is in the
					;interrupt. To mitigate this I have halved the 166 to 83 and that gives about 1.250s
	LDI R19, 83		
delayL0_A: LDI R20, 200
delayL1_A: LDI R21, 200
delayL2_A: DEC R21
	BRNE delayL2_A
	DEC R20
	BRNE delayL1_A
	DEC R19
	BRNE delayL0_A
	RET