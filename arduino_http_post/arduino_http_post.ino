#include <SPI.h>
#include <Ethernet.h>
#include "arduino_http_post.h"

void setup() {
  pinMode(7, OUTPUT); 
}

void loop() {
  //digitalWrite(7, !digitalRead(7));
  sendToExosite(1,26);
  sendToExosite(2,1);
  delay(1000);          
}
