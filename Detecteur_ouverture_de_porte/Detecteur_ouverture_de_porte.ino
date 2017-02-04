#include <RCSwitch.h>
#define couloir 12449942
#define porte 8851353

RCSwitch mySwitch = RCSwitch();

void setup() {
    Serial.begin(9600);
    mySwitch.enableReceive(0);
}

void loop() {
  if (mySwitch.available()) {

    int value = mySwitch.getReceivedValue();

    while (!Serial) ;

    switch (value) {
        case porte:
            Serial.println("Quelqu'un a ouvert la porte!");
            delay(100);
            break;

        case couloir:
            Serial.println("Quelqu'un marche dans le couloir !");
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
