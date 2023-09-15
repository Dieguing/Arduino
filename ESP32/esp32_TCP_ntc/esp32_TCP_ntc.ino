// PubNub MQTT example using ESP32.
#include <WiFi.h>
#include <PubSubClient.h>
#define adj 1

// WiFi Connection info.
const char* ssid = "Skynet_2.0";
const char* password =  "5anT1BrDi";
const int port = 5050;

const float temps[46]={-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40};
const float volts[46]={3.17, 3.07, 2.96, 2.87, 2.77, 2.69, 2.60, 2.52, 2.45, 2.37, 2.31, 2.24, 2.18, 2.12, 2.06, 2.01, 1.96, 1.91, 1.86, 1.82, 1.78, 1.74, 1.70, 1.66, 1.62, 1.59, 1.56, 1.53, 1.50, 1.47, 1.44, 1.42, 1.39, 1.37, 1.34, 1.32, 1.30, 1.28, 1.26, 1.24, 1.23, 1.21, 1.19, 1.18, 1.16, 1.15};

WiFiServer server(port);
 
#define LED 2

char grad_c[]="   ";
char grad_f[]="   ";
String thisChar = "";

uint16_t  C_grad, F_grad, avg[30],flag_send=0;
float temp_val;
boolean alreadyConnected = false; // whether or not the client was connected previously
long lastMsg = 0;

void setup() {
  Serial.begin(115200);
  WiFi.begin((char*)ssid, password);
  pinMode(LED,OUTPUT);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
    digitalWrite(LED, !digitalRead(LED));
  }
  Serial.println("Connected to the WiFi network");
  server.begin();
  Serial.println("Server started");
  
  digitalWrite(LED, LOW);
}
 
void loop() 
{
  WiFiClient client = server.available();
  
  if (client) {
    if (client.available() > 0) {
      // read the bytes incoming from the client:
      thisChar = client.readString();

      for (int i=0; i<sizeof(grad_c);i++){
        //server.write(thisChar[i]);
        client.print(grad_c[i]);
      }
      
      // echo the bytes to the server as well:
      
    }
  }
  
  long now = millis();
  if (now - lastMsg > 100) 
  {
     lastMsg = now;
     int avg_t=0;
     
     for (int i=0; i<29; i++)
     {
      avg[i] = avg[i+1];
     }
     
     temp_val = analogRead(34) * 3.83/4095; // Read analog voltage and convert it to Kelvin [ADC_val * (3.31V/4096) / 10e-3V/K]
     
     for (int i=0; i<47; i++)
     {
        if (temp_val>volts[i])
        {
          //Serial.println(temps[i]);
          avg[29] = temps[i];
          break;
        } 
     }

     for (int i=0; i<30; i++)
     {
      avg_t += avg[i];
     }
     
     C_grad = (avg_t / 30) - adj;
     
     flag_send++; 
  }

  if (flag_send==30)
  {
    F_grad = C_grad * 9 / 5 + 32;

     if(C_grad < 0)
     {
         C_grad = abs(C_grad);
         grad_c[0]='-';
     }else
         grad_c[0]  = ' ';
     grad_c[1]  = (C_grad / 10) % 10 + 48;
     grad_c[2]  = C_grad % 10 + 48;
     
     grad_f[0]  = (F_grad / 100) % 10 + 48;
     grad_f[1]  = (F_grad / 10) % 10 + 48;
     grad_f[2]  = F_grad % 10 + 48;

     //digitalWrite(LED, !digitalRead(LED));
     flag_send=0;
  } 
}
