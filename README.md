# esp8266-weather-station
Simple weather station (temperature and humidity) made with an esp8266 and a DHT11 sensor.
![20220717_153516](https://user-images.githubusercontent.com/88981092/179401297-ec74eba5-8fb1-4e25-8617-734570798e17.jpg)

## Diagram
This is the circuit I found online. I started from here as it's clean and simple.
![NodeMCU-ESP8266-and-DHT11DHT22-Schematic-Diagram](https://user-images.githubusercontent.com/88981092/179401236-158f1562-7662-4d65-98e9-dc4e04b6a4c0.jpeg)

## What's needed
- 1x Esp8266.
- 1x DHT11 sensor
- (Optional if you don't want to solder) 1x Breadboard
- 3x Cables (or jumpers if you opt for the breadboard)
- Arduino IDE
- The sketch of the project (only sketch.ino, index.html it's just a comodity file to check the webpage)

## Libraries used
1. If you use Arduino IDE be sure to [install esp8266 support](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/) first. If you are using linux (I use Arch and I had to set it up by myself) and you have problems making your machine recognize your MCU just ask me how to fix it (I may write a guide in future). 
2. In Arduino IDE library manager:
    1. **DHT sensor library** by **Adafruit**
    2. **Adafruit unified sensors** by **Adafruit**
3. External libraries:
    1. [**ESPAsyncWebServer**](https://github.com/me-no-dev/ESPAsyncWebServer)
    2. [**ESPAsyncTCP**](https://github.com/me-no-dev/ESPAsyncTCP) (required by ESPAsyncWebServer)


## How it works
### Sketch
The mcu will connect to the wifi network you specified in the sketch (replace WiFi params with your credentials) and it prints its ip address in the Arduino IDE serial monitor. It will start an asyncronous server, so that it can handle multiple devices connected to it. The mcu outputs the html page, here treated as a string, to the server, so that it will be viewed once someone connects to its ip address. The mcu uses the DHT11 sensor to record temperature and humidity values and it stores them in two variables. The DHT11 can scan at 1Hz (max), i.e. 1 record every second. Here it's set to scan every 5 seconds. There is also the processPlaceholder() function, that replaces placeholders in the html webpage with the values stored of temperature and humidity.
### Webpage
Everytime someone connects to the root of the webserver, the mcu outputs the webpage. From the webpage we can see the values of the last record, but also a plot chart of the latest twenty records. Of course the content is dynamic thanks to Javascript. The chart is made with [chartjs](https://www.chartjs.org/). Since the esp8266 doesn't have much memory, I didn't bother to hack into its spiffs filesystem to upload chartjs library. So, chartjs is provided with CDN, meaning that you won't see the chart plot if you don't have a connection to the web.

## Demo
Here is a 4x speed demo of the webpage UI to see what to expect.
![final_62d42640aa6907005ec0bb44_886437](https://user-images.githubusercontent.com/88981092/179406486-98ea10e7-c990-4647-b452-5bdb8cc181d1.gif)
