// PubNub MQTT example using ESP32.
#include <WiFi.h>
#include <PubSubClient.h>

// WiFi Connection info.
char* ssid = "Skynet_2.0";
char* password =  "5anT1BrDi";

//MQTT connection info
char* mqttServer = "192.168.178.27";
int mqttPort = 1883;
char* clientID = "thermometer";
char* topicA = "cmnd/temp/celsius";
char* topicB = "cmnd/temp/fahrenheit";
 
WiFiClient espClient;
PubSubClient client(espClient);
 
#define LED 2

char grad_c[]="   ";
char grad_f[]="   "; 

float Kelvin;
uint16_t  C_grad, F_grad;
uint8_t dk=6;

long lastMsg = 0;

void setup() {
 
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  pinMode(LED,OUTPUT);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
    digitalWrite(LED, !digitalRead(LED));
  }
  Serial.println("Connected to the WiFi network");
  
  digitalWrite(LED, LOW);
 
  client.setServer(mqttServer, mqttPort);
}
 
void loop() 
{
  long now = millis();
  if (now - lastMsg > 10000) 
  {
     lastMsg = now;
    
     Kelvin = analogRead(A0) * 0.0808 - dk; // Read analog voltage and convert it to Kelvin [ADC_val * (3.31V/4096) / 10e-3V/K]
     Serial.println(Kelvin);
     F_grad = (Kelvin - 273) * 9 / 5 + 32;
     C_grad = Kelvin - 273;                      // Convert Kelvin to degree Celsius

     if(C_grad < 0)
     {
         C_grad = abs(C_grad);
         grad_c[0]='-';
     }else
         grad_c[0]  = (C_grad / 100) % 10 + 48;
     grad_c[1]  = (C_grad / 10) % 10 + 48;
     grad_c[2]  = C_grad % 10 + 48;
     
     grad_f[0]  = (F_grad / 100) % 10 + 48;
     grad_f[1]  = (F_grad / 10) % 10 + 48;
     grad_f[2]  = F_grad % 10 + 48;
     
     client.connect(clientID);  
     client.publish(topicA, grad_c);
     client.publish(topicB, grad_f);
     client.disconnect();
  }
}
