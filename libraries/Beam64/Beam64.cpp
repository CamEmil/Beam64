#include "Beam64.h"

#define NOP __asm__ ("nop\n\t")


/*
*	Pin must be pin 7
*	Ensure interrupts are disabled 
*
*
*
*/

Beam64::Beam64(char pin, bool displayContext) {
	_displayContext = _displayContext;

	// _pin is one-hot representation of the pin number for port register manipulation
	_pin = (1 << pin);
}

char Beam64::readCommand() {
	setPinIn();
	char* byte = 0x00;

	// r20-27 are scratch
	// put byte result in 
	// r20 is counter for delay
	// r21 is counter for each bit
	// put pin number into r22

	__asm__(
		"ldi r21, 8			\n"
	"waitforlow:		\n"
		"sbic 9, 7  		\n"
		"rjmp waitforlow	\n"

		"ldi r20, 6			\n"
	"wait24cycles:		\n"
		"nop				\n"
		"dec r20			\n"
		"cpi r20, 0			\n"
		"brne wait24cycles	\n"

		"lsl r0				\n"
		"sbis 9, 7  		\n"
		"rjmp waitforhigh	\n"
		"inc r0				\n"
		"rjmp deccounter	\n"

	"waitforhigh:			\n"
		"sbis 9, 7			\n"
		"rjmp waitforhigh	\n"

	"deccounter:		\n"
		"dec r21			\n"
		"cpi r21, 0			\n"
		"brne waitforlow	\n"

		"ldi r20, 9			\n"
	"wait10cycles:			\n"
		"nop				\n"	
		"dec r20			\n"
		"cpi r20, 0			\n"
		"brne wait10cycles	\n\t"

	);

	return *byte;
}

char Beam64::readByte() {
	setPinIn();
	char* byte = 0x00;

	// r20-27 are scratch
	// put byte result in 
	// r20 is counter for delay
	// r21 is counter for each bit
	// put pin number into r22

	__asm__(
		"ldi r21, 8			\n"
		"waitforlow:		\n"
		"sbic 9, 7  		\n"
		"rjmp waitforlow	\n"

		"ldi r20, 6			\n"
		"wait24cycles:		\n"
		"nop				\n"
		"dec r20			\n"
		"cpi r20, 0			\n"
		"brne wait24cycles	\n"

		"lsl r0				\n"
		"sbis 9, 7  		\n"
		"rjmp waitforhigh	\n"
		"inc r0				\n"
		"rjmp deccounter	\n"

		"waitforhigh:			\n"
		"sbis 9, 7			\n"
		"rjmp waitforhigh	\n"

		"deccounter:		\n"
		"dec r21			\n"
		"cpi r21, 0			\n"
		"brne waitforlow	\n\t"
	);

	return *byte;
}

void Beam64::writeByte(unsigned char data) {
	//setPinOut();

	char* regPointer = 0x10;

	*regPointer = data;

	// r18-27 are scratch
	// put byte result in 
	// r20 is counter for delay
	// r21 is counter for each bit
	// r25 is the byte parameter

	__asm__(
		"ldi r21, 8			\n"
	"waitforstart:			\n"
		"sbis 9, 7  		\n"
		"rjmp waitforstart	\n"
	"startofbit:			\n"	
		"cbi 11, 7			\n"

		"ldi r20, 4			\n"
	"wait16cycles:			\n"
		"dec r20			\n"
		"cpi r20, 0			\n"
		"brne wait16cycles	\n"
		
		"sbrc r16, 7  		\n"
		"sbi 11, 7			\n"
		"nop				\n"	
		"ldi r20, 7			\n"
	"wait32cycles:			\n"
		"dec r20			\n"
		"cpi r20, 0			\n"
		"brne wait32cycles	\n"

		"sbrs r16, 7  		\n"
		"sbi 11, 7			\n"
		"lsl r16			\n"

		"ldi r20, 2			\n"
	"wait16cycles2:			\n"
		"dec r20			\n"
		"cpi r20, 0			\n"
		"brne wait16cycles2	\n"

		"dec r21			\n"
		"cpi r21, 0			\n"
		"brne startofbit	\n\t"
		
	);

}

