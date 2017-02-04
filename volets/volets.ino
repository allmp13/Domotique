/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
#define VOLET_HAUT 2
#define VOLET_STOP 3
#define VOLET_BAS 4
#define VOLET_CHANGE 5
 
 

void setup() {                
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(VOLET_HAUT, OUTPUT);  
  pinMode(VOLET_STOP, OUTPUT);
  pinMode(VOLET_BAS, OUTPUT);
  pinMode(VOLET_CHANGE, OUTPUT);  
}

void loop() {
  digitalWrite(VOLET_HAUT, HIGH);   // set the LED on
  delay(100);              // wait for a second
  digitalWrite(VOLET_HAUT, LOW);    // set the LED off
   delay(27000);  
  digitalWrite(VOLET_STOP, HIGH);   // set the LED on
  delay(100);              // wait for a second
  digitalWrite(VOLET_STOP, LOW);    // set the LED off
  
    delay(1000); 
    
   digitalWrite(VOLET_BAS, HIGH);   // set the LED on
  delay(100);              // wait for a second
  digitalWrite(VOLET_BAS, LOW);    // set the LED off
   delay(4050);  
  digitalWrite(VOLET_STOP, HIGH);   // set the LED on
  delay(100);              // wait for a second
  digitalWrite(VOLET_STOP, LOW);    // set the LED off
  
  delay(1000);              // wait for a second
  
    digitalWrite(VOLET_HAUT, HIGH);   // set the LED on
  delay(100);              // wait for a second
  digitalWrite(VOLET_HAUT, LOW);    // set the LED off
   delay(4050);  
  digitalWrite(VOLET_STOP, HIGH);   // set the LED on
  delay(100);              // wait for a second
  digitalWrite(VOLET_STOP, LOW);    // set the LED off

  delay(1000); 
  
    digitalWrite(VOLET_BAS, HIGH);   // set the LED on
  delay(100);              // wait for a second
  digitalWrite(VOLET_BAS, LOW);    // set the LED off
   delay(27000);  
  digitalWrite(VOLET_STOP, HIGH);   // set the LED on
  delay(100);              // wait for a second
  digitalWrite(VOLET_STOP, LOW);    // set the LED off

}
