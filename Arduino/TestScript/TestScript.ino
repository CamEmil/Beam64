char dataArray[32];
char testDataArray[128];

bool firstBoot;
int correctResponse = 0;
int incorrectResponse = 0;
int loopCounter = 0;

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
	char byte1 = 0;
	char byte2 = 0;
	char byte3 = 0;
	char byte4 = 0;

	//if (loopCounter == 10000) {
	//	Serial.print("Num Errors: ");
	//	Serial.println(incorrectResponse);
	//	Serial.print("Error Rate: ");
	//	Serial.print((((double)incorrectResponse) / 10000) * 100, 2);
	//}

	if (firstBoot) {
		delay(2000);
		firstBoot = false;
	}
	else {
		delay(1000);
	}

	noInterrupts();
	//sendCommandStatus();
	sendCommandPoll();
	byte1 = readByte();
	byte2 = readByte();
	byte3 = readByte();
	byte4 = readByte();
	interrupts();

	printByte(byte1); Serial.print(" ");
	printByte(byte2); Serial.print(" ");
	printByte(byte3); Serial.print(" ");
	printByte(byte4); Serial.println();

	//if (byte1 == 0x05 && byte2 == 0x00 && byte3 == 0x02) {
	//	correctResponse++;
	//}
	//else {
	//	incorrectResponse++;
	//}


	loopCounter++;
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


void printByte(char data) {
	for (int i = 0; i < 8; i++) {
		Serial.print((data & 0x80) && 1);
		data <<= 1;
	}
}

void sendCommandStatus() {
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
}

void sendCommandPoll() {
	PIN_7_OUTPUT;
	PIN_7_HIGH;

	SEND_ZERO;
	SEND_ZERO;
	SEND_ZERO;
	SEND_ZERO;
	SEND_ZERO;
	SEND_ZERO;
	SEND_ZERO;
	SEND_ONE;
	SEND_STOP_BIT;

	PIN_7_INPUT;
	PIN_7_HIGH;
}


char readByte() {
	
	char* byte = 0x00;
	// r20-27 are scratch
	// put byte result in 
	// r20 is counter for delay
	// r21 is counter for each bit

	__asm__(
		"ldi r21, 8			\n"
	"waitforlow:			\n"
		"sbic 9, 7  		\n"
		"rjmp waitforlow	\n"

		"ldi r20, 8			\n"
	"wait32cycles:			\n"
		"nop				\n"
		"dec r20			\n"
		"cpi r20, 0			\n"
		"brne wait32cycles	\n"

		"lsl r0				\n"
		"sbis 9, 7  		\n"
		"rjmp waitforhigh	\n"
		"inc r0				\n"
		"rjmp deccounter	\n"

	"waitforhigh:			\n"
		"sbis 9, 7			\n"
		"rjmp waitforhigh	\n"

	"deccounter:			\n"
		"dec r21			\n"
		"cpi r21, 0			\n"
		"brne waitforlow	\n\t"

	);

	return *byte;
}

// "sbis 11, 7  		\n"
// "sbis 11, 7  		\n"
/*
		"nop				\n"
		"nop				\n"
		"nop				\n"
		"nop				\n"


*/