void Beam64::writeByteStop(unsigned char data) {
	//setPinOut();

	// using r16 as data reg
	char* regPointer = 0x10;

	*regPointer = data;

	// r18-27 are scratch
	// put byte result in 
	// r20 is counter for delay
	// r21 is counter for each bit
	// r25 is the byte parameter

	__asm__(
		"ldi r21, 8			\n"
	"waitforstart1:			\n"
		"sbis 9, 7  		\n"
		"rjmp waitforstart1	\n"
	"startofbit1:			\n"
		"cbi 11, 7			\n"

		"ldi r20, 4			\n"
	"wait16cycles1:			\n"
		"dec r20			\n"
		"cpi r20, 0			\n"
		"brne wait16cycles1	\n"

		"sbrc r16, 7  		\n"
		"sbi 11, 7			\n"
		"nop				\n"
		"ldi r20, 7			\n"
	"wait32cycles1:			\n"
		"dec r20			\n"
		"cpi r20, 0			\n"
		"brne wait32cycles1	\n"

		"sbrs r16, 7  		\n"
		"sbi 11, 7			\n"
		"lsl r16			\n"

		"ldi r20, 2			\n"
	"wait16cycles21:			\n"
		"dec r20			\n"
		"cpi r20, 0			\n"
		"brne wait16cycles21	\n"

		"dec r21			\n"
		"cpi r21, 0			\n"
		"brne startofbit1	\n"

		"cbi 11, 7			\n"
		"ldi r20, 6			\n"
	"wait21:			\n"
		"nop				\n"
		"dec r20			\n"
		"cpi r20, 0			\n"
		"brne wait21			\n"

		"sbi 11, 7			\n\t"

	);

}

void Beam64::setPinIn() {
	DDRD = DDRD & _pin;
}

void Beam64::setPinOut() {
	DDRD = DDRD | _pin;
}

void Beam64::writePinHigh() {
	PORTD = _pin;
}

void Beam64::writePinLow() {
	PORTD = 0b00000000;
}

void Beam64::sendCommandStatus() {
	setPinOut();
	writePinHigh();

	sendZero(); sendZero(); sendZero(); sendZero();
	sendZero(); sendZero(); sendZero(); sendZero();
	sendStop();

	setPinIn();
	writePinHigh();
}

void Beam64::sendCommandPoll() {
	setPinOut();
	writePinHigh();

	sendZero(); sendZero(); sendZero(); sendZero();
	sendZero(); sendZero(); sendZero(); sendOne();
	sendStop();

	setPinIn();
	writePinHigh();
}

void Beam64::delayOneMicro() {
	NOP; NOP; NOP; NOP;
	NOP; NOP;
}

void Beam64::delayTwoMicros() {
	NOP; NOP; NOP; NOP;
	NOP; NOP; NOP; NOP;
	NOP; NOP; NOP; NOP;
	NOP; NOP; NOP; NOP;

	NOP; NOP; NOP; NOP;
	NOP; NOP; NOP; NOP;
	NOP; NOP; NOP; NOP;
	NOP; NOP; NOP; NOP;
}

void Beam64::delayThreeMicros() {
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

void Beam64::sendOne() {
	writePinLow(); 
	delayOneMicro(); 
	writePinHigh(); 
	delayThreeMicros();
}

void Beam64::sendZero() {
	writePinLow(); 
	delayThreeMicros(); 
	writePinHigh(); 
	delayOneMicro();
}

void Beam64::sendStop() {
	writePinLow(); 
	delayOneMicro(); 
	writePinHigh(); 
	delayTwoMicros();
}
