#include <Beam64.h>
#include <util/atomic.h>

#define CONTROLLER_DATA_PIN 3
#define TEST_PIN_1			8
#define TEST_PIN_2			9
#define RXCIE				7
#define RXCn				7

volatile uint8_t *_udrn = 0xC6;
volatile uint8_t *_ucsrb = 0xC1;
volatile uint8_t *_ucsra = 0xC0;
volatile uint8_t *_timsk0 = 0x6E;
//pin change interrupt flag register
volatile uint8_t *_pcif = 0x3B;
//external interrupt flag register
volatile uint8_t *_eifr = 0x3C;


bool firstBoot = true;
unsigned char dataByte1, dataByte2, dataByte3, dataByte4;
unsigned char incomingByte;
unsigned char byteIdentifier;
char* data;
volatile char command;
char numBytesRead;
char numBytesAvailable;
volatile bool handleConsoleData = false;
char state = 0;
char nextState = 0;
unsigned char readByte;
bool dataOrdered = true;
bool breakout = false;

Beam64 beam64(CONTROLLER_DATA_PIN, false);

void setup()
{
	Serial.begin(115200);
	digitalWrite(CONTROLLER_DATA_PIN, HIGH);
	pinMode(CONTROLLER_DATA_PIN, OUTPUT);
	
	pinMode(TEST_PIN_1, OUTPUT);
	pinMode(TEST_PIN_2, OUTPUT);
	data = (char*) malloc(5 * sizeof(char));

	// Disable interrupts on USART receive completion
	*_ucsrb &= ~(1 << RXCIE);

	// Disable all timer zero interrupts
	*_timsk0 = 0;

	attachInterrupt(digitalPinToInterrupt(CONTROLLER_DATA_PIN), handleConsole, FALLING);
}

void loop()
{
	if (firstBoot) {
		delay(1000);
		firstBoot = false;
	}
	
	while (1) {
		if (*_ucsra & (1 << RXCn)) {
			incomingByte = *_udrn;
		}

		//might need to turn off interupts here?
		switch (incomingByte & 0xF0) {
		case 0x90 :
			dataByte1 = (incomingByte << 4) | (dataByte1 & 0x0F);
			break;

		case 0x80:
			dataByte1 = (incomingByte & 0x0F) | (dataByte1 & 0xF0);
			break;

		case 0x70:
			dataByte2 = (incomingByte << 4) | (dataByte2 & 0x0F);
			break;

		case 0x60 :
			dataByte2 = (incomingByte & 0x0F) | (dataByte2 & 0xF0);
			break;

		default : 
			if (incomingByte & 0x01) {
				dataByte4 = incomingByte;
			}

			else {
				dataByte3 = incomingByte;
			}
			break;

		}

		//PORTB |= 2;
		//PORTB &= 0;
	}
	

	//// check for data on serial
	
	//if (*_ucsra & (1 << RXCn)) {
	//	digitalWrite(TEST_PIN, HIGH);
	//	//dataByte3 = *_udrn;
	//	digitalWrite(TEST_PIN, LOW);
	//}

	//while (1) {
	//	digitalWrite(TEST_PIN, HIGH);
	//	digitalWrite(TEST_PIN, LOW);
	//	if (handleConsoleData) {
	//		break;
	//	}
	//}

}

void handleConsole() {
	waitForConsoleCMD();

	//manually pin interrupt triggered by data being sent
	*_eifr = 2;
}

unsigned char USART_Receive(void) {
	/* Wait for data to be received */
	while (!(*_ucsra & (1 << RXCn)));
	/* Get and return received data from buffer */
	return *_udrn;
}

void waitForConsoleCMD() {
	command = beam64.readCommand7Bits();

	switch (command) {
	case 0x00:
		beam64.writeByte(0x05);
		beam64.writeByte(0x00);
		beam64.writeByteStop(0x02);
		break;

	case 0x01:
		beam64.writeByte(dataByte1);
		beam64.writeByte(dataByte2);
		beam64.writeByte(dataByte3);
		beam64.writeByteStop(dataByte4);
		break;

		// Send controller data by default
	default:
		beam64.writeByte(dataByte1);
		beam64.writeByte(dataByte2);
		beam64.writeByte(dataByte3);
		beam64.writeByteStop(dataByte4);
		break;
	}
}

int homemadeDelay() {
	int count = 0;
	int anumber;

	while (count < 31000) {
		count++;
		anumber = count + count;
	}

	return count;
}




//if (Serial.available() > 0) {
//	breakout = false;
//	while (dataOrdered == true) {
//		int loopCounter = 0;
//		while (Serial.available() == 0) {
//			loopCounter++;
//			if (loopCounter > 255) {
//				breakout = true;
//				break;
//			}	
//		}
//		if (breakout) break;

//		readByte = Serial.read();
//		//Serial.write(readByte);
//		

//		if (state == 0) {
//			if (readByte == 0xC0) {
//				nextState = 1;
//			}
//			else {
//				dataOrdered = false;
//				nextState = 0;
//			}
//		}

//		if (state == 1) {
//			if (readByte == 0x99) {
//				nextState = 2;

//			}
//			else {
//				dataOrdered = false;
//				nextState = 0;
//			}
//		}

//		if (state == 2) {
//			nextState = 3;
//			dataByte3 = readByte;
//		}

//		if (state == 3) {
//			nextState = 10;
//			dataByte4 = readByte;
//		}

//		if (state == 10) {
//			nextState = 15;
//			dataByte1 = readByte;
//		}

//		if (state == 15) {
//			dataOrdered = false;
//			nextState = 0;
//			dataByte2 = readByte;
//		}

//		state = nextState;
//	}

//}
//
//dataOrdered = true;