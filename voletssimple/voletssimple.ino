

#include "voletfunction.h"
#include <RCSwitch.h>
#include <SD.h>
#define couloir 5244241
#define porte 8851353
#define onvolet 5393
#define offvolet 5396

long pi=26000; //position Initiale des volets
long p;    // Hauteur des volets en %
RCSwitch mySwitch = RCSwitch();

int nbvolet = 4;
const byte BUFFER_SIZE = 32;

void setup() {                
  // initialize the digital pin as an output.
  #include "voletsetup.h" 
  char buffer[BUFFER_SIZE];
  int tabvolet[nbvolet-1];
  /* Déclare l'itérateur et le compteur de lignes */
  byte i,  line_counter = 0;
  
  Serial.begin(9600);
  mySwitch.enableReceive(0);// Receiver on inerrupt 0 => that is pin #2  
     pinMode(10, OUTPUT); 
     
  if (!SD.begin(10)) { // Gère les erreurs
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

    switch (value) {
        case porte:
            Serial.print("Quelqu'un a ouvert la porte! ");
            Serial.println(value);
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

        default:
            Serial.print("Dispositif inconnu: ");
            Serial.println(value);
            delay(500);
            break;
    }

    mySwitch.resetAvailable();
  }  
  
  
}


