//Guru Meditation Error: Core  0 panic'ed (Load access fault). Exception was unhandled.
/*
Instruction Access Fault, Load Access Fault, Store Access Fault
  This CPU exception happens when application attempts to execute, read from or write to an invalid memory location.
  The address which was written/read is found in MTVAL register in the register dump.   If this address is zero, it
  usually means that application attempted to dereference a NULL pointer. If this address is close to zero, it usually
  means that application attempted to access member of a structure, but the pointer to the structure was NULL. If this
  address is something else (garbage value, not in 0x3fxxxxxx - 0x6xxxxxxx range), it likely means that the pointer used
  to access the data was either not initialized or was corrupted.
*/
#define CRE_NAME F("Pham Duy Anh - 0908984010")
#define PRJ_NAME F("ESP32C3 RFThings RF210 - Get data and send to Titops MQTT")
#define UPDATED F("2023/11/15")
// define param for Serial Communicate
#define BAUDRATE 115200
#define TIMEOUT 20  //ms

#define DEBUG
#ifdef DEBUG
#define dbPrint(x) Serial.print(x)
#define dbPrintln(x) Serial.println(x)
#else
#define dbPrint(x)
#define dbPrintln(x)
#endif

// WIFI MANAGER ///////////////////////////////////////////////////////////////////////////////////
#define WIFI_MANAGER  // uncomment to use WifiManager library
/*
ESP32-C3FH4 with In-Package 4MB Flash
• 32-bit RISC-V single-core processor, up to 160 MHz

USB-CDC on boot -> Enable to serial monitor
USB-CDC on boot -> Disable to default

Flash download tool
  * https://www.espressif.com/en/support/download/other-tools
  * https://www.espressif.com/sites/default/files/tools/flash_download_tool_3.9.5.zip
  * X.bootloader.bin    0x01000
  * X.partitions.bin    0x08000
  * X.bin               0x10000

RF210
To dwonload:
1. press and hold        B_ESP
2. press                 EN_ESP
3. after download, press EN_ESP
////////////////////////////////////////////////////////////////////////////////////////////////////
RF210C  - RFThings
CHIP_EN -                                   EN_ESP button
GPIO00  - V_BCKP_ON_OFF
GPIO01  -           -> GPS ENABLE
GPIO02  -           ->          RAK BOOT    B_RAK button
GPIO03  - RX1       -> GPS TX
GPIO04  - LED ESP
GPIO05  - SDA
GPIO06  - SCL
GPIO07  - TX1       -> GPS RX
GPIO09  -                                   B_ESP button
GPIO10  -           ->          RAK ENA c
GPIO18  - USB D-
GPIO19  - USB D+
GPIO20  - RX0       ->          RAK TX2
GPIO21  - TX0       ->          RAK RX2
                                RST         R_RAK button

D1 LED  - GPIO04                            LED BLUE
D1 LED  -              GPS                  LED RED
D2 LED  -                       RAK         LED RED
////////////////////////////////////////////////////////////////////////////////////////////////////
12:03:42.339 -> I2C device found at address 0x1E  !
12:03:42.339 -> I2C device found at address 0x70  !

*/
// ESP32 C3 SERIAL1 (second UART)
HardwareSerial rakSerial(1);
//#include <Wire.h>


#define PIN_ESP_BTN 9
#define PIN_ESP_TX1 21
#define PIN_ESP_RX1 20
#define PIN_ESP_SDA 5
#define PIN_ESP_SCL 6
#define PIN_ESP_LED 4
#define PIN_RAK_EN 10
#define PIN_GPS_EN 1

// CONFIG IO PIN //////////////////////////////////////////////////////////////////////////////////
#define PIN_LED_STATE 1
#define PIN_LED PIN_ESP_LED      // BUILTIN_LED
#define TRIGGER_PIN PIN_ESP_BTN  // BUILTIN BUTTON
// LED STATUS /////////////////////////////////////////////////////////////////////////////////////
#define _timeLED 5
#define _timePeriod 200
#include "src/LEDStatus.h"  // https://github.com/PhamDuyAnh/LEDStatus
LEDStatus ledstatus(PIN_LED, 1, 5, 2000, PIN_LED_STATE);

