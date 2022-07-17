#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>


//Wifi params
const char* ssid = "your-ssid-here";
const char* password = "yourr-passw-here";

//D1 is connected to DHT11 data channel
#define DHTPIN      5
#define DHTTYPE     DHT11

//To interact with DHT
DHT dht(DHTPIN, DHTTYPE);

//Global variables to be updated in loop()
float temperature = 0.0;
float humidity = 0.0;

//Create AsyncWebServer object on port 80
AsyncWebServer server(80);

//Store last time DHT was updated
unsigned long previousMillis = 0;

// Interval (millis) among DHT readings (max 1read/1s = 1Hz)
// Interval set as 5 seconds.
const long interval = 5000; 

//WebPage to be displayed (uses ESPAsyncWebServer)
const char index_html[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
  <head>
    <meta name='viewport' content='width=device-width, initial-scale=1'>
    <meta charset='UTF-8'>
    <script src='https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.9.4/Chart.js'></script>

    <title>ESP8266 - DHT11</title>
    <style>
    *{
      margin: 0 auto;
      position: relative;
      font-size: 30px;
    }
    body{
      padding: 40px 15px;
      display: flex;
      flex-direction: column;
    }
    h1{
      font-size: 40px;
    }
    canvas{
      -moz-user-select: none;
      -webkit-user-select: none;
      -ms-user-select: none;
    }
    </style>
  </head>

  <body>
    <h1>DHT11 Temperature & Humidity</h1>
    <div style='padding-top: 20px; width: 320px;'>
      <span>Temperature:</span>
      <span id='temperature'>%TEMPERATURE%</span>
      <span>°C</span>
    </div>
    <div style='padding-top: 20px;'>
      <span>Humidity:</span>
      <span id='humidity'>%HUMIDITY%</span>
      <span>%</span>
    </div>
    <canvas id='chart' style='width:90%; margin-top: 20px;'></canvas>

  </body>

  <script>
    var tValue;
    var hValue;
    var label;
    var labelsNum = 0;
    var ctx = document.getElementById('chart').getContext('2d');   

    var chart = new Chart(ctx, {
      type: 'line',
      data: {
        datasets:[
          {
            label: 'Temperature',
            borderColor: 'rgb(255, 38, 38)',
          },
          {
            label: 'Humidity',
            borderColor: 'rgb(51, 80, 255)',
          }
        ]
      },
      options: {}
    });

    setInterval(function ( ) {
      // https://www.w3schools.com/xml/xml_http.asp
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById('temperature').innerHTML = this.responseText;
          tValue = this.responseText;
        }
      };
      xhttp.open('GET', '/temperature', true);
      xhttp.send();
    }, 5000);

    setInterval(function ( ) {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById('humidity').innerHTML = this.responseText;
          hValue = this.responseText;
        }
      };
      xhttp.open('GET', '/humidity', true);
      xhttp.send();
    }, 5000);
  
    setInterval(function(){
      updateChart();
    },5000);

    function updateChart(){
      if(labelsNum == 20){
        chart.data.labels.shift();
        chart.data.datasets[0].data.shift();
        chart.data.datasets[1].data.shift();
      }
      else{
        labelsNum++;
      }
      chart.data.labels.push(new Date().toLocaleTimeString());
      chart.data.datasets[0].data.push(tValue);
      chart.data.datasets[1].data.push(hValue);
      chart.update();
    }
  </script>
</html>
)=====";


//Function that replaces %placeholders% of html page with DHT values
String processPlaceholder(const String& var){
  if(var == "TEMPERATURE"){
    return String(temperature);
  }
  else if(var == "HUMIDITY"){
    return String(humidity);
  }
  return String();
}



void setup() {
  Serial.begin(115200);
  dht.begin();
  
  // Connect to WiFi
  WiFi.begin(ssid, password);

  //print "." while not connected
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  // Print the IP address and MAC
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.macAddress());

  // Async Server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processPlaceholder);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(temperature).c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(humidity).c_str());
  });

  // Start server
  server.begin();
}


void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save last update time
    previousMillis = currentMillis;
    // read temperature
    float newTemp = dht.readTemperature();
    if (isnan(newTemp)) {
      Serial.println("Failed to read from DHT sensor!");
    }
    else {
      temperature = newTemp;
    }
    // read humidity
    float newHum = dht.readHumidity();
    if (isnan(newHum)) {
      Serial.println("Failed to read from DHT sensor!");
    }
    else {
      humidity = newHum;
    }
  }
}
