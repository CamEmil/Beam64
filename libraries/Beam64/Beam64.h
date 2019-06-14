/*
	Beam64Lib - Library for bidirectional communication with an
	N64 controller and an N64 through the controller port. 

	Important Notes:
		- CURRENTLY TIED TO PIN 7 BY DEFAULT 
			- This is because I am using the sbic and sbis (skip if bit is clear/set for checking the PORTD register)
		- Turn off interrupts while transmitting and receiving data
		- The library currently only works for pins 0-7
		- Code is designed for the ATmega328p processor
		- Direct port register manipulation is used
		- The library has currently only been tested on an arduino nano

	Functions are provided to:
		- Send commands to a controller
		- Respond to commands from an N64
		- Tie the commands to a pin


*/

#ifndef Beam64_h
#define Beam64_h

#if (ARDUINO >= 100)
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class Beam64 {
	public:
		Beam64(char pin, bool displayContext=false);
		char readByte();
		char readCommand();
		char readCommand7Bits();
		void writeByte(unsigned char data);
		void writeByteStop(unsigned char data);

		void sendCommandStatus();
		void sendCommandPoll();

	private:
		char _pin;
		bool _displayContext;

		void delayOneMicro();
		void delayTwoMicros();
		void delayThreeMicros();

		void sendZero();
		void sendOne();
		void sendStop();

		void setPinIn();
		void setPinOut();
		void writePinHigh();
		void writePinLow();

};

#endif