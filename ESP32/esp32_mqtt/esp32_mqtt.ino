// PubNub MQTT example using ESP32.
#include <WiFi.h>
#include <PubSubClient.h>
// Connection info.
char* ssid = "Skynet";
char* password =  "laconchadetumadre";
char* mqttServer = "192.168.2.114";
int mqttPort = 1883;
char* clientID = "kitchen_light";
char* topic = "cmnd/light/two";
#define LED_BUILTIN 23
 
WiFiClient espClient;
PubSubClient client(espClient);
 

 
void setup() {
 
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  pinMode(LED_BUILTIN, OUTPUT);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect(clientID)) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
 
  client.subscribe(topic);
 
}
 
void loop() {
   if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  if (payload[0]=='1'){
     digitalWrite(LED_BUILTIN, HIGH);
  }
  else if (payload[0]=='0'){ 
     digitalWrite(LED_BUILTIN, LOW);
  }
 
  Serial.println();
  Serial.println("-----------------------");
 
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(clientID)) {
      Serial.println("connected");
      // Subscribe
      client.subscribe(topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
