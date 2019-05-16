#pragma once
#ifndef t1
#define t1

if (ARDUINO >= 100)
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class Beam64Lib {
	public:
		char pin;

		Beam64Lib(char pin, bool displayContext=false);

		char readByte();
	private:
		void delayOneMicro();
		void delayTwoMicro();
		void delayThreeMicro();
};

#endif