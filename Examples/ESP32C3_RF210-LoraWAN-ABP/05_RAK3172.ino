#define timeoutRakWait 10000  // 10s
// ESP32 C3 SERIAL1 (second UART)
//HardwareSerial rakSerial(1);

#define PIN_ESP_TX1 21
#define PIN_ESP_RX1 20
#define PIN_RAK_ON 10

void rakPowerOn() {
  pinMode(10, OUTPUT);  //Rak enable
  digitalWrite(10, HIGH);
  delay(500);
}

// initial Serial1 - connect tp RAK ///////////////////////////////////////////////////////////////
void initRakSerial() {
  pinMode(PIN_ESP_TX1, OUTPUT);
  pinMode(PIN_ESP_RX1, INPUT);
  rakSerial.begin(115200, SERIAL_8N1, PIN_ESP_RX1, PIN_ESP_TX1);
  unsigned int t = millis();
  while (!rakSerial && (millis() - t < 2000))
    ;
}
// Clear all Serial1 (connect to RAK3172) buffer //////////////////////////////////////////////////
void rakClearSerial() {
  if (rakSerial.available())
    while (rakSerial.available()) char c = rakSerial.read();
}

bool rakWaitDONE() {
  unsigned long _t;

  while (_t < timeoutRakWait) {  // timeout to wait Serial is 2s
    delay(1);
    _t++;

    if (rakSerial.available()) {
      char c = rakSerial.read();
      if (c == 'O')
        if (rakSerial.read() == 'N')
          if (rakSerial.read() == 'E') {
            dbPrintln("TX_DONE");
            rakClearSerial();
            return true;
          }
    }
  }
  return false;
}

bool rakWaitLoraWAN() {
  unsigned long _t;

  while (_t < timeoutRakWait) {  // timeout to wait Serial is 2s
    delay(1);
    _t++;

    if (rakSerial.available()) {
      char c = rakSerial.read();
      if (c == 'W')
        if (rakSerial.read() == 'A')
          if (rakSerial.read() == 'N') {
            dbPrintln("OK");
            rakClearSerial();
            return true;
          }
    }
  }
  return false;
}

bool rakWaitOk() {
  unsigned long _t = 1;
  while (_t < timeoutRakWait) {  // timeout to wait Serial is 2s
    delay(1);
    _t++;

    if (rakSerial.available()) {
      char c = rakSerial.read();
      if (c == 'O')
        if (rakSerial.read() == 'K') {
          dbPrintln("OK");
          rakClearSerial();
          return true;
        }
    }
  }
  return false;
}

bool rakWaitResponse() {
  unsigned long _t = 1;
  while (_t < timeoutRakWait) {  // timeout to wait Serial is 2s
    delay(1);
    _t++;

    if (rakSerial.available())
      return true;
  }
  return false;
}

void rakSendCMD(String _cmd) {
  dbPrintln(_cmd);
  rakSerial.println(_cmd);
}
// Complies with LoRaWAN 1.0.3 specifications for Class A, Class B, and Class C
// node 11 - DEVUEI AC1F09FFFE0E44BA
// node 14 -
String devAddr = "260D20" + String(NODE);
String nwkSkey = "A46A761599E269F738F33F11444B460D";
String appSkey = "A46A761599E269F738F33F11444B460D";

void initRAK3172_LoraWAN() {
  rakSendCMD("AT+NWM=1");  //0 = P2P_LORA, 1 = LoRaWAN, 2 = P2P_FSK
  rakWaitLoraWAN();
  rakSendCMD("AT+NJM=0");  // 0 = ABP, 1 = OTAA
  rakWaitOk();
  rakSendCMD("AT+CLASS=A");
  rakWaitOk();
  rakSendCMD("AT+BAND=" + String(band));  // 0 = EU433, 1 = CN470, 2 = RU864, 3 = IN865, 4 = EU868, 5 = US915, 6 = AU915,
                                          // 7 = KR920, 8 = AS923-1, 9 = AS923-2, 10 = AS923-3, 11 = AS923-4, 12 = LA915
  rakWaitOk();
  rakSendCMD("AT+DEVADDR=" + devAddr);
  rakWaitOk();
  rakSendCMD("AT+NWKSKEY=" + nwkSkey);
  rakWaitOk();
  rakSendCMD("AT+APPSKEY=" + appSkey);
  rakWaitOk();
  //rakSendCMD("AT+JOIN");  //ATC+SHT
  //rakWaitOk();
  rakSendCMD("ATC+SHT");
  rakWaitOk();
}
void rakSetAddr(String addr) {
  rakSendCMD("AT+DEVADDR=" + addr);
  rakWaitOk();
  rakSendCMD("AT+NWKSKEY=" + nwkSkey);
  rakWaitOk();
  rakSendCMD("AT+APPSKEY=" + appSkey);
  rakWaitOk();
  rakSendCMD("AT+JOIN");  //ATC+SHT
  rakWaitOk();
}
void rakReadSHT() {
  for (int i = 0; i < sizeof(cmd) / sizeof(String); i++) {
    rakSendCMD(cmd[i]);
    rakWaitResponse();
    //delay(300);
    rakSerial.setTimeout(100);
    String str = rakSerial.readStringUntil('\0');
    str.trim();
    str.replace("\r\n", "=");
    byte _l = str.length();
    byte _i = str.indexOf("OK");

    if (_i >= 0) {
      str = str.substring(0, _i - 1);

      _i = str.lastIndexOf("=");
      str = str.substring(_i + 1);
      if (cmd[i] == "ATC+TEMP") tem = str;
      if (cmd[i] == "ATC+HUM") hum = str;
    }
    rakClearSerial();
  }
}