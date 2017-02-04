//
// Nom sketch : phm_WebServer_lm35.ino 
// Date : 28 04 2012
// Auteur : PHM d'après l'exemple standard Arduino
//

#include <SPI.h>
#include <Ethernet.h>

// Remplacer les adresses MAC et IP par celles de votre Arduino Ethernet
byte mac[] = { 
  0x90, 0xA2, 0xDA, 0x00, 0xF6, 0x27 };
byte ip[] = { 
  192,168,0, 177 };
// Création de l'objet server de type EthernetServer disponible sur le port 80

EthernetServer server(80);

void setup()
{
  // Démarrage de la connexion Ethernet et du serveur WEB server
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.begin(9600);
  analogReference(INTERNAL); //Permet de fixer la temperature de refernce à 1,1 volt
}

void loop()
{
  //Déclaration et initialisation des variables
  float mesure=0;        //Variable pour le stockage mesure retournée par le capteur
  float tension=0;       //Variable pour le stockage de la tension 
  float temperature=0;   //Variable pour le stockage de la température 
  float tempmin=100;     //Temperature min
  float tempmax=0;       //Temperature max
  int portana0 = 5;      //Numéro du port analogique sur lequel la température est mesurée

  //Ecoute des requetes client (browser WEB)

  mesure = analogRead(portana0);  //Lecture de la valeur fournie par le capteur de température
  tension = mesure * 1.1 / 1024;  //Conversion en tension (en volt)
  temperature = tension * 100;    //Conversion en température (en degré Celsius)  delay(1000);
  Serial.println(temperature);    //Ecriture de la temperature sur le potr série
  delay(1000);
  EthernetClient client = server.available();
  if (client) {
    // Gestion de la fin de la requête WEB qui est indiquée par l'envoi d'une ligne blanche
    boolean currentLineIsBlank = true;

    while (client.connected()) {
      if (client.available()) {
        client.print("Temperature ambiante = ");
        client.print(temperature);  
        client.stop();
      }
    }
  }
}


