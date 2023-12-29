#define OTA

#include <WiFi.h>
struct wpas {
  char ssid[32];  // Maximum Buffer Size (and allocated memory)
  char pass[64];
};
wpas wpa[] = {
  // { "ssid", "password"},
  { "PDA", "0908984010" },
  { "Pham", "Da09Ha23Du23Ho09" },
  { "Titops", "12345678" },
  { "TITOPS", "12345678" },
  { "a3khia02", "12345678" }
};

void initMultiFifi() {
  //   if (WiFi.status() != WL_CONNECTED) {
  // #ifdef NODE
  //     // CKD add static IP
  //     if (!WiFi.config(IPAddress(192, 168, 1, 200 + NODE), IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0), IPAddress(8, 8, 8, 8), IPAddress(8, 8, 4, 4))) {
  //       Serial.println("Static IP Failed to configure");
  //     }
  //     // CKD add static IP
  // #endif
  //     Serial.print("Connect to saved Wifi..");
  //     WiFi.setAutoConnect(true);
  //     WiFi.mode(WIFI_STA);
  //     WiFi.begin();
  //     unsigned long t_out = millis();
  //     while (WiFi.status() != WL_CONNECTED && (millis() - t_out < 20000)) {  // https://github.com/espressif/arduino-esp32/blob/master/libraries/WiFi/src/WiFiType.h
  //       delay(200);
  //       Serial.print(".");
  //     }
  //     Serial.println();
  //   }

  if (WiFi.status() != WL_CONNECTED) {

    WiFi.disconnect();

    bool known = false;
    int _bestRSSI = INT_MIN;
    String ssid, pass;
    // char _bestSSID[32];
    // char _bestPASS[64];
    // int _wifiIndex;

    Serial.println("\nWifi scan...");
    // Wifi Scan
    // scanNetworks(bool async = false, bool show_hidden = false, bool passive = false, uint32_t max_ms_per_chan = 300, uint8_t channel = 0, const char * ssid=nullptr, const uint8_t * bssid=nullptr);
    //byte n = WiFi.scanNetworks();
    byte n = WiFi.scanNetworks(/*async=*/false, /*hidden=*/true);

    if (n == 0) {
      Serial.println("No networks found");
    } else {

      Serial.printf("%d Wifi networks found\n", n);

      for (int i = 0; i < n; i++) {
        String _ssid = WiFi.SSID(i);
        int32_t _rssi = WiFi.RSSI(i);

        for (byte i = 0; i < (sizeof(wpa) / sizeof(wpa[0])); i++) {
          if (String(wpa[i].ssid) == _ssid) {
            known = true;
            if (_rssi > _bestRSSI) {
              _bestRSSI = _rssi;
              //_wifiIndex = i;
              ssid = String(wpa[i].ssid);
              pass = String(wpa[i].pass);
            }
            break;
          }
        }
      }
    }
    WiFi.scanDelete();
    WiFi.disconnect();
    delay(200);
    if (known) {
#ifdef NODE
      // CKD add static IP
      if (!WiFi.config(IPAddress(192, 168, 1, 200 + NODE), IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0), IPAddress(8, 8, 8, 8), IPAddress(8, 8, 4, 4))) {
        Serial.println("Static IP Failed to configure");
      }
      // CKD add static IP
#endif
      Serial.print("Connect to Wifi: " + ssid + "...");
      WiFi.setAutoConnect(true);
      WiFi.mode(WIFI_STA);
      WiFi.begin(ssid.c_str(), pass.c_str());

      unsigned long t_out = millis();
      while (WiFi.status() != WL_CONNECTED && (millis() - t_out < 20000)) {  // https://github.com/espressif/arduino-esp32/blob/master/libraries/WiFi/src/WiFiType.h
        delay(200);
        Serial.print(".");
      }
      Serial.println();
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.print("WiFi connected --> IP address: ");
      Serial.println(WiFi.localIP());
    } else {
      Serial.println("Can't connect to Wifi:");
      switch (WiFi.status()) {
        case 255:
          Serial.println("Wifi error WL_NO_SHIELD");
          break;
        case 254:
          Serial.println("Wifi error WL_STOPPED");
          break;
        case 0:
          Serial.println("Wifi error WL_IDLE_STATUS");
          break;
        case 1:
          Serial.println("Wifi error WL_NO_SSID_AVAIL");
          break;
        case 2:
          Serial.println("Wifi error WL_SCAN_COMPLETED");
          break;
        case 3:
          Serial.println("Wifi error WL_CONNECTED");
          break;
        case 4:
          Serial.println("Wifi error WL_CONNECT_FAILED");
          break;
        case 5:
          Serial.println("Wifi error WL_CONNECTION_LOST");
          break;
        case 6:
          Serial.println("Wifi error WL_DISCONNECTED");
          break;
      }
    }
  }
}

void multiWifiLoop() {
  if (WiFi.status() != WL_CONNECTED) {
    initMultiFifi();
  }
}

String getFiwiSSID(){
  return WiFi.SSID();
}

// OTA ////////////////////////////////////////////////////////////////////////////////////////////
#ifdef OTA
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

void setupOTA(String host) {
#ifdef OTA
  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);

  // Hostname defaults to esp3232-[MAC]
  ArduinoOTA.setHostname(host.c_str());

  // No authentication by default
  ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA
    .onStart([]() {
      isOTA = true;
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else  // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("\nStart updating " + type);
    })
    .onEnd([]() {
      isOTA = false;
      Serial.println("\nProgress: 100%");
      Serial.println("End --> Reboot\n\n");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      static int _p;
      int p = 100 * progress / total;
      if (p != _p) {
        _p = p;
        if (p % 10 == 1) Serial.printf("Progress: %u%%\r", p);
      }
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR)
        Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR)
        Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR)
        Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR)
        Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR)
        Serial.println("End Failed");
    });

  ArduinoOTA.begin();

  Serial.print("OTA ready      --> IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Password: admin\n");
#endif
}

void loopOTA() {
#ifdef OTA
  ArduinoOTA.handle();
#endif
}
#endif
