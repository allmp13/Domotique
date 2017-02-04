/*
set:
http://arduino/set/?r1=1 ou 0  Allume et éteind Lampe Canapé
http://arduino/set/?r2=1 ou 0  Allume et éteind Lampe Porte
http://arduino/set/?r3=1 ou 0  Allume et éteind Lampe Table

http://arduino/set/?v1=n       Ouvre les volets de n%
http://arduino/set/?v2         Stoppe les volets

get:
http://arduino/get/?sensor=d1   Lecture Position des volets
http://arduino/get/?sensor=d2   Lecture luminosité
*/

//#define DEBUG

#include <SPI.h>
#include <Ethernet.h>
#include <RCSwitch.h>
#include <SD.h>

/*#define couloir 5244241
#define porte 8851353
#define onvolet 5393
#define offvolet 5396*/

#define VOLET_HAUT 5
#define VOLET_STOP 6
#define VOLET_BAS 7
#define VOLET_CHANGE 8

RCSwitch mySwitch = RCSwitch();
//String ln = String("\r\n");


// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,0,247);   // CONFIG (adresse de l'arduino)

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);

// Initialize variables
unsigned long depart;
unsigned long fin;
boolean enmarche;
int positionvolet;
String inputString = "";         // a string to hold incoming data
//boolean stringComplete = false;
boolean sens; // 0 baisse 1 monte
boolean bascule = true;


//Fonction annexes

void parse(String request, String *body, int *err);
void action_get(String request, String *body, int *err);
void action_set(String request, String *body, int *err);

// ********************** SETUP ****************




void setup() {
  #if defined DEBUG
  Serial.begin(9600);
  #endif
  pinMode(VOLET_HAUT, OUTPUT);   //VOLET_HAUT 5
  pinMode(VOLET_STOP, OUTPUT);   //VOLET_STOP 6
  pinMode(VOLET_BAS, OUTPUT);  //VOLET_BAS 7
  pinMode(VOLET_CHANGE, OUTPUT); //VOLET_CHANGE 8
 /*  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }*/
  mySwitch.enableReceive(0);// Receiver on inerrupt 0 => that is pin #2  
  mySwitch.enableTransmit(9); // 433Mhz Tx (emetteur 433Mhz)
     
  //Serial.println(F("Initializing SD card..."));
 
  if (!SD.begin(4)) {
    //Serial.println(F("initialization failed!"));
    return;
  }
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  // Ethernet.begin(mac);
  server.begin();
  /*Serial.print(F("Server is at "));
  Serial.println(Ethernet.localIP());*/
 

enmarche = false;
fin = millis();
//ecrireposvolet(100);
//Serial.println("Version optimvoletokv1");
positionvolet=lireposvolet(1);
#if defined DEBUG
 Serial.println(__FILE__);
 Serial.println(__DATE__);
 Serial.println(__TIME__);
  Serial.println(ip);
  //Serial.print(F("Position Initiale des Volets: "));
  Serial.println(positionvolet);
  //Serial.println(fin);
#endif
}

void loop() {  
  byte i;
  arretvolet();
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    // set variables defaut values
    i = 0;
    String get = String();
    String body = String();
    byte err = 200;
    //serial.println(F("new client"));
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        /*char* errors[][22] = {};*/
        /*
        *errors[200]="OK";
        *errors[400]="Bad request";
        *errors[401]="Unauthorized";
        *errors[404]="Not Found";
        */
        //*errors[500]="Internal Server Error";
        char c = client.read();
        if (i == 0) {
          get += c;
        }
        //Serial.write(c);
        /*if you've gotten to the end of the line (received a newline
        character) and the line is blank, the http request has ended,
        so you can send a reply*/
        if (c == '\n' && currentLineIsBlank) {
          // parse the request
          parse(get, &body, &err);
          //serial.println(F("From parse function"));
            //serial.println(F("Body:"));
            //serial.println(body);
            //serial.println(F("Erreur: "));
            //serial.println(err);
          // send a standard http response header
          if (err == 200) {                               //
            client.println(F("HTTP/1.1 200 OK"));            //  TODO: modifier pour retourner les codes d'erreur
          } else {                                        //
            client.println(F("HTTP/1.1 400 Bad Request"));   //
          }
          client.println(F("Content-Type: application/json;charset=utf-8"));
          client.println("Server: Arduino");
          client.println(F("Connnection: close"));
          client.println();
          //client.println("<!DOCTYPE HTML>");
          //client.println("<html>");
          client.println(body);
          //client.println("</html>");
          //serial.print(F("HTTP/1.1 "));
          //serial.print(err);
          //serial.print(F(" "));
          ////serial.println(*errors[err]);
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
          i++;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    //serial.println(F("client disonnected"));
    //serial.println();
  }

  if (mySwitch.available()) {                   // Pour utilisation d'un récepteur 433Mhz (utile pour cloner une tÃ©lÃ©commande SANS ROLLING CODE)
    int value = mySwitch.getReceivedValue();
    switch (value) {
      
        case 8851353:
            //serial.print(F("Quelqu'un a ouvert la porte! "));
            //serial.println(value);
            break;
            
        case 5393:
            //serial.println(F("Onvolet"));
            //serial.println(value);
              if (!enmarche) 
               {
                depart=millis();
                if (bascule) fin=depart+activevolet(100);
                else fin=depart+activevolet(0);
                bascule=!bascule;
              }
            break;

        case 5396:
            //serial.println(F("Offvolet"));
            //serial.println(value);
            /*depart=millis();
            if (!enmarche) fin=depart+activevolet(0);*/
            arret();
            break;
            
        default:
            //serial.print(F("Dispositif inconnu: "));
            //serial.println(value);
            break;
    }
    mySwitch.resetAvailable();
  }

}


