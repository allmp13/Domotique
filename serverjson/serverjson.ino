/*
 * UIPEthernet EchoServer example.
 *
 * UIPEthernet is a TCP/IP stack that can be used with a enc28j60 based
 * Ethernet-shield.
 *
 * UIPEthernet uses the fine uIP stack by Adam Dunkels <adam@sics.se>
 *
 *      -----------------
 *
 * This Hello World example sets up a server at 192.168.1.6 on port 1000.
 * Telnet here to access the service.  The uIP stack will also respond to
 * pings to test if you have successfully established a TCP connection to
 * the Arduino.
 *
 * This example was based upon uIP hello-world by Adam Dunkels <adam@sics.se>
 * Ported to the Arduino IDE by Adam Nielsen <malvineous@shikadi.net>
 * Adaption to Enc28J60 by Norbert Truchsess <norbert.truchsess@t-online.de>
 */

#include <UIPEthernet.h>

EthernetServer server = EthernetServer(1000);

void setup()
{
  Serial.begin(9600);

  uint8_t mac[6] = {0x00,0x01,0x02,0x03,0x04,0x05};
  IPAddress myIP(192,168,0,6);

  Ethernet.begin(mac,myIP);
  Serial.println(Ethernet.localIP());

  server.begin();
}

void loop()
{
  float temperatureIndoor;
  float temperatureOutdoor;
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: application/json;charset=utf-8");
          client.println("Server: Arduino");
          client.println("Connnection: close");
          client.println();
 
          temperatureIndoor = 22.77;
          temperatureOutdoor = 15.55;
 
          client.print("{\"arduino\":[{\"location\":\"indoor\",\"celsius\":\"");
          client.print(temperatureIndoor);
          client.print("\"},");
          client.print("{\"location\":\"outdoor\",\"celsius\":\"");
          client.print(temperatureOutdoor);
 
          client.print("\"}]}");
          client.println();
          break;
}
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
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
    Serial.println("client disonnected");

  }
 
}

