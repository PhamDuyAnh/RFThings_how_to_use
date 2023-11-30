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
#define PRJ_NAME F("ESP32C3 RFThings RF210 - Get RAK3172 data and send to MQTT")
#define UPDATED F("2023/11/30")
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
  char _ID[23];
  snprintf(_ID, sizeof(_ID), "RFThings-%llX", ESP.getEfuseMac());
  return String(_ID);
}
String clientId = getID();

// SETUP //////////////////////////////////////////////////////////////////////////////////////////
void setup() {

  pinMode(PIN_ESP_LED, OUTPUT);  // LED
  pinMode(PIN_ESP_BTN, INPUT_PULLUP);

  onRAK3172();

  ledstatus.ledFlasher(2);
  initSerial();
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


unsigned long t;
unsigned int i;
bool mqtt_connected;
String pieces[3];

// LOOP ///////////////////////////////////////////////////////////////////////////////////////////
void loop() {
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
      String str = rakReadStringUntil();
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
    }
    publishMessage(mqtt_TopicPrefix + "wifi", String(wifiStrength()));  // WiFi signal strength
  }                                                                     // else rakClearSerial();

  loopMQTT();
}



// void SerialBridge() {
//   if (Serial.available()) {  // If anything comes in Serial (USB),
//     char c = Serial.read();
//     rakSerial.write(c);  // read it and send it out mySerial1 (pins 0 & 1)
//     Serial.write(c);
//   }

//   if (rakSerial.available()) {       // If anything comes in Serial1 (pins 0 & 1)
//     Serial.write(rakSerial.read());  // read it and send it out Serial (USB)
//   }
// }

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