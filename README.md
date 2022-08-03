# esp8266-weather-station
Simple weather station (temperature and humidity) made with an esp8266 and a DHT11 sensor.
![20220717_153516](https://user-images.githubusercontent.com/88981092/179401297-ec74eba5-8fb1-4e25-8617-734570798e17.jpg)

## Circuit
Follow this. It's clean and simple.

![NodeMCU-ESP8266-and-DHT11DHT22-Schematic-Diagram](https://user-images.githubusercontent.com/88981092/179401236-158f1562-7662-4d65-98e9-dc4e04b6a4c0.jpeg)

## What's needed
- 1x Esp8266.
- 1x DHT11 sensor
- (Optional if you don't want to solder) 1x Breadboard
- 3x Cables (or jumpers if you opt for the breadboard)
- Arduino IDE
- Clone this repo: you need both the sketch and the data folder.

## Libraries used
1. If you use Arduino IDE be sure to [install esp8266 support](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/) first.
2. In Arduino IDE library manager:
    - **DHT sensor library** by **Adafruit**
    - **Adafruit unified sensors** by **Adafruit**
3. External libraries (look into each library's readme file to see further dependancies):
    - [**ESPAsyncWebServer**](https://github.com/me-no-dev/ESPAsyncWebServer)

## Project Structure
Using LittleFS uploader you will upload actual html, css and javascript files on the esp8266. As it works under Arduino IDE, just open a new project -> paste the sketch.ino content -> press CTRL+k -> from the project folder create a new folder named "data" -> copy the data folder content of this repo on your data folder. At the end, assuming you are working from a "sketch" folder you will have this situation:
```
sketch/
  |
  ꜔ sketch.ino
  ꜔ data/
      |
      ꜔ index.html
      ꜔ style.css
      ꜔ main.js
      ꜔ chart.min.js

```
Follow the LittleFS usage guide to flash the data/ content on the esp filesystem.

## How it works
### Sketch
The mcu will connect to the wifi network you specified in the sketch (replace WiFi params with your credentials) and it will print its ip address (also its MAC address, so that you can use it for address binding from your router) in the Arduino IDE serial monitor. Use the ip address to connect to the mcu. Thanks to **ESPAsyncWebServer** the mcu will act like a web-server, and it will handle multiple devices. ESPAsyncWebServer works with **LittleFS**, so that it will provide the needed content (the client side files) on request.
The mcu uses the DHT11 sensor to record temperature and humidity values and it stores them in two variables. The DHT11 can scan at 1Hz (max), i.e. 1 record every second. Here it's set to scan every 5 seconds. There function _processPlaceholder()_ replaces placeholders in the html webpage with the values stored of temperature and humidity.
### Webpage
Everytime someone connects to the root of the webserver, the mcu outputs the webpage. From the webpage we can see the values of the last record, but also a plot chart of the latest twenty records. Of course the content is dynamic thanks to Javascript. The chart is made with [chartjs](https://www.chartjs.org/). The chart canvas along with the rest of the web-interface is responsive thanks to the use of media queries.

## Custom name
The last commit introduced the possibility to open the webpage with a custom name as the header. The way it's done is via URL.
```
<ESP-IP-ADDRESS>[/?name=name-value]

// Fields
name-value : everything but blank

// Defaults
name-value : "ESP8266 - Weather Station"
```

## Demo
Sorry if If i recorded from mobile, I was having problems with screencast.
![WGIQ65Z - Imgur](https://user-images.githubusercontent.com/88981092/181226971-93f95b2f-7a33-41bb-affc-9e0d02baa40d.png)

![Schermata del 2022-08-03 18-40-21](https://user-images.githubusercontent.com/88981092/182664129-b250b601-da09-4127-b70c-d6454c38e97d.png)


https://user-images.githubusercontent.com/88981092/181229246-fc35eae6-2822-4223-bb1c-f5510a64c6d8.mp4

