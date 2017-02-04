//simple button GET server code to control servo and arduino pin 5
//for use with IDE 1.0
//open serial monitor to see what the arduino receives
//use the \ slash to escape the " in the html 
//for use with W5100 based ethernet shields
//Powering a servo from the arduino usually DOES NOT WORK.
//note that the below bug fix may be required
// http://code.google.com/p/arduino/issues/detail?id=605 

#include <SPI.h>
#include <Ethernet.h>
#include <RCSwitch.h>

#include <Servo.h> 
Servo myservo;  // create servo object to control a servo 

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //physical mac address
byte ip[] = { 192, 168,0, 177 }; // ip in lan
byte gateway[] = { 192, 168, 1, 1 }; // internet access via router
byte subnet[] = { 255, 255, 255, 0 }; //subnet mask
EthernetServer server(80); //server port

String readString; 
String chaine[]={"led0=0","led0=1","led1=0","led1=1"};
RCSwitch mySwitch = RCSwitch();

//////////////////////

void setup(){

  pinMode(6, OUTPUT); //pin selected to control
  //start Ethernet
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
  //the pin for the servo co
  //enable serial data print 
  Serial.begin(9600); 
  Serial.println("server LED test 1.0"); // so I can keep track of what is loaded
  mySwitch.enableTransmit(2);
}

void loop(){
  // Create a client connection
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        //read char by char HTTP request
        if (readString.length() < 100) 
        {
          //store characters to string 
          readString += c; 
        } 

        //if HTTP request has ended
        if (c == '\n') {

          ///////////////
          //Serial.println(readString); //print to serial monitor for debuging 

          client.println("HTTP/1.1 200 OK"); //send new page
          client.println("Content-Type: text/html");
          client.println();

          client.println("<HTML>");
          client.println("<HEAD>");
          client.println("<meta name='apple-mobile-web-app-capable' content='yes' />");
          client.println("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />");
          client.println("<link rel='stylesheet' type='text/css' href='http://chriscosma.co.cc/a.css' />");
          client.println("<TITLE>Automatisation de la Maison</TITLE>");
          client.println("</HEAD>");
          client.println("<BODY>");
          client.println("<H1>Lumiere Salon</H1>");
          client.println("<hr />");
          client.println("<br />");
          
          client.println("<a href=\"/preset.htm?led0=1\"\">Allumer la lumiere Table Salon</a><br />"); 
          client.println("<a href=\"/preset.htm?led0=0\"\">Eteindre la lumiere Table Salon</a><br />");  
          client.println("<a href=\"/preset.htm?led1=1\"\">Allumer la lumiere Entree Salon</a><br />"); 
          client.println("<a href=\"/preset.htm?led1=0\"\">Eteindre la lumiere Entree Salon</a><br />");         
       
 
          client.println("</BODY>");
          client.println("</HTML>");
 
          delay(1);
          //stopping client
          client.stop();
         
     int i = 0;       
     for (int i=0; i <= 3; i++)
     { 
         int toto = 100;
         Serial.println(chaine[i]);
         if (chaine[i] == readString.substring(16,22))
           {
             toto = i;
           }
          Serial.println(toto);
           
          switch (toto) 
            {
              case 0:
                mySwitch.send(5204, 24); //Off Table Salon
                break;
              case 1:
                mySwitch.send(5201, 24); //On Table Salon
                break;
               case 2:
                mySwitch.send(4436, 24); //Off Entree Salon
                break;
              case 3:
                mySwitch.send(4433, 24); //On Entree Salon
                break;
              case 100:
              Serial.println("erreur");
           }
      
   } 
       
          //clearing string for next read
          readString="";

        }
      }
    }
  }
}
