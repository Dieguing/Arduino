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
const float temps[226]={-5.00,-4.80,-4.60,-4.40,-4.20,-4.00,-3.80,-3.60,-3.40,-3.20,-3.00,-2.80,-2.60,-2.40,-2.20,-2.00,-1.80,-1.60,-1.40,-1.20,-1.00,-0.80,-0.60,-0.40,-0.20,0.00,0.20,0.40,0.60,0.80,1.00,1.20,1.40,1.60,1.80,2.00,2.20,2.40,2.60,2.80,3.00,3.20,3.40,3.60,3.80,4.00,4.20,4.40,4.60,4.80,5.00,5.20,5.40,5.60,5.80,6.00,6.20,6.40,6.60,6.80,7.00,7.20,7.40,7.60,7.80,8.00,8.20,8.40,8.60,8.80,9.00,9.20,9.40,9.60,9.80,10.00,10.20,10.40,10.60,10.80,11.00,11.20,11.40,11.60,11.80,12.00,12.20,12.40,12.60,12.80,13.00,13.20,13.40,13.60,13.80,14.00,14.20,14.40,14.60,14.80,15.00,15.20,15.40,15.60,15.80,16.00,16.20,16.40,16.60,16.80,17.00,17.20,17.40,17.60,17.80,18.00,18.20,18.40,18.60,18.80,19.00,19.20,19.40,19.60,19.80,20.00,20.20,20.40,20.60,20.80,21.00,21.20,21.40,21.60,21.80,22.00,22.20,22.40,22.60,22.80,23.00,23.20,23.40,23.60,23.80,24.00,24.20,24.40,24.60,24.80,25.00,25.20,25.40,25.60,25.80,26.00,26.20,26.40,26.60,26.80,27.00,27.20,27.40,27.60,27.80,28.00,28.20,28.40,28.60,28.80,29.00,29.20,29.40,29.60,29.80,30.00,30.20,30.40,30.60,30.80,31.00,31.20,31.40,31.60,31.80,32.00,32.20,32.40,32.60,32.80,33.00,33.20,33.40,33.60,33.80,34.00,34.20,34.40,34.60,34.80,35.00,35.20,35.40,35.60,35.80,36.00,36.20,36.40,36.60,36.80,37.00,37.20,37.40,37.60,37.80,38.00,38.20,38.40,38.60,38.80,39.00,39.20,39.40,39.60,39.80,40.00};
const float volts[226]={3.175,3.153,3.131,3.109,3.088,3.066,3.045,3.025,3.004,2.984,2.964,2.944,2.924,2.904,2.885,2.866,2.847,2.828,2.810,2.791,2.773,2.755,2.737,2.720,2.702,2.685,2.668,2.651,2.634,2.618,2.601,2.585,2.569,2.553,2.538,2.522,2.506,2.491,2.476,2.461,2.446,2.432,2.417,2.403,2.388,2.374,2.360,2.347,2.333,2.319,2.306,2.293,2.279,2.266,2.253,2.241,2.228,2.215,2.203,2.191,2.179,2.166,2.155,2.143,2.131,2.119,2.108,2.097,2.085,2.074,2.063,2.052,2.041,2.030,2.020,2.009,1.999,1.989,1.978,1.968,1.958,1.948,1.938,1.929,1.919,1.909,1.900,1.890,1.881,1.872,1.863,1.854,1.845,1.836,1.827,1.818,1.810,1.801,1.792,1.784,1.776,1.767,1.759,1.751,1.743,1.735,1.727,1.719,1.712,1.704,1.696,1.689,1.681,1.674,1.667,1.659,1.652,1.645,1.638,1.631,1.624,1.617,1.610,1.604,1.597,1.590,1.584,1.577,1.571,1.564,1.558,1.552,1.545,1.539,1.533,1.527,1.521,1.515,1.509,1.503,1.497,1.492,1.486,1.480,1.475,1.469,1.464,1.458,1.453,1.447,1.442,1.437,1.431,1.426,1.421,1.416,1.411,1.406,1.401,1.396,1.391,1.386,1.381,1.377,1.372,1.367,1.363,1.358,1.353,1.349,1.344,1.340,1.336,1.331,1.327,1.323,1.318,1.314,1.310,1.306,1.302,1.297,1.293,1.289,1.285,1.281,1.278,1.274,1.270,1.266,1.262,1.258,1.255,1.251,1.247,1.244,1.240,1.236,1.233,1.229,1.226,1.222,1.219,1.215,1.212,1.209,1.205,1.202,1.199,1.196,1.192,1.189,1.186,1.183,1.180,1.177,1.173,1.170,1.167,1.164,1.161,1.158,1.156,1.153,1.150,1.147};

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
  width: 1200px;
  height: 700px;
  ">
<!-- Adding a slider for controlling data rate -->
<div>
  <input type="range" min="1" max="10" value="5" id="dataRateSlider" oninput="sendDataRate()" />
  <label for="dataRateSlider" id="dataRateLabel">Rate: 0.2Hz</label>
  <h1>Light control</h1>
  <input type=button onClick=window.open('http://192.168.178.28/cm?cmnd=Power%20on','demo','width=550,height=400,left=150,top=200,toolbar=0,status=0,'); value='Power ON'>
  <input type=button onClick=window.open('http://192.168.178.28/cm?cmnd=Power%20off','demo','width=550,height=400,left=150,top=200,toolbar=0,status=0,'); value='Power OFF'>
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
          title: {
            display: true,
            text: "Room temperature"
          },
          legend: {display: false},
          scales: {
          yAxes: [{ticks: {min: 15, max:35, stepSize: 1}}],
          }
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
    temp_val=0;
     // Serial.println(bmp.readTemperature());
     String json = "{\"value\":";
     for(int i=0; i<100; i++)
     {
      temp_val += analogRead(analogPin)*3.83/4095;  // read the input pin
     }
     temp_val/=100;
     
     for (int i=0; i<226; i++)
     {
        if (temp_val>volts[i])
        {
          Serial.println(temps[i]);
          json += temps[i];
          temp_val=0;
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
