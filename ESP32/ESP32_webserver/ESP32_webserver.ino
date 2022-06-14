// PubNub MQTT example using ESP32.
#include <WiFi.h>
#include <WebServer.h>
#include <Ticker.h>
#include <WebSocketsServer.h>

//Defines
#define analogPin 34

Ticker timer;
bool get_data = false;
float temp_val;

// WiFi Connection info.
const char* ssid = "Skynet_2.0";
const char* password =  "5anT1BrDi";
const float temps[46]={-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40};
const float volts[46]={3.17, 3.07, 2.96, 2.87, 2.77, 2.69, 2.60, 2.52, 2.45, 2.37, 2.31, 2.24, 2.18, 2.12, 2.06, 2.01, 1.96, 1.91, 1.86, 1.82, 1.78, 1.74, 1.70, 1.66, 1.62, 1.59, 1.56, 1.53, 1.50, 1.47, 1.44, 1.42, 1.39, 1.37, 1.34, 1.32, 1.30, 1.28, 1.26, 1.24, 1.23, 1.21, 1.19, 1.18, 1.16, 1.15};

// Running a web server
WebServer server(80);

// Set web server port number to 80
WebSocketsServer webSocket = WebSocketsServer(81);

// Serving a web page (from flash memory)
// formatted as a string literal!
char webpage[] PROGMEM = R"=====(
<html>
<!-- Adding a data chart using Chart.js -->
<head>
  <script src='https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.9.4/Chart.js'></script>
</head>
<body onload="javascript:init()" style="
  width: 800px;
  height: 400px;
  ">
<!-- Adding a slider for controlling data rate -->
<div>
  <input type="range" min="1" max="10" value="5" id="dataRateSlider" oninput="sendDataRate()" />
  <label for="dataRateSlider" id="dataRateLabel">Rate: 0.2Hz</label>
</div>
<hr />
<div>
  <canvas id="line-chart" width="1200" height="700"></canvas>
</div>
<!-- Adding a websocket to the client (webpage) -->
<script>
  var webSocket, dataPlot;
    var maxDataPoints = 20;
    function removeData(){
      dataPlot.data.labels.shift();
      dataPlot.data.datasets[0].data.shift();
    }
    function addData(label, data) {
      if(dataPlot.data.labels.length > maxDataPoints) removeData();
      dataPlot.data.labels.push(label);
      dataPlot.data.datasets[0].data.push(data);
      dataPlot.update();
    }
    function init() {
      webSocket = new WebSocket('ws://' + window.location.hostname + ':81/');
      dataPlot = new Chart(document.getElementById("line-chart"), {
        type: 'line',
        data: {
          labels: [],
          datasets: [{
            data: [],
            label: "Temperature (C)",
            borderColor: "#3e95cd",
            fill: false
          }]
        },
        options: {
          legend: {display: false},
        }
      });
      webSocket.onmessage = function(event) {
        var data = JSON.parse(event.data);
        var today = new Date();
        var t = today.getHours() + ":" + today.getMinutes() + ":" + today.getSeconds();
        addData(t, data.value);
      }
    }
    function sendDataRate(){
      var dataRate = document.getElementById("dataRateSlider").value;
      webSocket.send(dataRate);
      dataRate = 1.0/dataRate;
      document.getElementById("dataRateLabel").innerHTML = "Rate: " + dataRate.toFixed(2) + "Hz";
    }
</script>
</body>
</html>
)=====";

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as input
  pinMode(analogPin, INPUT);

  
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/",[](){
    server.send_P(200, "text/html", webpage);
  });
  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  
  timer.attach(5, getData);
}

void loop(){
    
  webSocket.loop();
  server.handleClient();

  if(get_data) 
  {
     // Serial.println(bmp.readTemperature());
     String json = "{\"value\":";
     temp_val = analogRead(analogPin)*3.83/4095;  // read the input pin
     for (int i=0; i<47; i++)
     {
        if (temp_val>volts[i])
        {
          Serial.println(temps[i]);
          json += temps[i];
          break;
        } 
     }
     json += "}";
     webSocket.broadcastTXT(json.c_str(), json.length());
     get_data = false;
  }
}

void getData() {
  get_data = true;
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length){
  // Do something with the data from the client
  if(type == WStype_TEXT){
    float dataRate = (float) atof((const char *) &payload[0]);
    timer.detach();
    timer.attach(dataRate, getData);
  }
}
