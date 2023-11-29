# RFThings_how_to_use

### Installing the Arduino IDE & ESP32 Board

1.  Download and install the Arduino IDE (at least version v1.6.8)
2.  Start the Arduino IDE
3.  Go into File/Preferences Add --> **Additional Board Manager URLs** --> **Add URL**
   `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`

![Additional Board Manager URLs](https://github.com/PhamDuyAnh/RFThings_how_to_use/blob/main/REF/ESP32-URL-Arduino-IDE.webp)

4.  Open the Boards Manager. Go to Tools > Board > Boards Manager… --> Search for ESP32 and press install button for the **ESP32 by Espressif Systems** & **Install**
image install

### First test: RF210_RAK3172_bridge
1.  Connect your RF210 board (microUSB port) to your PC USB port (normally Windows will automatically install the driver)
2.  Download sample sketch from Github --> [RF210_RAK3172_bridge](https://github.com/FabienFerrero/SUCA/tree/main/Examples/RF210_RAK3172_bridge)
3.  Unzip and open the file RF210_RAK3172_bridge.ino (note that the file name must be the same as the folder name)
4.  Open the Boards Manager from the **Tools** --> **Board** --> **esp32** --> **ESP32C3 Dev Module** and Select.
5.  Open the Boards Manager from the **Tools** --> **Port** and Select Port (check step 1)
6.  Open the Boards Manager from the **Tools** --> **USB-CDC on boot** --> **Enable**
7.  Hold the **B_ESP1** (boot) button and press **ESP_EN1** (reset) button and release the B_ESP1 (boot) button to enter bootmode.
8.  Upload Sketch (Ctrl+U)
9.  After upload sketch, press **ESP_EN1** to reboot
10.  Use Serial Monitor to monitor Serial and send CMD
11.  Enter **AT?** and press **Enter** and see
