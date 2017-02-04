

/************************************************************** 
 * Affiche sur le moniteur serie le nombre entier reçu.
 * Sketch du récepteur (branché à la pin 11, par défaut).
 * http://electroniqueamateur.blogspot.com/2014/01/modules-rf-433-mhz-virtualwire-et.html
 ***************************************************************/
 
#include <VirtualWire.h>
 


 
char Message[VW_MAX_MESSAGE_LEN]; 
 
void setup() {
  Serial.begin(9600);     
  vw_set_rx_pin(11);
  vw_setup(2000);   // Bits par seconde   
 
  vw_rx_start();       
 
}
 
void loop(){
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  char msg[3];

  if (vw_get_message(buf, &buflen)) 
  {
    int i;
 
    for (i = 0; i < buflen; i++)
    {               
      msg[i] = char(buf[i]);
    }
 
  msg[buflen] = '\0';
  Serial.print("Message recu: ");
  Serial.println(msg);
    // Conversion du tableau de chars en int:
    

    Serial.print("Capteur: ");
    Serial.println(subString(msg,0,1));
 
    Serial.print("Valeur: ");
    Serial.println(subString(msg,1,strlen(msg)-1));
          
  }
}

char* subString(char* arr, int begin, int len)
{
    char* res = new char[len];
    for (int i = 0; i < len; i++)
        res[i] = *(arr + begin + i);
    res[len] = 0;
    return res;
}