#include <time.h>

#define delayTime 2000

String getID() {
  char _ID[23];  // ESP32 -> 23, ESP8266 -> 19
#ifdef ESP8266
  snprintf(_ID, sizeof(_ID), "RFThings-%08X", ESP.getFlashChipId());  //getChipID IoTDevice-0C8819A    getFlashChipId IoTDevice-016405E<<<<
#else
  snprintf(_ID, sizeof(_ID), "RFThings-%llX", ESP.getEfuseMac());
#endif
  return String(_ID);
}


String clientId = getID();  // AP:  A0764E01991D  STA:  A0764E01991C
// SETUP //////////////////////////////////////////////////////////////////////////////////////////
void setup() {

  pinMode(PIN_ESP_LED, OUTPUT);  // LED
  pinMode(PIN_ESP_BTN, INPUT_PULLUP);

  onRAK3172();

  ledstatus.ledFlasher(2);
  initSerial();
  ledstatus.ledFlasher(2);
  initSPIFFS();
  ledstatus.ledFlasher(2);
  initWifi();
  ledstatus.ledFlasher(2);
  initRakSerial();

  ledstatus.ledFlasher(2);
  initMQTT();
  ledstatus.ledFlasher(2);
  initRAK3172();
  // while(1){
  //   if (rakSerial.available()){
  //     Serial.write(rakSerial.read());
  //   }
  // }
  ledstatus.ledFlasher(5);
  dbPrintln("Setup done!");
  dbPrintln("Client ID: " + clientId);
  //initI2C();
}

String mqtt_TopicPrefix = clientId + "/";
String cmd[11] = { "ATC+TEMP",
                   // 08:28:35.787 -> ATC+TEMP
                   // 08:28:35.787 ->
                   // 08:28:35.787 -> 32.09
                   // 08:28:35.787 -> OK
                   "ATC+HUM",
                   "ATC+AX",
                   "ATC+AY",
                   "ATC+AZ",
                   "ATC+GPSSAT",
                   "ATC+GPSLAT",
                   "ATC+GPSLON",
                   "ATC+GPSALT",
                   "ATC+GPSTIME",
                   "ATC+BAT" };

String topic[11] = { "tem",
                     "hum",
                     "ax",
                     "ay",
                     "az",
                     "sat",
                     "lat",  // Latitude
                     "lon",  // Longitude
                     "alt",  // Altitude
                     "time",
                     "bat" };
String val[11];

// define will_topic end will message
#define will_topic "will"
#define will_Mess "0"  // will message is "0" -> offline, "1" is online
#define will_QoS 0
#define will_Retain true

