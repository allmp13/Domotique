/*

 ** SD CARD **
 ** CS - pin 4
 
 ** VOLET **
 
 ** VOLET_HAUT pin 3 --> 5
 ** VOLET_STOP pin 4 --> 6
 ** VOLET_BAS pin 5 --> 7
 ** VOLET_CHANGE pin 6 --> 8
 
 ** Photoresistance pin A0
 
 ** DHT11 pin 3
 ** Recepteur 433Mhz Pin 2
 ** Emetteur 433Mhz Pin 9
 
*/


#include "voletfunction.h"
#include <RCSwitch.h>
#include <SD.h>
#include <dht11.h>
#define couloir 5244241
#define porte 8851353
#define onvolet 5393
#define offvolet 5396
#define DHT11PIN 3

long pi=26000; //position Initiale des volets
long p;    // Hauteur des volets en %
RCSwitch mySwitch = RCSwitch();
int chk;
dht11 DHT11;

int nbvolet = 4;
const byte BUFFER_SIZE = 32;
  int photocellPin = 0; // the cell and 10K pulldown are connected to a0
  int photocellReading;

void setup() {                
  // initialize the digital pin as an output.
  #include "voletsetup.h" 
  char buffer[BUFFER_SIZE];
  int tabvolet[nbvolet-1];
  /* Déclare l'itérateur et le compteur de lignes */
  byte i,  line_counter = 0;

  
  Serial.begin(9600);
  mySwitch.enableReceive(0);// Receiver on inerrupt 0 => that is pin #2  
     //pinMode(10, OUTPUT);
    
    //pinMode(9, OUTPUT); 
    Serial.println("PIN 9 OK");
     
  if (!SD.begin(4)) { // Gère les erreurs
    Serial.println("Erreur de carte SD !");
    for(;;);
  }
 
  /* Ouvre le  fichier de configuration */
  File config_file = SD.open("test.txt", FILE_READ);
  if(!config_file) { // Gère les erreurs
    Serial.println("Erreur d'ouverture du fichier !");
    for(;;);
  }
 
  /* Tant que non fin de fichier */
  while(config_file.available() > 0 ){
 
    /* Récupère une ligne entière dans le buffer */
    i = 0;
    while((buffer[i++] = config_file.read()) != '\n') {
    }
    /* Finalise la chaine de caractéres ASCIIZ en supprimant le \n au passage */
    buffer[--i] = '\0';  
    tabvolet[line_counter]=atoi(&buffer[0]);
    /* Incrémente le compteur de lignes */
    ++line_counter;    
  }
 
  /* Ferme le fichier de configuration */
  config_file.close();
  pi=tabvolet[0];
  Serial.println(pi);
}

void loop() {   
 if (Serial.available())
  {
  p = Serial.parseInt(); 
  pi=volet(p,pi);
  Serial.print(p);
  Serial.print(" % --> ");
  Serial.println(pi);
  }
  if (mySwitch.available()) {

    long value = mySwitch.getReceivedValue();
        Serial.println(value);
    switch (value) {
      
        case porte:
            Serial.print("Quelqu'un a ouvert la porte! ");
            Serial.println(value);
            //Serial.println(digitalRead(9));
            //digitalWrite(9, !digitalRead(9));
            photocellReading = analogRead(photocellPin);
            Serial.print("Analog reading = ");
            Serial.print(photocellReading); // the raw analog reading
            // We'll have a few threshholds, qualitatively determined
            if (photocellReading < 20) {
              Serial.println(" - Noir");
            } else if (photocellReading < 200) {
              Serial.println(" - Sombre");
            } else if (photocellReading < 500) {
              Serial.println(" - Lumiere");
            } else if (photocellReading < 800) {
              Serial.println(" - Lumineux");
            } else {
              Serial.println(" - Tres lumineux");
            }
            chk = DHT11.read(DHT11PIN);
              switch (chk)
              {
                case 0: Serial.println("OK"); break;
                case -1: Serial.println("Checksum error"); break;
                case -2: Serial.println("Time out error"); break;
                default: Serial.println("Unknown error"); break;
              }
            Serial.print("Humidity (%): ");
            Serial.println((float)DHT11.humidity, 2);
            Serial.print("Temperature (oC): ");
            Serial.println((float)DHT11.temperature, 2);
            delay(1000);
            break;
            
        case onvolet:
            Serial.println("Onvolet");
            Serial.println(value);
            pi=volet(100,pi);
            delay(1000);
            break;

        case offvolet:
            Serial.println("Offvolet");
            Serial.println(value);
            pi=volet(0,pi);
            delay(1000);
            break;
            
        case couloir:
            Serial.println("Couloir");
            digitalWrite(9, HIGH);
            break;

        default:
            Serial.print("Dispositif inconnu: ");
            Serial.println(value);
            delay(500);
            break;
    }

    mySwitch.resetAvailable();
  }  
  
  
}


