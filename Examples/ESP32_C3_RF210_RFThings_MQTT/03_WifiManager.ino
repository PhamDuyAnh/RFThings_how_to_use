#define wm_AP_NAME "RFThings"
#define PortalTimeout 120

#include <WiFiManager.h>  //https://github.com/tzapu/WiFiManager
#include <ArduinoJson.h>  // https://arduinojson.org/ - https://github.com/bblanchon/ArduinoJson

unsigned int mqtt_tcpssl = 0;         // 1 use TCP SSL, 0 not use TCP SSL
unsigned int mqtt_port = 1883;        // MQTT port
char mqtt_server[60] = "titops.org";  // MQTT server
char mqtt_username[32] = "titops";    // MQTT user name
char mqtt_password[32] = "titops";    // MQTT password

//default custom static IP
// char static_ip[16] = "192.68.1.201";
// char static_gw[16] = "192.168.1.1";
// char static_sn[16] = "255.255.255.0";

//flag for saving data
bool shouldSaveConfig = false;

// Open file config.json and parse data  //////////////////////////////////////////////////////////
// https://arduinojson.org/v6/doc/
// (ARDUINOJSON_VERSION_MAJOR) && (ARDUINOJSON_VERSION_MAJOR >= 6)
void openConfigJson() {
  dbPrintln("mounting FS...");

  if (SPIFFS.begin()) {
    dbPrintln("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      dbPrintln("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        dbPrintln("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        //#if defined(ARDUINOJSON_VERSION_MAJOR) && (ARDUINOJSON_VERSION_MAJOR >= 6)
        DynamicJsonDocument json(1024);
        auto deserializeError = deserializeJson(json, buf.get());
        //serializeJson(json, Serial);  // Write the JSON document to the serial port
        if (!deserializeError) {
          // #else
          //         DynamicJsonBuffer jsonBuffer;
          //         JsonObject& json = jsonBuffer.parseObject(buf.get());
          //         json.printTo(Serial);
          //         if (json.success()) {
          // #endif
          dbPrintln("\nparsed json");

          //strcpy(my_server, json["my_server"]);
          if (json.containsKey("mqtt_server")) strcpy(mqtt_server, json["mqtt_server"]);
          if (json.containsKey("mqtt_user")) strcpy(mqtt_username, json["mqtt_user"]);
          if (json.containsKey("mqtt_pass")) strcpy(mqtt_password, json["mqtt_pass"]);
          if (json.containsKey("mqtt_port")) mqtt_port = json["mqtt_port"];
          if (json.containsKey("mqtt_tcpssl")) mqtt_tcpssl = json["mqtt_tcpssl"];
          if (json.containsKey("mqtt_prefix")) {
            char _prefix[60];
            strcpy(_prefix, json["mqtt_prefix"]);
            mqtt_TopicPrefix = String(_prefix);
          }

          // mqtt_port = String(json["mqtt_port"]).toInt();
          // mqtt_tcpssl = String(json["mqtt_tcpssl"]).toInt();


          // if (json["ip"]) {
          //   dbPrintln("setting custom ip from config");
          //   strcpy(static_ip, json["ip"]);
          //   strcpy(static_gw, json["gateway"]);
          //   strcpy(static_sn, json["subnet"]);
          //   dbPrintln(static_ip);
          // } else {
          //   dbPrintln("no custom ip in config");
          // }
        } else {
          dbPrintln("failed to load json config");
        }
      }

      //dbPrintln(static_ip);
      //dbPrintln(mqtt_server);

    } else {
      dbPrintln("File config.json not exists!!!");
    }
  } else {
    dbPrintln("failed to mount FS");
  }
  //end read
}
// Open file config.json and save data  ///////////////////////////////////////////////////////////
// https://arduinojson.org/v6/doc/
// (ARDUINOJSON_VERSION_MAJOR) && (ARDUINOJSON_VERSION_MAJOR >= 6)
void saveConfigJson() {
  //save the custom parameters to FS
  if (shouldSaveConfig) {
    dbPrintln("saving config");
    //#if defined(ARDUINOJSON_VERSION_MAJOR) && (ARDUINOJSON_VERSION_MAJOR >= 6)
    DynamicJsonDocument json(1024);
    // #else
    //     DynamicJsonBuffer jsonBuffer;
    //     JsonObject& json = jsonBuffer.createObject();
    // #endif
    json["mqtt_server"] = mqtt_server;
    json["mqtt_port"] = mqtt_port;
    json["mqtt_user"] = mqtt_username;
    json["mqtt_pass"] = mqtt_password;
    json["mqtt_tcpssl"] = mqtt_tcpssl;
    json["mqtt_prefix"] = mqtt_TopicPrefix;

    //json["api_token"] = api_token;

    //json["ip"] = WiFi.localIP().toString();
    //json["gateway"] = WiFi.gatewayIP().toString();
    //json["subnet"] = WiFi.subnetMask().toString();

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      dbPrintln("failed to open config file for writing");
    }

    //#if defined(ARDUINOJSON_VERSION_MAJOR) && (ARDUINOJSON_VERSION_MAJOR >= 6)
    //serializeJson(json, Serial);  // Write the JSON document to the serial port
    serializeJson(json, configFile);
    // #else
    //     json.printTo(Serial);
    //     json.printTo(configFile);
    // #endif
    configFile.close();
    //end save
  }

  dbPrintln("\nlocal ip");
  dbPrintln(WiFi.localIP());
  dbPrintln(WiFi.gatewayIP());
  dbPrintln(WiFi.subnetMask());
  dbPrintln("Save to config.json");
  dbPrintln("MQTT server: " + String(mqtt_server));
  dbPrintln("MQTT port: " + String(mqtt_port));
  dbPrintln("MQTT prefix: " + mqtt_TopicPrefix);
  dbPrintln("TCP SSL: " + String(mqtt_tcpssl));

  shouldSaveConfig = false;
  //serverName = String(my_server);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void wmConfig() {
  openConfigJson();
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wm;

  // WM_DEBUG_SILENT    = 0, // debug OFF but still compiled for runtime
  // WM_DEBUG_ERROR     = 1, // error only
  // WM_DEBUG_NOTIFY    = 2, // default stable,INFO
  // WM_DEBUG_VERBOSE   = 3, // move verbose info
  // WM_DEBUG_DEV       = 4, // development useful debugging info
  // WM_DEBUG_MAX       = 5  // MAX extra dev auditing, var dumps etc (MAX+1 will print timing,mem and frag info)

