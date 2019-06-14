#include <Beam64.h>

#define CONTROLLER_DATA_PIN 7

bool firstBoot = true;
unsigned char contByte1, contByte2, contByte3, contByte4;
unsigned char btByte1, btByte2, btByte3, btByte4, btByte5, btByte6;

Beam64 beam64(CONTROLLER_DATA_PIN, false);

void setup()
{
	Serial.begin(115200);

}

void loop()
{
	//dataByte1 = 0x01;
	//dataByte2 = 0x02;
	//dataByte3 = 0xFF;
	//dataByte4 = 0xFF;
	if (firstBoot) {
		delay(2000);
		firstBoot = false;
	}

	// Request button data from controller
	noInterrupts();
	beam64.sendCommandPoll();
	contByte1 = beam64.readByte();
	contByte2 = beam64.readByte();
	contByte3 = beam64.readByte();
	contByte4 = beam64.readByte();
	interrupts();

	btByte1 = 0x90 | ((contByte1 >> 4) & 0x0F);
	btByte2 = 0x80 | ((contByte1)	& 0x0F);
	btByte3 = 0x70 | ((contByte2 >> 4) & 0x0F);
	btByte4 = 0x60 | ((contByte2) & 0x0F);
	btByte5 = contByte3 & 0xFE;
	btByte6 = contByte4 | 0x01;

	// Wait 250 ms then write to bluetooth by serial
	delay(10);
	Serial.write(btByte1);
	Serial.write(btByte2);
	Serial.write(btByte3);
	Serial.write(btByte4);
	Serial.write(btByte5);
	Serial.write(btByte6);
	
}
