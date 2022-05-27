//------------------------------------------------
//Arduino Web Server via Ethernet Shield & SD Card
//------------------------------------------------
#include <SPI.h> 
#include <Ethernet.h>
#include <SD.h>
//------------------------------------------------
byte mac[] = {0x90, 0xA2, 0xDA, 0x00, 0x4A, 0xE0};
EthernetServer server(80);
//------------------------------------------------
File HMTL_file;
//=================================================================
void setup()
{
  Serial.begin(9600);
  //---------------------------------------------------------
  Serial.println("Initializing SD card...");
  if(!SD.begin(4))
  {
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card initialized.");
  //---------------------------------------------------------
  if(!SD.exists("webpage.htm"))
  {
    Serial.println("webpage.htm file not found!");
    return;
  }
  Serial.println("webpage.htm file found");
  //---------------------------------------------------------
  Ethernet.begin(mac);
  server.begin();
  Serial.print("Server Started...\nLocal IP: ");
  Serial.println(Ethernet.localIP());
}
//=================================================================
void loop()
{ 
  EthernetClient client = server.available(); 
  if(client)
  { 
    boolean currentLineIsBlank = true;
    while(client.connected())
    { 
      if(client.available())
      { 
        char c = client.read();
        if(c == '\n' && currentLineIsBlank)
        {
          //send HTTP response header to client
          client.println("HTTP/1.1 200 OK\n\rContent-Type: text/html\n\r\n\r");
          //-------------------------------------------------------------------
          //send HTTP file to client
          HMTL_file = SD.open("webpage.htm");
          if(HMTL_file)
          {
            while(HMTL_file.available()) client.write(HMTL_file.read());
            HMTL_file.close();
          }
          break;
        }
        //---------------------------------------------------------------------
        if (c == '\n') currentLineIsBlank = true;
        else if(c != '\r') currentLineIsBlank = false;
      }
    }
    delay(10);
    client.stop();
  }
}