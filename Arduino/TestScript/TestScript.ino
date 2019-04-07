char dataArray[1024];
char controllerPin = 13;

#define BYTE 8

#define CTR_CONSOLE_STOP    0x01
#define CTR_CONTROLLER_STOP 0x02

#define WAIT_FOR_LOW while(digitalRead(controllerPin))
#define WAIT_FOR_HIGH while(!digitalRead(controllerPin))

void setup() {
  Serial.begin(9600);
  pinMode(controllerPin, OUTPUT);
  digitalWrite(controllerPin, HIGH);

}

void loop() {

  sendCommand( 0x00 , 0x01);
  for (int i = 0; i < 1024; i++) {
    dataArray[i] = digitalRead(controllerPin);
  }

  Serial.println("Console Start Command Sent");
  for (int i = 0; i < BYTE; i++) {
    if (i % 8 == 0) Serial.println();
    Serial.print(dataArray[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}

void sendCommand( char cmd , char ctrReg) {
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
}

void pollPin ( char pinNum ) {

}

void collectBytes( char* data, char numBytes ) {
  for (int i = 0; i < numBytes ; i++) {
    for (int j = 0; j < BYTE ; j++) {
      *(data + i) << 1;
      WAIT_FOR_LOW;
      delayMicroseconds(2);
      *(data + i) |= digitalRead(controllerPin);
      if (*(data + i) == 0){
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