unsigned long t;
unsigned int i;
bool mqtt_connected;
String pieces[3];
// LOOP ///////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  // Start LED Status - can use another CPU to run multi tasks ------

  //ledstatus.run();
  // End LED Status -------------------------------------------------

  //I2CScanner();
  if (millis() - t > 5000) {
    //if (0) {
    t = millis();

    if (isWifiConnected()) {
      if (!mqtt_connected) ledstatus.ledFlasher(1);
      else ledstatus.ledFlasher(2);
    } else ledstatus.ledFlasher(5);

    //if (isWifiConnected() && mqtt_connected) {
    // rakSendCMD("AT");                     // Send AT to test
    // if (rakWaitOk()) dbPrintln("<--OK");  //
    // rakClearSerial();

    for (byte i = 0; i < 11; i++) {
      //delay(100);
      rakSendCMD(cmd[i]);
      waitRAKResponse();
      //delay(300);
      rakSerial.setTimeout(100);
      String str = rakSerial.readStringUntil('\0');
      str.trim();
      str.replace("\r\n", "=");
      byte _l = str.length();
      byte _i = str.indexOf("OK");
      //dbPrintln("---: " + str);
      if (_i >= 0) {
        str = str.substring(0, _i - 1);

        _i = str.lastIndexOf("=");
        val[i] = str.substring(_i + 1);

        //dbPrintln("is OK......");
        //val[i] = getNumFromString(str);

        if (cmd[i] == "ATC+GPSTIME") {              // re calculate to Local time (GMT+7)
          time_t rawtime = val[i].toInt() + 25200;  // + 25200 -> GMT +7
          struct tm ts;
          char buf[80];

          // Format time, "ddd yyyy-mm-dd hh:mm:ss zzz"
          ts = *localtime(&rawtime);
          // ts = *gmtime(&rawtime);
          strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S", &ts);  // "%a %Y-%m-%d %H:%M:%S %Z" --> %Z TimeZone
          // strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
          val[i] = String(buf) + " VN";
          //dbPrintln(String(buf));
        }

        dbPrintln("value: " + val[i]);

        publishMessage(mqtt_TopicPrefix + topic[i], val[i]);
      }
      rakClearSerial();

      // splitStringToCMD(str, sizeof(pieces) / sizeof(String), "=");
      // if ((pieces[0] == cmd[i]) && (pieces[2] == "OK")) {
      // }


      // dbPrint("ESP32C3 send: ");
      // dbPrintln(cmd[i]);
      // dbPrint("RAK3172 send: ");
      // dbPrintln(val[i]);
    }
    //rakSerialClear();
    // rakSerial.println(cmd[i]);
    // serial1Wait();
    // String payload = rakSerialReadToString();

    // SKIP_ALL: all characters other than a minus sign, decimal point, or digits are ignored when scanning the stream for a floating point number. This is the default mode.
    // SKIP_NONE: Nothing is skipped, and the stream is not touched unless the first waiting character is valid.
    // SKIP_WHITESPACE: Only tabs, spaces, line feeds, and carriage returns are skipped.
    //float value = rakSerial.parseFloat();

    // unsigned int l = topic[i].length() + 1;
    // char _topic[l];
    // String _c = topicPrefix + topic[i];
    // (topicPrefix + topic[i]).toCharArray(_topic, l);

    // dbPrint("Topic: ");
    // dbPrintln(_topic);
    // dbPrint("Length: ");
    // dbPrintln(topic[i].length());
    // publish message to MQTT server
    //publishMessage(_topic, String(value));
    //publishMessage(_topic, payload);

    // for (byte i = 0; i < 11; i++) {
    //   publishMessage(mqtt_TopicPrefix + topic[i], val[i]);
    //   delay(10);
    // }
    //}
    publishMessage(mqtt_TopicPrefix + "wifi", String(wifiStrength()));  // WiFi signal strength
  }                                                                     // else rakClearSerial();

  SerialBridge();
  loopMQTT();
  // Start on Demain function, check button short press or long press
  // can use another CPU to run multi tasks
  onDemain();
  // End on Demain function------------------------------------------
}

String rakSerialReadToString() {
  String s;
  if (rakSerial.available()) {  // If anything comes in Serial1 (pins 0 & 1)
    while (rakSerial.available()) {
      char c = rakSerial.read();
      if (45 <= c && c <= 57) {  // only char: -./0123456789 -- https://docs.arduino.cc/built-in-examples/strings/CharacterAnalysis
        s += c;
      }
      delay(5);
    }
  }
  return s;
}
/*
String str;
void hwSerialRead() {
  str = "";
  while (rakSerial.available()) {
    char c = rakSerial.read();  // read it and send it out Serial (USB)
    str += c;
  }
  str.trim();  // remove any \r \n whitespace at the end of the String
  dbPrint("RAK3172 send: ");
  dbPrintln(str);
}
*/
/*
void initI2C() {
  Wire.begin(PIN_ESP_SDA, PIN_ESP_SCL);
  dbPrintln("\nI2C Scanner");
  delay(999);
}
void I2CScanner() {
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++) {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");

      nDevices++;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

  delay(5000);  // wait 5 seconds for next scan
}
*/






