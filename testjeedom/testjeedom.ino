#include <MySensor.h>
#include <SPI.h>
#include <RCSwitch.h>
 
#define ID 0
#define ON 1
#define OFF 0
#define debounceDelay 2000
double last_times[2] = {0,0};
 
MySensor gw;
MyMessage msg(ID, V_LIGHT);
RCSwitch mySwitch = RCSwitch();
 
void setup() 
{ 
  gw.begin();
  gw.present(ID, S_LIGHT); 
  gw.sendSketchInfo("Test", "1.0");
  mySwitch.enableReceive(1);
}

  bool debounce(int number) {
    if ((last_times[number] == 0) ||
        ((millis() - last_times[number]) > debounceDelay)) {
        last_times[number] = millis();
        return true;
    }
    return false;
}
 
void loop()
{
    if (mySwitch.available()) {
    
    int value = mySwitch.getReceivedValue();
    
        switch (value) {
        case 5393:
            if (debounce(0))
            {
                Serial.println("Allume !");
                gw.send(msg.set(ON));
            }
            break;
        case 5396:
            if (debounce(1))
            {
                Serial.println("Eteind !");
                gw.send(msg.set(OFF));
            }
            break;
        default:
            Serial.print("Dispositif inconnu: ");
            Serial.println(value);
            break;
    }
    
    /*if (value == 0) {
      Serial.print("Unknown encoding");
    } else {
      Serial.print("Received ");
      Serial.print( mySwitch.getReceivedValue() );
      Serial.print(" / ");
      Serial.print( mySwitch.getReceivedBitlength() );
      Serial.print("bit ");
      Serial.print("Protocol: ");
      Serial.println( mySwitch.getReceivedProtocol() );
      if (debounce(0))
      gw.send(msg.set(mySwitch.getReceivedValue()));
    }*/

    mySwitch.resetAvailable();
  }
  //gw.sleep(1,CHANGE);
  
  
     /*gw.send(msg.set(OPEN)); 
     delay(10000); // Wait 10 seconds
     gw.send(msg.set(CLOSE));
     delay(10000);*/
}