void arretvolet()
{
   if (enmarche && millis() >= fin)
   {
      //arreter les volets
      arret();
   }
}

unsigned long activevolet(int nouvelleposition)
{
  
  int delta=0;

  if (nouvelleposition <= 100 && nouvelleposition >= 0)
  {
  delta = nouvelleposition-positionvolet;
  if (delta != 0)
    {
      if (delta > 0)
        {
          monte();
        }
      else
        {
          //descend
          baisse();
        }   
      enmarche=true;
      //positionvolet=nouvelleposition;
    }
    //else Serial.println(F("Ne bouge pas !!!!"));
  }
#if defined DEBUG
Serial.print(F("DELTA "));
Serial.println(delta);
#endif
return abs(delta)*260;
}

void monte()
{
  digitalWrite(VOLET_HAUT, HIGH);   // set the LED on
  delay(100);              // wait for a second
  digitalWrite(VOLET_HAUT, LOW);    // set the LED off 
  #if defined DEBUG
  Serial.println(F("Monte"));
  #endif
  sens=true;
}

void baisse()
{ 
  digitalWrite(VOLET_BAS, HIGH);   // set the LED on
  delay(100);              // wait for a second
  digitalWrite(VOLET_BAS, LOW);    // set the LED off
  #if defined DEBUG
  Serial.println(F("Baisse"));
  #endif
  sens=false;
}

void arret()
{ 
  if (enmarche)
  {
  enmarche = false;
  if (!sens)
  {
    positionvolet=positionvolet-((millis()+200-depart)/260);
  }
  else
  {
    positionvolet=positionvolet+((millis()+200-depart)/260);
  }
  #if defined DEBUG
  Serial.print(F("Pos volet "));
  Serial.println(positionvolet);
  #endif
  if (positionvolet==0) delay(6000);
  ecrireposvolet(positionvolet);
  digitalWrite(VOLET_STOP, HIGH);   // set the LED on
  delay(100);              // wait for a second
  digitalWrite(VOLET_STOP, LOW);    // set the LED off 
  #if defined DEBUG
  Serial.println(F("Arret"));
  #endif
  }
}

void ecrireposvolet(long posvolet)
{
    //Serial.println(F("Ecriture SD"));
    File myFile = SD.open("test.txt", FILE_WRITE);
    myFile.seek(0);
 
  // if the file opened okay, write to it:
  if (myFile) {
    myFile.println(posvolet);
	// close the file:
    myFile.close();
    //Serial.println(posvolet);
  } else {
    // if the file didn't open, print an error:
   //Serial.println(F("error opening test.txt"));
  }
}
int luminosite()
{
  int photocellPin = 3; // the cell and 10K pulldown are connected to a0
  int photocellReading;
            photocellReading = analogRead(photocellPin);
            /*Serial.print("Analog reading = ");
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
            }*/       
return photocellReading;
}
long lireposvolet(byte ligne)
{
   //File myFile;
   char buffer[32];
   int i,  line_counter = 0;
   long valeur;
   ligne=ligne-1;
    /* Ouvre le  fichier de configuration */
  File config_file = SD.open("test.txt", FILE_READ);
  if(!config_file) { // GÃ¨re les erreurs
    //Serial.println(F("Erreur d'ouverture du fichier !"));
    for(;;);
  }
 
  /* Tant que non fin de fichier */
  while(config_file.available() > 0 ){ 
    /* RÃ©cupÃ¨re une ligne entiÃ¨re dans le buffer */
    i = 0;
    while((buffer[i++] = config_file.read()) != '\n') {
    }
    /* Finalise la chaine de caractÃ©res ASCIIZ en supprimant le \n au passage */
    buffer[--i] = '\0';  
    if (line_counter == ligne) valeur=atoi(&buffer[0]);
    /* IncrÃ©mente le compteur de lignes */
    ++line_counter;    
  }
  /* Ferme le fichier de configuration */
  config_file.close();
  return valeur;
}

