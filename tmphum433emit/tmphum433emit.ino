// Example sketch showing how to control physical relays.
// This example will remember relay state even after power failure.

#include <MySensors.h>
#include <SPI.h>
#include <RCSwitch.h>
#include <DHT.h>


#define CHILD_ID_HUM 0
#define CHILD_ID_TEMP 1
#define HUMIDITY_SENSOR_DIGITAL_PIN 3
unsigned long SLEEP_TIME = 30000; // Sleep time between reads (in milliseconds)


#define RELAY_1  3  // Arduino Digital I/O pin number for first relay (second on pin+1 etc)
#define NUMBER_OF_RELAYS 4 // Total number of attached relays
#define RELAY_ON 1  // GPIO value to write to turn on attached relay
#define RELAY_OFF 0 // GPIO value to write to turn off attached relay
// RCSwitch configuration
RCSwitch mySwitch = RCSwitch();
int RCTransmissionPin = 4;
int rcGroup = 1;
int rcAddr = 1;

DHT dht;
float lastTemp;
float lastHum;
boolean metric = true;

unsigned long previousMillis = 0; 
const long interval = 60000;

MySensor gw;
MyMessage msgHum(CHILD_ID_HUM, V_HUM);
MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);

void setup()
{
  // Initialize library and add callback for incoming messages
  gw.begin(incomingMessage, AUTO, false);
  Serial.println();
  Serial.print ("Nom fichier: ");
  Serial.println (__FILE__);
  Serial.println();
  dht.setup(HUMIDITY_SENSOR_DIGITAL_PIN); 
  // Send the sketch version information to the gateway and Controller
  gw.sendSketchInfo("MultiSensorsJMD", "1.0");
  // Initialize RF Transmitter
  mySwitch.enableTransmit( RCTransmissionPin );

  // Fetch relay status
  for (int plug=RELAY_1; plug < NUMBER_OF_RELAYS+RELAY_1 ;plug++) {
    // Register all sensors to gw (they will be created as child devices)
    gw.present(plug, S_LIGHT);
    //gw.sendBatteryLevel(0);
  }
  gw.present(CHILD_ID_HUM, S_HUM);
  gw.present(CHILD_ID_TEMP, S_TEMP);
  metric = gw.getConfig().isMetric;
}


void loop()
{
  unsigned long currentMillis = millis();
  gw.process();
  // Alway process incoming messages whenever possible
  delay(dht.getMinimumSamplingPeriod());

if (currentMillis - previousMillis >= interval) {
  
  previousMillis = currentMillis; 
    
  float temperature = dht.getTemperature();
  if (isnan(temperature)) {
      Serial.println("Failed reading temperature from DHT");
  } else if (temperature != lastTemp) {
    lastTemp = temperature;
    if (!metric) {
      temperature = dht.toFahrenheit(temperature);
    }
    gw.send(msgTemp.set(temperature, 1));
    Serial.print("T: ");
    Serial.println(temperature);
  }
  
  float humidity = dht.getHumidity();
  if (isnan(humidity)) {
      Serial.println("Failed reading humidity from DHT");
  } else if (humidity != lastHum) {
      lastHum = humidity;
      gw.send(msgHum.set(humidity, 1));
      Serial.print("H: ");
      Serial.println(humidity);
  }
 }
}

void incomingMessage(const MyMessage &message) {
  // We only expect one type of message from controller. But we better check anyway.
  if (message.type==2) {
     // Get the sensor for using rcswitch
     if (message.getBool() == 1) {
       switch (message.sensor) {
        case 1:
          mySwitch.switchOn("11111",1);
          break;
        case 2:
          mySwitch.switchOn("11111",2);
          break;
        case 3:
          mySwitch.switchOn("11111",3);
          break;
        case 4:
          mySwitch.switchOn("11111",4);
          break;
        case 5:
          mySwitch.switchOn("11111",1);
          break;
        case 6:
          mySwitch.switchOn("11111",2);
          break;
      }
     } else {
      switch (message.sensor) {
        case 1:
          mySwitch.switchOff("11111",1);
          break;
        case 2:
          mySwitch.switchOff("11111",2);
          break;
        case 3:
          mySwitch.switchOff("11111",3);
          break;
        case 4:
          mySwitch.switchOff("11111",4);
          break;
        case 5:
          mySwitch.switchOff("11111",1);
          break;
        case 6:
          mySwitch.switchOff("11111",2);
          break;
      }
     }

      Serial.print("Recu");

     // Store state in eeprom
     gw.saveState(message.sensor, message.getBool());
     // Write some debug info
     Serial.print("Incoming change for sensor:");
     Serial.print(message.sensor);
     Serial.print(", New status: ");
     Serial.println(message.getBool());
     Serial.print(", Message Type: ");
     Serial.println(message.type);
     //gw.sendBatteryLevel(70);
   }
}
