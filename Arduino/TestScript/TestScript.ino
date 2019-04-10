char dataArray[1024];

#define BYTE 8
#define controllerPin 4

#define CTR_CONSOLE_STOP    0x01
#define CTR_CONTROLLER_STOP 0x02

#define WAIT_FOR_LOW while(digitalRead(controllerPin) == 1)
#define WAIT_FOR_HIGH while(digitalRead(controllerPin) == 0)

void setup() {
	Serial.begin(9600);
	pinMode(controllerPin, OUTPUT);
	digitalWrite(controllerPin, HIGH);

	//attachInterrupt(digitalPinToInterrupt(controllerPin), collectBytesISR, FALLING);
}

void loop() {

	sendCommand(0x00 , 0x01);
	collectBytesISR();
	uint8_t hexData;

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 8; j++) {
			hexData <<= hexData;
			hexData |= dataArray[i];
		}
		if (hexData == 0){
			Serial.print("00");
		}
		else if (hexData < 16) {
			Serial.print("0");
		}
		else {

		}
		Serial.print(hexData, HEX);
		Serial.print(" ");
	}
	Serial.println();

}

void sendCommand( char cmd , char ctrReg) {
	noInterrupts();
	pinMode(controllerPin, OUTPUT);
	digitalWrite(controllerPin, HIGH);

	for (int i = 0; i < BYTE; i++) {
		if (cmd & 0x80) {
			digitalWrite(controllerPin, LOW);
			delayMicroseconds(1);
			digitalWrite(controllerPin, HIGH);
			delayMicroseconds(3);
		} else {
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
	pinMode(controllerPin, INPUT);
	digitalWrite(controllerPin, HIGH);
	interrupts();
}

void pollPin ( char pinNum ) {

}

void collectBytes( char* data, char numBytes ) {
	for (int i = 0; i < numBytes ; i++) {
		for (int j = 0; j < BYTE ; j++) {
			*(data + i) <<= 1;
			//WAIT_FOR_LOW;
			//delayMicroseconds(2);
			*(data + i) += digitalReadFast(controllerPin);
			if (*(data + i) == 0){
			}
		}
	}
}


void collectBytesISR() {
	uint8_t tempData;

	for (int i = 0; i < 80; i++) {
		dataArray[i] = digitalReadFast(controllerPin);
	}
}

//asm volatile ("
//
//; your assembly code here
//
// ");