void parse(String request, String *body, byte *err) {            //Fonction servant Ã  parser la requÃªte GET (voir syntaxe en haut du fichier)
  request = request.substring(4, request.length() - 11);
  //serial.print(F("La requete: "));
  //serial.println(request);

if (request.charAt(1) == 'g') {
  // Get
  action_get(request, &*body, &*err);
}
// Set
else if (request.charAt(1) == 's') {

  action_set(request, &*body, &*err);
  
}
else {
  //serial.println(F("Erreur de lecture de la requÃªte"));
  *err = 400;
}
}


void action_get(String request, String *body, byte *err) {     // Pour rÃ©cupÃ©rer des informations 
  char pinChar[request.substring(14).length() + 1];
  request.substring(14).toCharArray(pinChar, sizeof(pinChar));
  int pin = atoi(pinChar);
  int value;
  value=positionvolet;
  // digital read
  if ((request.charAt(13) == 'd') || (request.charAt(13) == 'D')) {
      if (enmarche)
      {
      if (!sens)
      {
        value = positionvolet-((millis()-depart)/260);
      }
      else
      {
        value = positionvolet+((millis()-depart)/260);
      }
      }
      else value=positionvolet;
      #if defined DEBUG
      Serial.print(F("Pos volet "));
      Serial.println(value);
      #endif

    }   
  // analog read
  else if ((request.charAt(13) == 'a') || (request.charAt(13) == 'A')) {
    switch (pin) {                                            // Idem que le commentaire prÃ©cÃ©dent, ici dans le cas d'un analog read
      case 1337:
      //*body += map(analogRead(14), 0, 1023, 100, 0);/* + ln;*/
      break;
      default:
      //*body += analogRead(pin);/* + ln;*/
      break; 
    }
  }
  else {
    //serial.println(F("Erreur de lecture de la requÃªte"));
    *err = 400;
  }
  *body += "{\"module\":\"arduino1\",\"localisation\":\"salon\",\"capteurs\":[{\"type\":\"luminosite\",\"valeur\":\"";
  *body += map((luminosite()), 0, 1023, 0, 100);
  *body += "\"},{\"type\":\"volet\",\"valeur\":\"";
  *body += value;
  *body += "\"}]}";
}


void action_set(String request, String *body, byte *err) {     // Pour effectuer des actions
  //serial.println(F("set"));
  char pinChar[request.substring(7, request.indexOf("=")).length() + 1];
  request.substring(7, request.indexOf("=")).toCharArray(pinChar, sizeof(pinChar));
  byte pin = atoi(pinChar);
  char valueChar[request.substring(request.indexOf("=") + 1).length() + 1];
  request.substring(request.indexOf("=") + 1).toCharArray(valueChar, sizeof(valueChar));
  byte value = atoi(valueChar);
  if ((request.charAt(6) == 'v') || (request.charAt(6) == 'V')) {
    //Serial.println(F("digital"));
    switch (pin) {        // MÃªme utilitÃ© que dans la fonction action_get()
      case 1:
        /*depart=millis();
        if (!enmarche) fin=depart+activevolet(value);*/
         if (!enmarche) 
               {
                depart=millis();
                fin=depart+activevolet(value);
              }
        break;
      case 2:
        arret();
        break;
    }
  }
  else if ((request.charAt(6) == 'a') || (request.charAt(6) == 'A')) {
    switch (pin) {                                           // // MÃªme utilitÃ© que dans la fonction action_get()
      default:
      analogWrite(pin, value); // Attention, analogWrite ne veut en rien dire que l'on Ã©crit obligatoirement sur une pin Analog, c'est pour diffÃ©rencier de digitalWrite (1 ou 0, ici 0-255) pour Ã©crire sur une pin digital, indiquer le nombre simplement, pour Ã©crire sur de l'analog, ajouer 14 au nÂ° de la pin. Exemple, pour A0 Ã©crire /set/?a14=val
    }
  }
  else if ((request.charAt(6) == 'r') || (request.charAt(6) == 'R')) {
        if (value == 1) {
          //Serial.println(F("On"));
          mySwitch.switchOn("11111", pin);
        } else {
         // Serial.println(F("Off"));
          mySwitch.switchOff("11111", pin);
        }
  }
  *body += "{\"module\":\"arduino1\",\"localisation\":\"salon\",\"capteurs\":[{\"type\":\"luminosite\",\"valeur\":\"";
  *body += map((luminosite()), 0, 1023, 0, 100);
  *body += "\"},{\"type\":\"volet\",\"valeur\":\"";
  *body += positionvolet;
  *body += "\"}]}";
}

