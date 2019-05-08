char dataArray[32];
char testDataArray[128];

#define BYTE 8
#define controllerPin 7

#define CTR_CONSOLE_STOP    0x01
#define CTR_CONTROLLER_STOP 0x02

#define PIN_7_OUTPUT	DDRD = DDRD | 0b10000000
#define PIN_7_INPUT		DDRD = DDRD & 0b01111111

#define PIN_7_LOW		PORTD = 0b00000000
#define PIN_7_HIGH		PORTD = 0b10000000

#define PIN_7_READ		((PIND >> 7) & 1)

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

void setup() {
	Serial.begin(9600);
	digitalWrite(7, HIGH);
	pinMode(7, OUTPUT);
	//digitalWrite(7, HIGH);

	//attachInterrupt(digitalPinToInterrupt(controllerPin), collectBytesISR, FALLING);
}

void loop() {
	char goodByte = 0;
	delay(2000);

	sendCommandStatus();
	//sendCommand(0x00, 0x01);
	goodByte = readBytes();

	printByte(goodByte);

	/*
	collectDebugBytes(32);
	for (int i = 0; i < 32; i++) {
		
		if (i % 4 == 0) 
			Serial.println();
		

		printByte(testDataArray[i]);
		Serial.print(" ");
	}
	*/

	while (1);

	//translateBytes();
	//Serial.println("Yeet2");
	uint8_t hexData;

	//Serial.println("hello world");

}

void sendCommand(char cmd, char ctrReg) {
	noInterrupts();
	pinMode(7, OUTPUT);
	digitalWrite(7, HIGH);

	for (int i = 0; i < BYTE; i++) {
		if (cmd & 0x80) {
			//PIN_7_LOW;
			digitalWrite(7, LOW);
			//loopNOP(1);
			delayMicroseconds(1);
			//PIN_7_HIGH;
			digitalWrite(7, HIGH);
			//loopNOP(3);
			delayMicroseconds(3);
		}
		else {
			PIN_7_LOW;
			delayThreeMicros();
			PIN_7_HIGH;
			delayOneMicro();

		}
		cmd <<= 1;
	}

	if (ctrReg & CTR_CONSOLE_STOP) {
		PIN_7_LOW;
		delayOneMicro();
		PIN_7_HIGH;
		delayTwoMicros();
	}

	if (ctrReg & CTR_CONTROLLER_STOP) {

	}

	//Set back to input, high impedence
	pinMode(7, INPUT);
	digitalWrite(7, HIGH);
	interrupts();
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


void collectDebugBytes(int numBytes) {
	uint8_t tempData = 0;

	for (int j = 0; j < numBytes; j++) {
		tempData = 0;
		for (int i = 0; i < 8; i++) {
			tempData <<= 1;
			tempData |= PIN_7_READ;
		}
		testDataArray[j] = tempData;
	}
}

void printByte(char data) {
	for (int i = 0; i < 8; i++) {
		Serial.print(data & 1);
		data >>= 1;
	}
}

void sendCommandStatus() {
	noInterrupts();
	PIN_7_OUTPUT;
	PIN_7_HIGH;

	SEND_ZERO;
	SEND_ZERO;
	SEND_ZERO;
	SEND_ZERO;
	SEND_ZERO;
	SEND_ZERO;
	SEND_ZERO;
	SEND_ZERO;
	SEND_STOP_BIT;

	PIN_7_INPUT;
	PIN_7_HIGH;
	interrupts();
}


char readBytes() {
	char readByte;
	// r20-27 are scratch
	// put byte result in 
	// r20 is counter for delay
	// r21 is counter for each bit
	__asm__(
		"testPortD:			\n"
		"ldi r21, 8			\n"
		"sbis 9, 7  	\n"
		"rjmp delay			\n"
		"nop				\n"
		"rjmp testPortD		\n"
     "resetcounter:			\n"
		"ldi r20, 16		\n"
     "delay:				\n"
		"dec r20			\n"
		"cpi r20, 0			\n"
		"brne delay			\n"
		"sbis 9, 7  	\n"
		"rjmp getout		\n"
		"nop				\n"
		"inc r0				\n"
     "getout:				\n"
		"lsl r0				\n"
		"dec r21			\n"
		"cpi r21, 0			\n"
		"brne resetcounter	\n\t"
	);

	return readByte;
}

// "sbis 11, 7  		\n"
// "sbis 11, 7  		\n"