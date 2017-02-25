/*
 PROJECT: MySensors / RGB test for Light & Sensor
 PROGRAMMER: AWI/GizMoCuz
 DATE: september 27, 2015/ last update: October 10, 2015
 FILE: AWI_RGB.ino
 LICENSE: Public domain
 
 Hardware: Nano and MySensors 1.5
   
 Special:
  uses Fastled library with NeoPixel (great & fast RBG/HSV universal library)       https://github.com/FastLED/FastLED
 
 Remarks:
  Fixed node-id
  Added option to request/apply last light state from gateway
 
 Domoticz typicals - 2015 10 10:
  - Domoticz is using HUE values internally, there might be a slight difference then using direct RGB colors.
*/
//#define MY_DEBUG
#define MY_RADIO_NRF24
//#define MY_NODE_ID 39
#include <MySensors.h>
#include <SPI.h>
#include <FastLED.h>
 
 
 
const int stripPin = 3 ;                  // pin where 2812 LED strip is connected
 
const int numPixel = 30 ;                  // set to number of pixels
 
//#define NODE_ID 254                       // fixed MySensors node id
 
#define CHILD_ID 0                  // Child Id's
 
CRGB leds[numPixel];
 
char actRGBvalue[] = "000000";               // Current RGB value
uint16_t actRGBbrightness = 0xFF ;         // Controller Brightness
int actRGBonoff=0;                        // OnOff flag
 
//MyMessage lastColorStatusMsg(CHILD_ID,V_VAR1);
 
void setup() {
  Serial.begin(115200);
  FastLED.addLeds<NEOPIXEL, stripPin >(leds, numPixel); // initialize led strip
 
//  begin(receive, MY_NODE_ID, false);      // initialize MySensors
 
 
  // Flash the "hello" color sequence: R, G, B, black.
  colorBars();
 
  //Request the last stored colors settings
  //request(CHILD_ID, V_VAR1);
}
 
void loop() {
  //process();                       // wait for incoming messages
}
 
void presentation()
{
  sendSketchInfo("JMD LED", "1.1");
  present(CHILD_ID, S_RGB_LIGHT);        // present to controller
}
 
void colorBars()
{
  SendColor2AllLEDs( CRGB::Red );   FastLED.show(); delay(500);
  SendColor2AllLEDs( CRGB::Green ); FastLED.show(); delay(500);
  SendColor2AllLEDs( CRGB::Blue );  FastLED.show(); delay(500);
  SendColor2AllLEDs( CRGB::Black ); FastLED.show(); delay(500);
}
 
void SendColor2AllLEDs(const CRGB lcolor)
{
  for(int i = 0 ; i < numPixel ; i++) {
    leds[i] = lcolor;
  }
}
 
/*void SendLastColorStatus()
{
  String cStatus=actRGBvalue+String("&")+String(actRGBbrightness)+String("&")+String(actRGBonoff);
  send(lastColorStatusMsg.set(cStatus.c_str()));
}*/
 
String getValue(String data, char separator, int index)
{
 int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;
  for(int i=0; i<=maxIndex && found<=index; i++){
  if(data.charAt(i)==separator || i==maxIndex){
  found++;
  strIndex[0] = strIndex[1]+1;
  strIndex[1] = (i == maxIndex) ? i+1 : i;
  }
 }
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
 
void receive(const MyMessage &message) {
  if (message.type == V_RGB) {            // check for RGB type
    Serial.println( message.getString());  
    actRGBonoff=1;
    String toto=message.getString();
    toto =toto.substring(1,toto.length());
    Serial.println(toto.c_str());
    SendColor2AllLEDs(strtol(toto.c_str(), NULL, 16));
    
    //SendLastColorStatus();
  }
 /* else if (message.type == V_DIMMER) {           // if DIMMER type, adjust brightness
    actRGBonoff=1;
    actRGBbrightness = map(message.getLong(), 0, 100, 0, 255);
    FastLED.setBrightness( actRGBbrightness );
    //SendLastColorStatus();
  }
  else if (message.type == V_STATUS) {           // if on/off type, toggle brightness
    actRGBonoff = message.getInt();
    FastLED.setBrightness((actRGBonoff == 1)?actRGBbrightness:0);
   // SendLastColorStatus();
  }
  else if (message.type==V_VAR1) {            // color status
    String szMessage=message.getString();
    strcpy(actRGBvalue, getValue(szMessage,'&',0).c_str());
    actRGBbrightness=atoi(getValue(szMessage,'&',1).c_str());
    actRGBonoff=atoi(getValue(szMessage,'&',2).c_str());
    SendColor2AllLEDs(strtol(actRGBvalue, NULL, 16));
    FastLED.setBrightness((actRGBonoff == 1)?actRGBbrightness:0);
  }*/
    if (message.type == V_VAR1) { 
       FastLED.setBrightness(map(message.getInt(),0,100,0,255));
      }
  FastLED.show();
}
