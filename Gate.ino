/** If someone is reading this by any chance:
 *  
 *  - My relay is a low-voltage trigger, check your relay's
 *    spec sheet and change LOW to HIGH in the callback if needed
 *    
 *  - Put your own values where you see <>
**/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define SSID "<WIFI_SSID>"
#define PASSWORD "<WIFI_PASSWD>"

#define MQTT_SERVER "<SERVER_ADDRESS>"
#define MQTT_USER "<USERNAME>"
#define MQTT_PASSWORD "<PASSWD>"
#define CLIENT_ID "<CLIENT_ID>"
#define TOPIC "<TOPIC>"

// Relay trigger pin
#define RELAY_PIN D2

//const char phrase[15] = "<PHRASE(Not used)>";

void callback(char* topic, byte* payload, unsigned int length) {
    /*for(unsigned int i = 0; i < length; i++) {
        if(((char) payload[i]) != phrase[i]) {
            return;
        }
    }*/

    // Turn on the LED and power/trigger the relay coil
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(RELAY_PIN, LOW);

    // Wait 400ms to imitate a human pressing the buttton
    delay(400);

    // Turn off LED and un-trigger the relay
    digitalWrite(RELAY_PIN, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
}

WiFiClient espClient;
PubSubClient client(MQTT_SERVER, <PORT>, callback, espClient);

void setup() {
    /* Note: The order here is important to avoid accidental
     *  triggers at startup however unlikely.
     * Also check your relay specs to see if it's HIGH or LOW trigger.
     */
    // Set RELAY_PIN to HIGH and then pin mode as OUTPUT
    digitalWrite(RELAY_PIN, HIGH);
    pinMode(RELAY_PIN, OUTPUT);

    // Turn on LED to show ESP is trying to establish a connection
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    WiFi.begin(SSID, PASSWORD);
    while(WiFi.status() != WL_CONNECTED) {
        delay(500);
    }

    // Set server and register callback
    client.setServer(MQTT_SERVER, <PORT>);
    client.setCallback(callback);

    // Turn off LED
    digitalWrite(LED_BUILTIN, HIGH);
}

void reconnect() {
    digitalWrite(LED_BUILTIN, LOW);

    while(!client.connected()) {
        if(client.connect(CLIENT_ID, MQTT_USER, MQTT_PASSWORD)) {
            client.subscribe(TOPIC);
        }
        else {
            delay(5000);
        }
    }

    digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
    if(!client.connected()) {
        reconnect();
    }
    client.loop();
}
