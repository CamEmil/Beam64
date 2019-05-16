#include "Beam64lib.h"

#define NOP __asm__ ("nop\n\t")

#define PRINT_DEBUG(debug) Serial.println(debug, HEX)

#define SEND_ONE 		PIN_7_LOW; \
						delayOneMicro(); \
						PIN_7_HIGH; \
						delayThreeMicros();

#define SEND_ZERO 		PIN_7_LOW; \
						delayThreeMicros(); \
						PIN_7_HIGH; \
						delayOneMicro();

#define SEND_STOP_BIT	PIN_7_LOW; \
						delayOneMicro(); \
						PIN_7_HIGH; \
						delayTwoMicros();


/*
*	Pin cannot be a pin that supports PWM
*	Ensure interrupts are disabled 
*
*
*
*/
Beam64lib::Beam64lib(char pin, bool displayContext) {

}

char Beam64lib::readByte() {

	
}

void delayOneMicro() {
	NOP; NOP; NOP; NOP;
	NOP; NOP;
}

void delayTwoMicros() {
	NOP; NOP; NOP; NOP;
	NOP; NOP; NOP; NOP;
	NOP; NOP; NOP; NOP;
	NOP; NOP; NOP; NOP;

	NOP; NOP; NOP; NOP;
	NOP; NOP; NOP; NOP;
	NOP; NOP; NOP; NOP;
	NOP; NOP; NOP; NOP;
}

void delayThreeMicros() {
	NOP; NOP; NOP; NOP;
	NOP; NOP; NOP; NOP;
	NOP; NOP; NOP; NOP;
	NOP; NOP; NOP; NOP;

	NOP; NOP; NOP; NOP;
	NOP; NOP; NOP; NOP;
	NOP; NOP; NOP; NOP;
	NOP; NOP; NOP; NOP;

	NOP; NOP; NOP; NOP;
	NOP; NOP; NOP; NOP;
	NOP; NOP;
}