#ifdef DEBUG
  wm.setDebugOutput(true, WM_DEBUG_MAX);
#else
  wm.setDebugOutput(false);
#endif

  //if true (default) then start the config portal from autoConnect if connection failed
  // wm.setEnableConfigPortal(false);
  //set config save notify callback
  wm.setSaveConfigCallback(saveConfigCallback);
  //set custom ip for portal
  //wm.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

  //Set Static ip
  // IPAddress _ip, _gw, _sn;
  // _ip.fromString(static_ip);
  // _gw.fromString(static_gw);
  // _sn.fromString(static_sn);
  //wm.setSTAStaticIPConfig(_ip, _gw, _sn);
  // inject custom head element You can use this to any html bit to the head of the configuration portal. If you add a <style> element, bare in mind it overwrites the included css, not replaces.
  //wm.setCustomHeadElement("<style>html{filter: invert(100%); -webkit-filter: invert(100%);}</style>");
  wm.setCustomHeadElement("<script>function ssl(){var s=document.getElementById('tcpssl');s.checked?s.value='1':s.value='0';} function sp(){var p=document.getElementById('password');p.type==='password'?p.type='text':p.type='password';}</script>");

  // can customize certain elements of the default template with some builtin classes
  //wm.setClass("invert"); // dark theme
  //wm.setScanDispPerc(true); // display percentages instead of graphs for RSSI

  // inject a custom bit of html in the configuration/param form
  //WiFiManagerParameter custom_text("<p>This is just a text paragraph</p>");
  //wm.addParameter(&custom_text);

  // The extra parameters to be configured (can be either global or just in the setup)
  // After connecting, parameter.getValue() will get you the configured value
  // id/name placeholder/prompt default length
  //WiFiManagerParameter custom_my_server("server", "Server, ex: https://titops.com/co2", my_server, 40);
  char _port[6];
  String(mqtt_port).toCharArray(_port, 6);  // convert int to char array
  WiFiManagerParameter custom_mqtt_port("port", "MQTT port", _port, 6);

  String _val;
  String _custom;
  if (mqtt_tcpssl == 1) {
    _val = "1";
    _custom = "onclick='ssl()' type='checkbox' checked";
  } else {
    _val = "0";
    _custom = "onclick='ssl()' type='checkbox'";
  }

  WiFiManagerParameter custom_mqtt_tcpssl("tcpssl", " Use TCP SSL", _val.c_str(), 1, _custom.c_str(), WFM_LABEL_AFTER);
  WiFiManagerParameter custom_mqtt_server("server", "MQTT server", mqtt_server, 60);
  WiFiManagerParameter custom_mqtt_user("user", "MQTT user", mqtt_username, 60);
  String label = "Use topic prefix, ex: <strong>" + mqtt_TopicPrefix + "</strong>";
  WiFiManagerParameter custom_mqtt_prefix("prefix", label.c_str(), mqtt_TopicPrefix.c_str(), 60);
  WiFiManagerParameter custom_mqtt_pass("password", "MQTT password <span style='font-size:12px'>(double click to show password)</span>", mqtt_password, 60, "type='password' onDblClick='sp()'");
  //WiFiManagerParameter custom_html_hr("<hr>");

  //add all your parameters here
  //wm.addParameter(&custom_my_server);

  wm.addParameter(&custom_mqtt_user);
  wm.addParameter(&custom_mqtt_pass);
  //wm.addParameter(&custom_html_hr);
  wm.addParameter(&custom_mqtt_server);
  wm.addParameter(&custom_mqtt_prefix);
  wm.addParameter(&custom_mqtt_port);
  wm.addParameter(&custom_mqtt_tcpssl);

  //reset settings - for testing
  //wm.resetSettings();

  //set minimu quality of signal so it ignores AP's under that quality
  //defaults to 8%
  //wm.setMinimumSignalQuality(10);
  // can also remove or show duplicate networks (default is remove). Use this function to show (or hide) all networks.
  //wm.setRemoveDuplicateAPs(false);

  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  wm.setTimeout(PortalTimeout);

  // if you just want an unsecured access point
  // wifiManager.autoConnect("AP-NAME");
  // if you want to use and auto generated name from 'ESP' and the esp's Chip ID use
  // wifiManager.autoConnect();
  // wm.setParamsPage(true);

  if (!wm.autoConnect(wm_AP_NAME, "0908984010")) {
    dbPrintln("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(5000);
  }

  // if (!ondemain) {
  //   if (!wm.autoConnect(wm_AP_NAME, "0908984010")) {
  //     dbPrintln("failed to connect and hit timeout");
  //     delay(3000);
  //     //reset and try again, or maybe put it to deep sleep
  //     ESP.restart();
  //     delay(5000);
  //   }
  // } else {
  //   //wm.resetSettings();
  //   //if (!wm.autoConnect(wm_AP_NAME, "0908984010")) {
  //   // WiFi.disconnect();
  //   // delay(100);
  //   if (!wm.startConfigPortal(wm_AP_NAME, "0908984010")) {
  //     dbPrintln("failed to connect and hit timeout");
  //     delay(3000);
  //     //reset and try again, or maybe put it to deep sleep
  //     ESP.restart();
  //     delay(5000);
  //   }
  // }

  //if you get here you have connected to the WiFi
  dbPrintln("connected...yeey :)");

  //read updated parameters
  mqtt_port = String(custom_mqtt_port.getValue()).toInt();
  mqtt_tcpssl = String(custom_mqtt_tcpssl.getValue()).toInt();
  mqtt_TopicPrefix = String(custom_mqtt_prefix.getValue());
  strcpy(mqtt_server, custom_mqtt_server.getValue());
  strcpy(mqtt_username, custom_mqtt_user.getValue());
  strcpy(mqtt_password, custom_mqtt_pass.getValue());

  saveConfigJson();
}
//callback notifying us of the need to save config
void saveConfigCallback() {
  dbPrintln("Should save config");
  shouldSaveConfig = true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
/*
void SPIFFSformat() {
  if (SPIFFS.format()) {
    dbPrintln("\n\nSuccess formatting");
  } else {
    dbPrintln("\n\nError formatting");
  }
}
*/
///////////////////////////////////////////////////////////////////////////////////////////////////


void onDemainShortPress() {
  digitalWrite(PIN_LED, PIN_LED_STATE);
  ondemain = true;

  WiFi.disconnect();
  WiFiManager wm;
  //wm.setSaveConfigCallback(saveConfigCallback);
  //wmConfig();
  //wm.setParamsPage(true);
  //wm.setTimeout(PortalTimeout);
  wm.setConfigPortalTimeout(PortalTimeout);
  if (!wm.startConfigPortal(wm_AP_NAME, "0908984010")) {
    dbPrintln("failed to connect and hit timeout");
    dbPrintln("ESP will restart...\n");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(5000);
  }

  ondemain = false;
  dbPrintln("connected...yeey :)");
  digitalWrite(PIN_LED, !PIN_LED_STATE);
}
void onDemainLongPress() {
  digitalWrite(PIN_LED, PIN_LED_STATE);
  dbPrintln("Erasing Config, restarting");
  WiFiManager wm;
  removeConfigFile();
  wm.resetSettings();
  digitalWrite(PIN_LED, !PIN_LED_STATE);
  ESP.restart();
}
void removeConfigFile() {
  if (SPIFFS.exists("/config.json")) {
    if (SPIFFS.remove("/config.json")) dbPrintln("File config removed!");
  }
}
/*
// #define TRIGGER_PIN 0
// On Demain
void onDemain() {
  if (digitalRead(TRIGGER_PIN) == LOW) {
    delay(500);
    if (digitalRead(TRIGGER_PIN) == LOW) {
      digitalWrite(PIN_LED, 0);
      dbPrintln("Button Pressed");
      WiFiManager wm;
      delay(1000);
      if (digitalRead(TRIGGER_PIN) == LOW) {
        dbPrint("Button long press check");
        for (int i = 0; i < 10; i++) {
          dbPrint(".");
          digitalWrite(PIN_LED, !digitalRead(PIN_LED));
          delay(200);
        }
        // still holding button for 3000 ms, reset settings, code not ideaa for production
        if (digitalRead(TRIGGER_PIN) == LOW) {
          digitalWrite(PIN_LED, 0);
          dbPrintln("\nButton is long pressed");
          dbPrintln("Erasing Config, restarting");
          //WiFiManager wm;
          wm.resetSettings();
          digitalWrite(PIN_LED, 1);
          ESP.restart();
        }
        dbPrintln("");
      }
    }
  }
}
*/