#include <Arduino.h>

/*
 Basic ESP8266 MQTT example

 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.

 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.

 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"

*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <config.h>

#define RELAIS 0

WiFiClient espClient;
PubSubClient client(espClient);
char msg[50];

static bool state;

void setup_wifi() {
    bool info = true;
    digitalWrite(LED_BUILTIN, info);
    delay(10);
    info = !info;
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        //Serial.print(".");
        digitalWrite(LED_BUILTIN, info);
        info = !info;
    }
    randomSeed(micros());
    digitalWrite(LED_BUILTIN, false);
}

void callback(char* topic, byte* payload, unsigned int length) {
  if(strcmp(topic, "espRelais/switch") == 0 && length == 1)
  {
      // Switch on the LED if an 1 was received as first character
      bool state = (char)payload[0] == '1';
      digitalWrite(RELAIS, state);
  }
}

void reconnect() {
  // Loop until we're reconnected
  bool info = true;
  while (!client.connected()) {
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    client.connect(clientId.c_str());
    digitalWrite(LED_BUILTIN, info);
    info = !info;
  }
  client.subscribe("espRelais/switch");
  digitalWrite(LED_BUILTIN, false);
}

void setup() {
    state = false;
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(RELAIS, OUTPUT);
    digitalWrite(RELAIS, state);
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
    reconnect();
    snprintf (msg, 50, "%d", state);
    client.publish("espRelais/switch", msg);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
