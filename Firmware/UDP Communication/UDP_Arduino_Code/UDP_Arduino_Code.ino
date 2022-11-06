#include <W600WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>

#define APPSSID "MiniCopter"
#define PASSWORD  ""
#define UDP_PORT 4210

WiFiUDP UDP;
char incomingPacket[100];

void setup() {
  WiFi.softAP(APPSSID, PASSWORD);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  // Begin listening to UDP port
  UDP.begin(UDP_PORT);
  Serial.print("Listening on UDP port ");
  Serial.println(UDP_PORT);
}

void loop() {
  read_udp_packet();
}

void read_udp_packet(){
  int packetSize = UDP.parsePacket();
  if (packetSize){
    // receive incoming UDP packets
    int len = UDP.read(incomingPacket, 255);
    if (len > 0) incomingPacket[len] = 0;

    //===========================================
    //decode incomingPacket using a json decoder
    //===========================================
  }
}
