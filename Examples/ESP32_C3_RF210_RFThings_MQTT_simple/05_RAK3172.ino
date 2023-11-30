// ESP32 C3 SERIAL1 (second UART)
HardwareSerial rakSerial(1);

#define waitATCResponseTimeout 1000  // 50s
#define timeoutRakWait 10000         // 10s
// initial Serial1  ///////////////////////////////////////////////////////////////////////////////
void initRakSerial() {
  pinMode(PIN_ESP_TX1, OUTPUT);
  pinMode(PIN_ESP_RX1, INPUT);
  rakSerial.begin(115200, SERIAL_8N1, PIN_ESP_RX1, PIN_ESP_TX1);
  unsigned int _t;
  while (!rakSerial && (_t < 200)) {  // timeout to wait Serial is 2s
    delay(10);
    _t++;
  }
}

void onRAK3172() {
  dbPrintln("\nPower on RAK3172, GNSS, sensor...");
  // RAK config
  pinMode(PIN_RAK_EN, OUTPUT);     // RAK enable
  digitalWrite(PIN_RAK_EN, LOW);   // Switch Off RAK3172
  delay(500);                      //
  digitalWrite(PIN_RAK_EN, HIGH);  // Switch On RAK3172
  delay(500);                      // wait RAK3172 boot

  // pinMode(PIN_GPS_EN, OUTPUT);     // GPS enable
  // digitalWrite(PIN_GPS_EN, HIGH);  // Switch on LC76F GPS
}

void initRAK3172() {
  delay(2000);
  dbPrintln("\nSetup RAK3172, GNSS, sensor...");
  rakClearSerial();  // Clear all Serial1 buffer

  // dbPrintln("--> ATR - Reset RAK3172");
  // rakSendCMD("ATR");
  // delay(4000);

  // https://docs.rakwireless.com/Product-Categories/WisDuo/RAK3172-Module/Deprecated-AT-Command/
  // https://docplayer.net/224365748-Rak3172-module-at-command-manual.html
  // Send ATC commnand to config RAK3172
  // ATC+SHT
  // ATC+KX023
  // ATC+GPSON=1
  // ATC+GPS
  // ATC+GPSCONST
  //delay(1000);
  //rakClearSerial();            // Clear all Serial1 buffer

  rakSendCMD("AT");                     // Send AT to test
  ledFlashOne();                        //
  if (rakWaitOk()) dbPrintln("<--OK");  //
  rakSendCMD("ATC+SHT");                //Activate the SHT sensor
  ledFlashOne();                        //
  if (rakWaitOk()) dbPrintln("<--OK");  //
  rakSendCMD("ATC+KX023");              //Activate the KX023 sensor
  ledFlashOne();                        //
  if (rakWaitOk()) dbPrintln("<--OK");  //
  rakSendCMD("ATC+GPSON=1");            //Activate the GNSS LDO
  ledFlashOne();                        //
  // if (rakWaitOk()) dbPrintln("<--OK");  //
  // rakSendCMD("ATC+GPSCONST");           // Activate Galileo, Beidu, GPS and Glonass constellations
  ledFlashOne();                        //
  if (rakWaitOk()) dbPrintln("<--OK");  //
  ledFlashOne();
  delay(2000);
  rakClearSerial();
  dbPrintln("");
}

// void wait() {
//   long a = millis();
//   while (1) {
//     if (rakSerial.available()) {
//       Serial.write(rakSerial.read());
//     }
//     if (millis() - a > 10000) break;
//   }
// }

// LED flashe (light) one time at 5ms  ////////////////////////////////////////////////////20231114
void ledFlashOne() {
  digitalWrite(PIN_ESP_LED, 1);
  delay(5);
  digitalWrite(PIN_ESP_LED, 0);
}

String rakReadStringUntil() {
  rakSerial.setTimeout(100);
  return rakSerial.readStringUntil('\0');
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

bool rakWaitOk() {
  unsigned int _t = 1;
  while (_t < timeoutRakWait) {  // timeout to wait Serial is 2s
    delay(1);
    _t++;

    if (rakSerial.available()) {
      char c = rakSerial.read();
      if (c == 'O')
        if (rakSerial.read() == 'K') {
          //rakClearSerial();
          return true;
        }
    }
  }
  return false;
}

void rakSendCMD(String _cmd) {
  dbPrintln("-->" + _cmd);
  //rakClearSerial();
  rakSerial.println(_cmd);
  //rakSerial.flush();
}

void rakClearSerial() {
  if (rakSerial.available())
    while (rakSerial.available()) char c = rakSerial.read();
}

void waitRAKResponse() {
  dbPrint("Wait...");
  unsigned long o = millis();
  while (!rakSerial.available()) {
    delay(200);
    if (millis() - o > waitATCResponseTimeout) {
      dbPrint("error...\n");
      break;
    }
  }
  delay(50);
  dbPrintln(millis() - o);
  //delay(200);
}