#ifdef ESP8266
#include <ESP8266WiFi.h>  // ESP8266
#else
#include <WiFi.h>  // ESP32
#endif

const char* ssid = "YOUR AP SSID";
const char* pass = "YOUR AP PASSWORD";
// initWifi ///////////////////////////////////////////////////////////////////////////////////////
void initWifi() {
  dbPrintln("WIFI -> Wait for connection to Wifi");
#ifdef WIFI_MANAGER
  wmConfig();
#else
  WiFi.mode(WIFI_STA);  // WIFI_STA, WIFI_AP, WIFI_AP_STA
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    ledstatus.ledFlasher();
    //delay(500);
    dbPrint(".");
  }
  dbPrintln("");
#endif
  dbPrint("\n");
  dbPrintln("WIFI -> Connected successfully");
  dbPrint("IP address: ");
  dbPrintln(WiFi.localIP());
}
// WIFI FUNCTION //////////////////////////////////////////////////////////////////////////////////
bool isWifiConnected() {
  return (WiFi.status() == WL_CONNECTED);
}
long wifiStrength() {
  return WiFi.RSSI();
}