// void serial1Wait() {
//   while (!rakSerial.available()) {
//     delay(100);
//     dbPrint(".");
//   }
//   dbPrintln("");
//   delay(100);
// }
// void serial1Read() {
//   if (rakSerial.available()) {
//     String s;
//     while (rakSerial.available()) {
//       char c = rakSerial.read();
//       if (isprint(c) || c == 0x0A || c == 0x0D) s += c;  // https://docs.arduino.cc/built-in-examples/strings/CharacterAnalysis
//       //delay(2);
//     }
//     dbPrint("<--" + s);  // read it and send it out Serial (USB)
//   }
// }
void SerialBridge() {
  if (Serial.available()) {  // If anything comes in Serial (USB),
    char c = Serial.read();
    rakSerial.write(c);  // read it and send it out mySerial1 (pins 0 & 1)
    Serial.write(c);
  }

  if (rakSerial.available()) {       // If anything comes in Serial1 (pins 0 & 1)
    Serial.write(rakSerial.read());  // read it and send it out Serial (USB)
  }
}
// FUCNTION ///////////////////////////////////////////////////////////////////////////////////////
static bool ondemain;
void onDemain() {
  if (digitalRead(TRIGGER_PIN) == LOW) {
    delay(999);
    if (digitalRead(TRIGGER_PIN) == LOW) {
      digitalWrite(PIN_LED, PIN_LED_STATE);
      dbPrintln("Button Pressed");

      delay(1000);
      if (digitalRead(TRIGGER_PIN) == LOW) {
        dbPrint("Button long press check");
        for (int i = 0; i < 6; i++) {
          dbPrint(".");
          ledstatus.ledFlasher();
          //delay(250);
        }
        // still holding button for 3000 ms, reset settings, code not ideaa for production
        if (digitalRead(TRIGGER_PIN) == LOW) {
          digitalWrite(PIN_LED, PIN_LED_STATE);
          dbPrintln("\nButton is long pressed (> 3s)");
          dbPrintln("Call function Long press\n");
          //delay(2000);
          // call function 2
          onDemainLongPress();
          // Restart ESP
          // ESP.restart();
        }
        dbPrintln("");
      }
      ondemain = true;
      dbPrintln("\nButton is short pressed (> 0.5s and < 3s)");
      dbPrintln("Call function Short press\n");
      onDemainShortPress();
      // call function 1
      ondemain = false;
      digitalWrite(PIN_LED, !PIN_LED_STATE);
    }
  }
}
// initial Serial  ////////////////////////////////////////////////////////////////////////20231114
void initSerial() {
  Serial.begin(BAUDRATE);

  unsigned int _t;
  while (!Serial && (_t < 200)) {  // timeout to wait Serial is 2s
    delay(100);
    _t++;
  }

  Serial.setTimeout(TIMEOUT);

  dbPrintln("\n\nCreated by " + String(CRE_NAME));
  dbPrintln("  Project " + String(PRJ_NAME) + ", edited on " + String(UPDATED));
  char dateStr[11];
  dateReFormat(__DATE__, dateStr);
  dbPrintln("  Compiled on: " + String(__TIME__) + " - " + String(dateStr) + "\n");

  dbPrint(String(millis()) + "ms - ");
  dbPrintln("Initial Serial!");
}
void dateReFormat(char const *date, char *buff) {
  int month, day, year;
  static const char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
  sscanf(date, "%s %d %d", buff, &day, &year);

  month = (strstr(month_names, buff) - month_names) / 3 + 1;
  //sprintf(buff, "%d%02d%02d", year, month, day);
  sprintf(buff, "%d/%02d/%02d", year, month, day);
}
///////////////////////////////////////////////////////////////////////////////////////////20231114