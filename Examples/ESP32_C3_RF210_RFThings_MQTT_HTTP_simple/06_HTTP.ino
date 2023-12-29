
#include <HTTPClient.h>
void httpGET(String url) {
  HTTPClient http;
  http.setConnectTimeout(10000);

  // Your Domain name with URL path or IP address with path
  http.begin(url.c_str());

  // Send HTTP GET request
  int httpResponseCode = http.GET();
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    // String payload = http.getString();
    // if (payload.length()) Serial.println(payload);
  } else {
    Serial.print("HTTP Error code: ");
    Serial.println(httpResponseCode);
    // String payload = http.getString();
    // if (payload.length()) Serial.println(payload);
  }
  // Free resources
  http.end();
}