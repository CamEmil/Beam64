#include <Beam64.h>
#include <SoftwareSerial.h>

#define BYTE 8
#define controllerPin 7

#define CTR_CONSOLE_STOP    0x01
#define CTR_CONTROLLER_STOP 0x02

char dataArray[32];
char testDataArray[128];

bool firstBoot;
int correctResponse = 0;
int incorrectResponse = 0;
int loopCounter = 0;
char command = 0;
char btData = 0;

Beam64 beam64(controllerPin, false);
SoftwareSerial BTserial(2, 3);

void setup() {
	Serial.begin(9600);
	BTserial.begin(38400);
	digitalWrite(controllerPin, HIGH);
	pinMode(controllerPin, OUTPUT);

}

void loop() {
	
	if (firstBoot) {
		delay(2000);
		firstBoot = false;
	}
	else {
		//delay(1000);
	}

	//noInterrupts();
	//sendCommandStatus();
	//beam64.sendCommandPoll();

	/*
	command = beam64.readCommand();

	if (command == 0) {
		beam64.writeByte(0x05);
		beam64.writeByte(0x00);
		beam64.writeByteStop(0x02);
	}
	else {
		if (loopCounter % 10 == 0) {
			beam64.writeByte(0x80);
		}
		else {
			beam64.writeByte(0x00);
		}
		
		beam64.writeByte(0x00);
		beam64.writeByte(0x0F);
		beam64.writeByteStop(0x0F);
	} 
	*/

	//byte2 = beam64.readByte();
	//byte3 = beam64.readByte();
	//byte4 = beam64.readByte();



	//interrupts();

	//whatRegister(0x33);
	//while (1);

	//printByte(byte1); //Serial.println(" ");
	//printByte(byte2); Serial.print(" ");
	//printByte(byte3); Serial.print(" ");
	//printByte(byte4); Serial.println();

	//if (byte1 == 0x05 && byte2 == 0x00 && byte3 == 0x02) {
	//	correctResponse++;
	//}
	//else {
	//	incorrectResponse++;
	//}


	//loopCounter++;
}

void printByte(char data) {
	for (int i = 0; i < 8; i++) {
		Serial.print((data & 0x80) && 1);
		data <<= 1;
	}
}

void whatRegister(unsigned char byte) {

	char* regPointer = 0;

	*(regPointer + 16) = 0xBB;

	for (int i = 0; i < 30; i++) {
		Serial.print("Reg ");
		Serial.print(i);
		Serial.print(": ");
		Serial.println(*(regPointer + i), HEX);
	}

	char funny = *regPointer;

	Serial.println(byte, HEX);
}
