#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <LittleFS.h>



//Wifi params
const char* ssid = "TP-LINK_81CC";
const char* password = "67652594";

// D1 is connected to DHT11 data channel
#define DHTPIN      5

// DHT handler
DHT dht(DHTPIN, DHT11);

// State variables
float temperature = 0.0;
float humidity = 0.0;

// AsyncWebServer object on port 80
AsyncWebServer server(80);

// Store last time DHT was updated
// unsigned long previousMillis = 0;

// Interval (millis) among DHT readings (max 1read/1s = 1Hz)
// Interval set as 5 seconds.
//const long interval = 5000;
const unsigned short int interval = 5000;


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

  if (!LittleFS.begin()) {
    Serial.println("An error has occurred while mounting LittleFS");
  }
  
  // Connect to WiFi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.macAddress());

  // Web Server Root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html", String(), false, processPlaceholder);
  });
  server.serveStatic("/", LittleFS, "/");

  // Requests
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
  /*unsigned long currentMillis = millis();
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
  }*/
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
  delay(interval);
}
