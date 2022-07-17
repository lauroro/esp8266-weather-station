# esp8266-weather-station
Simple weather station (temperature and humidity) made with an esp8266 and a DHT11 sensor.
![20220717_153516](https://user-images.githubusercontent.com/88981092/179401297-ec74eba5-8fb1-4e25-8617-734570798e17.jpg)
## Diagram
This is the circuit I found online. I started from here as it's clean and simple.
![NodeMCU-ESP8266-and-DHT11DHT22-Schematic-Diagram](https://user-images.githubusercontent.com/88981092/179401236-158f1562-7662-4d65-98e9-dc4e04b6a4c0.jpeg)
## Libraries used
1. If you use Arduino IDE be sure to [install esp8266 support](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/) first. If you are using linux (I use Arch) and you have problems making your machine recognize your MCU just ask me how to fix it (I may write a guide in future). 
2. In Arduino IDE library manager:
2.1 **DHT sensor library** by **Adafruit**
