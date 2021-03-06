#include <WiFi.h>
#include <WiFiUdp.h>
#define led 2
int status = WL_IDLE_STATUS;
char ssid[] = "Skynet"; //  your network SSID (name)
char pass[] = "laconchadetumadre";    // your network password (use for WPA, or use as key for WEP)

unsigned int localPort = 2390;      // local port to listen on

char packetBuffer[255]; //buffer to hold incoming packet
char  ReplyBuffer[] = "acknowledged";       // a string to send back

WiFiUDP Udp;

void setup() {
  //Initialize serial and wait for port to open:
   pinMode(led, OUTPUT);
   Serial.begin(9600);
   while (!Serial);
   
   // attempt to connect to Wifi network:
   while (status != WL_CONNECTED) 
   {
      Serial.print("Attempting to connect to SSID: ");
      Serial.println(ssid);
      // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
      status = WiFi.begin(ssid,pass);
      
      // wait 10 seconds for connection:
      delay(10000);
   }
   Serial.println("Connected to wifi");
   printWifiStatus();
   
   Serial.println("\nStarting connection to server...");
   // if you get a connection, report back via serial:
   Udp.begin(localPort);
}

void loop() {

   // if there's data available, read a packet
   int packetSize = Udp.parsePacket();
   if (packetSize) 
   {
      Serial.print("Received packet of size ");
      Serial.println(packetSize);
      Serial.print("From ");
      IPAddress remoteIp = Udp.remoteIP();
      Serial.print(remoteIp);
      Serial.print(", port ");
      Serial.println(Udp.remotePort());
      
      // read the packet into packetBufffer
      int len = Udp.read(packetBuffer, 255);
      if (len > 0) {
      packetBuffer[len] = 0;
      }
      Serial.print("Contents: ");
      Serial.println(packetBuffer);
      
      if (!strcmp(packetBuffer,"ON"))
      {
         digitalWrite(led,HIGH);
         Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
         Udp.printf("Encendido");
         Udp.endPacket();
      }else if (!strcmp(packetBuffer,"OFF")){
         digitalWrite(led,LOW);
         Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
         Udp.printf("Apagado");
         Udp.endPacket();
      }  
   }
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
