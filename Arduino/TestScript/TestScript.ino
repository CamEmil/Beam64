char dataArray[1024];

#define BYTE 8
#define controllerPin 3

#define CTR_CONSOLE_STOP    0x01
#define CTR_CONTROLLER_STOP 0x02

#define WAIT_FOR_LOW while(digitalRead(controllerPin))
#define WAIT_FOR_HIGH while(!digitalRead(controllerPin))

void setup() {
	Serial.begin(9600);
	pinMode(controllerPin, OUTPUT);
	digitalWrite(controllerPin, HIGH);

	attachInterrupt(digitalPinToInterrupt(controllerPin), collectBytesISR, FALLING);
}

void loop() {

	sendCommand( 0x00 , 0x01);
	//collectBytes( dataArray, 2);

	for (int i = 0; i < 3; i++) {
		Serial.print(dataArray[i], HEX);
		Serial.print(" ");
	}
	Serial.println();

	delay(2);
}

void sendCommand( char cmd , char ctrReg) {
	noInterrupts();
	pinMode(controllerPin, OUTPUT);
	digitalWrite(controllerPin, HIGH);

	for (int i = 0; i < BYTE; i++) {
		if (cmd & 255) {
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
		cmd << 1;
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
			*(data + i) << 1;
			//WAIT_FOR_LOW;
			delayMicroseconds(2);
			*(data + i) += digitalRead(controllerPin);
			if (*(data + i) == 0){
				WAIT_FOR_HIGH;
			}
		}
	}
}


void collectBytesISR() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < BYTE; j++) {
			*(dataArray + i) << 1;
			//WAIT_FOR_LOW;
			delayMicroseconds(2);
			*(dataArray + i) += digitalRead(controllerPin);
			if (*(dataArray + i) == 0) {
				WAIT_FOR_HIGH;
			}
		}
	}
}

//asm volatile ("
//
//; your assembly code here
//
// ");
