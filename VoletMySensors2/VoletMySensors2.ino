 // Version modifié pour compatibilité avec mysensors 2.x

// Enable debug prints to serial monitor
#define MY_DEBUG

#define SN "Volet_JMD"
#define SV "2.1"
#define MY_RADIO_NRF24

#include <MySensors.h> 
#include <SPI.h>

#define VOLET_HAUT 5
#define VOLET_STOP 6
#define VOLET_BAS 7
#define VOLET_CHANGE 8

// Initialize variables
unsigned long depart;
unsigned long fin;
boolean enmarche;
int positionvolet=0;
/*String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;*/
boolean sens; // 0 baisse 1 monte
boolean stoppe = false;

//MySensor gw;

static int currentLevel = 0;  // Current dim level...
MyMessage dimmerMsg(0, V_DIMMER);
MyMessage lightMsg(0, V_LIGHT);


/***
 * Dimmable LED initialization method
 */
void setup()  
{ 

  Serial.begin(115200);
  pinMode(VOLET_HAUT, OUTPUT);   //VOLET_HAUT 5
  pinMode(VOLET_STOP, OUTPUT);   //VOLET_STOP 6
  pinMode(VOLET_BAS, OUTPUT);  //VOLET_BAS 7
  pinMode(VOLET_CHANGE, OUTPUT); //VOLET_CHANGE 8
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }



enmarche = false;
fin = millis();
//ecrireposvolet(100);
 Serial.println(__FILE__);
 Serial.println(__DATE__);
 Serial.println(__TIME__);
//Serial.println(ip);
//Serial.print(F("Position Initiale des Volets: "));
//Serial.println(fin);
  positionvolet=loadState(0);
  Serial.println(positionvolet);
  
  Serial.println( SN ); 
  //begin( incomingMessage );
  request( 0, V_DIMMER );
}

void presentation()
{
  present( 0, S_DIMMER );
  sendSketchInfo(SN, SV);
}

void loop() 
{
  arretvolet();
  //process();
}



void receive(const MyMessage &message) {
  if (message.type == V_LIGHT || message.type == V_DIMMER) {
    
    //  Retrieve the power or dim level from the incoming request message

    int requestedLevel = atoi( message.data );
    if (  requestedLevel == 999)
    {
       if (enmarche) 
               {
                stoppe=true;
               }
      }
    else
    {
        if (!enmarche) 
               {
                 depart=millis();
                 fin=depart+activevolet(requestedLevel);
               }
    }
   
    }
}



void arretvolet()
{
   if ((enmarche && millis() >= fin) || stoppe )
   {
      //arreter les volets
      arret();
      stoppe=false;
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
    else Serial.println(F("Ne bouge pas !!!!"));
  }
Serial.print(F("DELTA "));
Serial.println(delta);
return abs(delta)*260;
}

void monte()
{
  digitalWrite(VOLET_HAUT, HIGH);   // set the LED on
  delay(100);              // wait for a second
  digitalWrite(VOLET_HAUT, LOW);    // set the LED off 
  Serial.println(F("Monte"));
  sens=true;
}

void baisse()
{ 
  digitalWrite(VOLET_BAS, HIGH);   // set the LED on
  delay(100);              // wait for a second
  digitalWrite(VOLET_BAS, LOW);    // set the LED off
  Serial.println(F("Baisse"));
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
  Serial.print(F("Pos volet "));
  Serial.println(positionvolet);
  if (positionvolet==0) delay(2000);
  digitalWrite(VOLET_STOP, HIGH);   // set the LED on
  delay(100);              // wait for a second
  digitalWrite(VOLET_STOP, LOW);    // set the LED off 
  //ecrireposvolet(positionvolet);
  // Inform the gateway of the current DimmableLED's SwitchPower1 and LoadLevelStatus value...
  send(lightMsg.set(positionvolet > 0 ? 1 : 0));
  send(dimmerMsg.set(positionvolet) );
  saveState(0, positionvolet);
  Serial.println(F("Arret"));
  }
}



