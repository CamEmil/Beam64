char dataArray[32];

#define BYTE 8
#define controllerPin 7

#define CTR_CONSOLE_STOP    0x01
#define CTR_CONTROLLER_STOP 0x02

#define WAIT_FOR_LOW while(digitalRead(controllerPin) == 1)
#define WAIT_FOR_HIGH while(digitalRead(controllerPin) == 0)

#define PIN_4_OUTPUT	DDRD |= 0x10
#define PIN_4_INPUT		DDRD |= 0x00

#define PIN_4_LOW		PORTD = 0x00 
#define PIN_4_HIGH		PORTD = 0x10

#define NOP __asm__ __volatile__ ("nop\n\t")

void setup() {
	Serial.begin(9600);
	pinMode(controllerPin, OUTPUT);
	digitalWrite(controllerPin, HIGH);

	//attachInterrupt(digitalPinToInterrupt(controllerPin), collectBytesISR, FALLING);
}

void loop() {

	//delay(1000);

	sendCommand(0x00, 0x01);
	translateBytes();
	uint8_t hexData;

	Serial.print(dataArray[1], HEX);

	//for (int i = 0; i < 8; i++) {
	//	if (i % 8 == 0) Serial.print("__");
	//	if (i % 32 == 0) Serial.println();

	//	Serial.print(dataArray[i], HEX);
	//	Serial.print(" ");
	//	//if (dataArray[i] == 1) 
	//	//	Serial.print("1");
	//	//else 
	//	//	Serial.print("0");
	//	//hexData = 0;
	//	//for (int j = 0; j < 8; j++) {
	//	//	hexData = hexData << 1;
	//	//	hexData = hexData + dataArray[i * 8 + j];
	//	//}
	//	//if (hexData < 16) {
	//	//	Serial.print("0");
	//	//}
//	//Serial.print(hexData, HEX);
//	//Serial.print(" ");
//}
Serial.println();

}

void sendCommand(char cmd, char ctrReg) {
	noInterrupts();
	pinMode(controllerPin, OUTPUT);
	digitalWrite(controllerPin, HIGH);

	for (int i = 0; i < BYTE; i++) {
		if (cmd & 0x80) {
			digitalWrite(controllerPin, LOW);
			delayMicroseconds(1);
			digitalWrite(controllerPin, HIGH);
			delayMicroseconds(3);
		}
		else {
			digitalWrite(controllerPin, LOW);
			delayMicroseconds(3);
			digitalWrite(controllerPin, HIGH);
			delayMicroseconds(1);
		}
		cmd <<= 1;
	}

	if (ctrReg & CTR_CONSOLE_STOP) {
		digitalWrite(controllerPin, LOW);
		delayMicroseconds(1);
		digitalWrite(controllerPin, HIGH);
		delayMicroseconds(2);
	}

	if (ctrReg & CTR_CONTROLLER_STOP) {

	}

	//Set back to input, high impedence
	PIN_4_INPUT;
	PIN_4_HIGH;
	interrupts();
}

void pollPin(char pinNum) {

}

//void collectBytes(char* data, char numBytes) {
//	for (int i = 0; i < numBytes; i++) {
//		for (int j = 0; j < BYTE; j++) {
//			*(data + i) <<= 1;
//			//WAIT_FOR_LOW;
//			//delayMicroseconds(2);
//			*(data + i) += digitalReadFast(controllerPin);
//			if (*(data + i) == 0) {
//			}
//		}
//	}
//}


//void collectBytesISR() {
//	uint8_t tempData;
//
//	for (int i = 0; i < 256; i++) {
//		dataArray[i] = (PIND >> 4) & 0x01;
//	}
//}

void translateBytes() {
	uint8_t tempData = 0;
	uint8_t lowCount = 0;

	for (int i = 0; i < 16; i++) {
		tempData <<= 1;
		// wait for pin to go low
		while ((PIND >> 4) & 0x01);
		loopNOP(64);
		tempData |= (PIND >> 4) & 0x01;

		// Read low?
		if (!(tempData & 0x01))
			while (((PIND >> 4) & 0x01) == 0);

	}

	dataArray[1] = tempData;
}

void translateBytes2() {
	uint8_t tempData = 0;
	uint8_t lowCount = 0;
	uint8_t highCount = 0;

	for (int i = 0; i < 8; i++) {
		tempData <<= 1;
		// wait for pin to go low
		while ((PIND >> 4) & 0x01);

		if ((PIND >> 4) & 0x01) {
			lowCount = 0;
			highCount++;
		}
		else {
			highCount = 0;
			lowCount++;
		}
	}

	dataArray[1] = tempData;
}

void loopNOP(int micros) {
	for (int i = 0; i < micros; i++) {
		NOP;
	}
}

