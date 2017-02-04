
/******************************************************
 * Envoi de nombres croissants par l'entremise de VirtualWire
 * Sketch de l'émetteur (branché à la pin 12, par défaut).
 * http://electroniqueamateur.blogspot.com/2014/01/modules-rf-433-mhz-virtualwire-et.html
 ******************************************************/
 
#include <VirtualWire.h>
 
char msg[4];
 
char Message[VW_MAX_MESSAGE_LEN]; 
 
void setup() {
  vw_set_tx_pin(9);
  vw_setup(2000);     // Bits par seconde
  Serial.begin(9600);
}
 
void loop() {
 
   for (int i=95; i<=100;i++)
  {
  sprintf(msg,"A%d",i);
  Serial.println(msg);
  vw_send((uint8_t *)msg, strlen(msg));
  vw_wait_tx(); // On attend la fin de la transmission
  delay(200);  // et on se repose un peu...
  } 
  //delay(6000);
} 
