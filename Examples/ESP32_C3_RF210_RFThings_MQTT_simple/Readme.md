# ESP32_C3_RF210_RFThings_MQTT_simple
Link: [https://github.com/PhamDuyAnh/RFThings_how_to_use/new/main/Examples/ESP32_C3_RF210_RFThings_MQTT_simple](https://github.com/PhamDuyAnh/RFThings_how_to_use/new/main/Examples/ESP32_C3_RF210_RFThings_MQTT_simple)

Is a simple example
ESP32C3 will connect to the RAK3172 via the UART port. Use AT cammand to request sensor information, GNSS, etc. then analyze and publish all to MQTT server

### Quick start
To start, open the **ESP32_C3_RF210_RFThings_MQTT_simple.ino** file and edit the content in the following files:
*  **01_Wifi.ino**: this file contains the definitions and functions of Wifi, edit **your AP ssid** & **your AP password** according to the AP you want the RF210 to connect to. This connection is used for RF210 to connect to the internet (line 2 & 3)
```
const char* ssid = "your AP ssid";
const char* pass = "your AP password";
```
*  **04_MQTT.ino**: this file contains the definitions and functions of MQTT, edit your MQTT server info (line 27..).
```
#define mqtt_tcpssl 0                         // 0 is don't use TCP SSL, default port 1883
const char* mqtt_server = "mqtt server";      // replace with your MQTT URL
const char* mqtt_username = "mqtt username";  // replace with your Username
const char* mqtt_password = "mqtt password";  // replace with your Password
```
*  The **LEDStatus**, **PubSubClient** library is already available in the **src** folder
*  Other libraries are available when installing **Arduino IDE** and **ESP32 by Espressif Systems**
  
**Okay**, now you can select Upload, then press RESET (ESP_EN1) and wait for the results.
Remember to open the Serial Monitor window to read the results returned by RF210 

Use **MQTT Explorer** or another program (app) to view, pub/sub topic on MQTT Server

*  The **will** topis: is a topic that reports the device's online status. When the device connect timeout (keep align), will topic will be changed value by MQTT server.
*  The **tem, hum,.. alt, lon...** topics: are topics that store data of sensors and gnss
*  Master topic is **RFThings-XXXXXXXXXXXX**, [XXXXXXXXXXXX] is the FuseMAC of the ESP32 chip

![MQTT Explorer](https://github.com/PhamDuyAnh/RFThings_how_to_use/blob/main/REF/MQTT-Explorer.png)

### Note
*  **RAK3172 AT command** --> [..github.com/PhamDuyAnh/RFThings_how_to_use...](https://github.com/PhamDuyAnh/RFThings_how_to_use/tree/main#readme)
*  **MQTT Explorer** --> You can use the **MQTT Explorer** app to quickly view topics on the MQTT server. [mqtt-explorer.com](https://mqtt-explorer.com/)
*  **IoT MQTT Panel** --> You can ues the **IoT MQTT panel** app (android) to Pup/Sub topic on the MQTT server.  [play.google...](https://play.google.com/store/apps/details?id=snr.lab.iotmqttpanel.prod&hl=en&gl=US)
