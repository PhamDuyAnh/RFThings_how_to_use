// https://console.hivemq.cloud/clusters/free/8fcfc29c35f04789a706f34250c4fc85
// https://console.hivemq.cloud/clusters/free/8fcfc29c35f04789a706f34250c4fc85/web-client
// config MQTT info
/*
MQTT Broker Info  ---> EMQX.io
Server: broker.emqx.io
TCP Port: 1883
WebSocket Port: 8083
SSL/TLS Port: 8883
Secure WebSocket Port: 8084

MQTT Broker Info  ---> HiveMQ.cloud
Server: ....s2.eu.hivemq.cloud
SSL/TLS Port: 8883
Secure WebSocket Port: 8884
*/

//#include <HTTPClient.h>
#include <WiFiClientSecure.h>  // https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/bearssl-client-secure-class.html
WiFiClient espClient;
WiFiClientSecure espClientSecure;

#include "src/PubSubClient.h"  // https://github.com/knolleary/pubsubclient/tree/master
PubSubClient mqtt;


#define mqtt_tcpssl 0  // 0 is don't use TCP SSL, default port 1883
// const int mqtt_port = 1883;
// const char* mqtt_server = "mqtt server";      // replace with your MQTT URL
// const char* mqtt_username = "mqtt username";  // replace with your Username
// const char* mqtt_password = "mqtt password";  // replace with your Password
const int mqtt_port = 1883;
const char* mqtt_server = "titops.org";  // replace with your HiveMQ Cluster URL
const char* mqtt_username = "titops";    // replace with your Username
const char* mqtt_password = "titops";    // replace with your Password

#define connectAfter 5000  // ms, Wait 5 seconds before retrying
//#define publishDelay 10000  // ms, Wait 10 seconds before publish message

#define MQTT_KEEPALIVE 15
#define MQTT_SOCKET_TIMEOUT 15

// define will_topic end will message
#define will_topic "will"
#define will_Mess "0"  // will message is "0" -> offline, "1" is online
#define will_QoS 0
#define will_Retain true

// HiveMQ Cloud Let's Encrypt CA certificate
static const char* root_ca PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
)EOF";

////////////////////////////////////////////////////////////////////////////////////////////////////
void initMQTT() {
  if (mqtt_tcpssl == 1) {
    mqtt.setClient(espClientSecure);
    dbPrintln("Set certificate");
    espClientSecure.setCACert(root_ca);  // enable this line and the the "certificate" code for secure connection
  } else {
    mqtt.setClient(espClient);
  }
  // mqtt.setServer(const char * domain, uint16_t port);
  mqtt.setServer(mqtt_server, mqtt_port);
  mqtt.setCallback(callback);

  loopMQTT();
}

void loopMQTT() {
  if (!mqtt.connected()) {
    static unsigned long lastConnect;
    unsigned long now = millis();

    // Loop until we’re reconnected
    //while (!mqtt.connected()) {
    if (now - lastConnect > connectAfter) {
      lastConnect = now;

      dbPrint("Connect to " + String(mqtt_server) + "\n");
      dbPrint("Attempting MQTT connection… ");
      delay(100);
      if (reconnect()) {
        dbPrintln("connected");
        mqtt_connected = true;
      } else {
        dbPrint("failed, rc = ");
        dbPrint(mqtt.state());
        dbPrintln(" try again in 5 seconds");
        mqtt_connected = false;
      }
    }
  } else {
    mqtt.loop();
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  dbPrint("Message arrived [");
  dbPrint(topic);
  dbPrint("] ");
  char pl[length];
  for (int i = 0; i < length; i++) {
    dbPrint((char)payload[i]);
    pl[i] = (char)payload[i];  // convert byte array to char array
  }
  dbPrintln();
  //dbPrintln(String(pl));
  String _topic = String(topic);  // convert char array to string
  if (_topic == will_topic) {
    unsigned int val = String(pl).toInt();  // convert char array to Int
    dbPrintln("Will topic payload is: " + String(val));
  }
}

bool reconnect() {
  //String clientId = chipID;
  // Attempt to connect
  // Insert your password
  // boolean connect(const char* id, const char* user, const char* pass, const char* willTopic, uint8_t willQos, boolean willRetain, const char* willMessage);
#ifdef will_topic
  //if (mqtt.connect(clientId.c_str(), mqtt_username, mqtt_password, (mqtt_TopicPrefix + String(will_topic)).c_str(), will_QoS, will_Retain, will_Mess))
  String _will = mqtt_TopicPrefix + String(will_topic);
  if (mqtt.connect(clientId.c_str(), mqtt_username, mqtt_password, _will.c_str(), will_QoS, will_Retain, will_Mess))
#else
  if (mqtt.connect(clientId.c_str(), mqtt_username, mqtt_password))
#endif
  {
    delay(100);
    // Once connected, publish an announcement…
    // publish(const char* topic, const char* payload, boolean retained);
    // default retained = false
#ifdef will_topic
    mqtt.publish(_will.c_str(), "1", will_Retain);
    mqtt.subscribe(_will.c_str());
#endif
    String _ssid_topic = mqtt_TopicPrefix + "ssid";
    mqtt.publish(_ssid_topic.c_str(), getFiwiSSID().c_str());
  }
  return mqtt.connected();
}


void publishMessage(String topic, String payload) {
  if (mqtt.publish(topic.c_str(), payload.c_str(), true))
    dbPrintln("Message publised [" + String(topic) + "]: " + payload);
}

/*
// Possible values for client.state()
#define MQTT_CONNECTION_TIMEOUT     -4
#define MQTT_CONNECTION_LOST        -3
#define MQTT_CONNECT_FAILED         -2
#define MQTT_DISCONNECTED           -1
#define MQTT_CONNECTED               0
#define MQTT_CONNECT_BAD_PROTOCOL    1
#define MQTT_CONNECT_BAD_CLIENT_ID   2
#define MQTT_CONNECT_UNAVAILABLE     3
#define MQTT_CONNECT_BAD_CREDENTIALS 4
#define MQTT_CONNECT_UNAUTHORIZED    5
*/