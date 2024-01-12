#define CRE_NAME F("Pham Duy Anh - 0908984010")
#define PRJ_NAME F("ESP32C3 RFThings RF210")
#define UPDATED F("2023/12/03")
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

#define OTA
#define NODE 11  // <50
#define t_led 1000
#define INTERVAL 120000  //s ~ 10min
// 0 = EU433, 1 = CN470, 2 = RU864, 3 = IN865, 4 = EU868, 5 = US915, 6 = AU915,
// 7 = KR920, 8 = AS923-1, 9 = AS923-2, 10 = AS923-3, 11 = AS923-4, 12 = LA915
#define band 9

String _band[13] = { "EU433", "CN470", "RU864", "IN865", "EU868", "US915", "AU915", "KR920", "AS923-1", "AS923-2", "AS923-3", "AS923-4", "LA915" };
bool isOTA = false;

#define LED 4

// ESP32 C3 SERIAL1 (second UART)
HardwareSerial rakSerial(1);

void setup() {
  pinMode(LED, OUTPUT);  // LED

  initSerial();

  setupOTA();

  rakPowerOn();
  initRakSerial();
  initRAK3172_LoraWAN();

  dbPrintln("RAK3172 LoraWAN node " + String(NODE) + " Setup OK");
  dbPrintln("Lora band: " + _band[band]);

  readAndSend();
}

void led() {
  digitalWrite(LED, 1);
  delay(5);
  digitalWrite(LED, 0);
}
unsigned long t, r;



String cmd[2] = { "ATC+TEMP", "ATC+HUM" };
String tem = "0000";
String hum = "00";

void loop() {
  if (millis() - t > t_led) {
    t += t_led;
    led();
  }

  if (!isOTA) {
    if (millis() - r > INTERVAL) {
      r += INTERVAL;

      digitalWrite(LED, 1);
      readAndSend();
      digitalWrite(LED, 0);
    }
  }

  loopOTA();

  serialBridger();
}

void readAndSend() {

#if NODE == 11
  static int i;
  rakSetAddr("260D20" + String(NODE + i));
  i++;
  if (i >= 3) i = 0;
#endif

  rakReadSHT();
  dbPrintln("Temp: " + tem + "; Hum: " + hum);

  tem.replace(".", "");
  String payload = tem + hum;

  // AT+SEND=<port>:<payload>
  // <port>:    1 decimal integer and the range of values is 1~233.
  // <payload>: 2~500-digit length, must be an even number of digits and character 0-9, a-f, A-F only, representing 1~256 hexadecimal numbers.
  rakSendCMD("AT+SEND=3:" + payload);
  rakWaitDONE();
}

void serialBridger() {
  if (Serial.available()) {          // If anything comes in Serial (USB),
    rakSerial.write(Serial.read());  // read it and send it out mySerial1 (pins 0 & 1)
  }
  if (rakSerial.available()) {       // If anything comes in Serial1 (pins 0 & 1)
    Serial.write(rakSerial.read());  // read it and send it out Serial (USB)
  }
}

// https://www.rapidtables.com/convert/number/ascii-to-hex.html
// Function to convert ASCII Text (String) to Hex Code Converter
// @ str  -> (String) ASCII needs convert to Hex code (String)
// Ex:
// convertHexToText("Pham Duy Anh - 0908984010")
// return String -> 5068616D2044757920416E68202D2030393038393834303130
String convertTextToHex(String str) {
  String s;
  for (int i = 0; i < str.length(); i++) {
    char c = str.charAt(i);
    s += String(c, HEX);
  }
  return s;
}
