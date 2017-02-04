
#include <Arduino.h>
#include <SD.h>
#define VOLET_HAUT 3
#define VOLET_STOP 4
#define VOLET_BAS 5
#define VOLET_CHANGE 6

void monte()
{
  Serial.println("Monte");
  digitalWrite(VOLET_HAUT, HIGH);   // set the LED on
  delay(100);              // wait for a second
  digitalWrite(VOLET_HAUT, LOW);    // set the LED off
}

void baisse()
{
  Serial.println("Baisse");
  digitalWrite(VOLET_BAS, HIGH);   // set the LED on
  delay(100);              // wait for a second
  digitalWrite(VOLET_BAS, LOW);    // set the LED off
}

void arret()
{
  Serial.println("Arret");
  digitalWrite(VOLET_STOP, HIGH);   // set the LED on
  delay(100);              // wait for a second
  digitalWrite(VOLET_STOP, LOW);    // set the LED off
}

long volet(long niveau,long positioninitiale)
{
long deb;
long delta;
 if (niveau >= 0 && niveau <= 100)
 {   
   niveau=niveau*260;
   if (niveau < positioninitiale)
   {
    baisse();
    deb=millis();
    do
    {
      delta=millis()-deb;
    }while (niveau <= (positioninitiale-delta));
    arret();
  } 
  else if (niveau > positioninitiale)
  {
    monte();
    deb=millis();
    do
    {
      delta=millis()-deb;
    }while (niveau >= (positioninitiale+delta));
    arret();
  }
 }

 /* if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    for(;;);}*/
  Serial.println("initialization done.");
    File myFile = SD.open("test.txt", FILE_WRITE);
    myFile.seek(0);
 
  // if the file opened okay, write to it:
  if (myFile) {
    myFile.println(niveau);
	// close the file:
    myFile.close();
    Serial.println(niveau);
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
   return niveau;
}
