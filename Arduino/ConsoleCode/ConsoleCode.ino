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
volatile char command;

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
		// Check for serial data from bluetooth receiver
		if (*_ucsra & (1 << RXCn)) {
			incomingByte = *_udrn;
		}

		switch (incomingByte & 0xF0) {
		case 0x90 :
			dataByte1 = (incomingByte << 4) | (dataByte1 & 0x0F);
			break;

		case 0x80 :
			dataByte1 = (incomingByte & 0x0F) | (dataByte1 & 0xF0);
			break;

		case 0x70 :
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
	}
}

void handleConsole() {
	waitForConsoleCMD();

	//manually pin interrupt triggered by data being sent
	*_eifr = 2;
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

