/*
  Example for different sending methods
  
  http://code.google.com/p/rc-switch/
  
*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() {

  Serial.begin(9600);
  
  // Transmitter is connected to Arduino Pin #10  
  mySwitch.enableTransmit(4);
  pinMode(13, OUTPUT);
  // Optional set pulse length.
  // mySwitch.setPulseLength(320);
  
  // Optional set protocol (default is 1, will work for most outlets)
  // mySwitch.setProtocol(2);
  
  // Optional set number of transmission repetitions.
  // mySwitch.setRepeatTransmit(15);
  
}

void loop() {

  /* See Example: TypeA_WithDIPSwitches */
  mySwitch.switchOn("11111", 3);
    digitalWrite(13, HIGH);
  delay(1000);
  mySwitch.switchOff("11111", 3);
    digitalWrite(13, LOW);
  delay(1000);

}
