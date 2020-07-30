/*
* Example of NodeMCU publish MQTT to Ubidots with multiple values
* Based on Ubidot API doc: https://ubidots.com/docs/hw/#send-data-publish
* Author: github.com/rintoprie
*/

#include <PubSubClient.h>
#include <ESP8266WiFi.h>

#define wifi_ssid ""
#define wifi_password ""

#define mqtt_server "things.ubidots.com"    // Ubidots MQTT Cloud address
#define mqtt_username ""                    // Ubidots token
#define topic "/v1.6/devices/mqtt1"         // Path from Ubidots device name

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
long temp;

void reconnect() {
    // Loop until we're reconnected
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (client.connect("nodemcubidots",mqtt_username,"")) {
            Serial.println("connected");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void setup() {
    Serial.begin(115200);
    WiFi.begin(wifi_ssid, wifi_password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    client.setServer(mqtt_server, 1883);
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
    
    long now = millis();
    if (now - lastMsg > 1000) {
        // Wait a few seconds between measurements
        lastMsg = now;

        temp = random(27, 37);
        // payload: {"temp": {"value": 34, "context":{"uid" : 518758}}}
        String payload = "{\"temp\": {\"value\": " + String(temp) + ", \"context\":{\"uid\" : " + String(now) + "}}}";
        int str_len = payload.length() + 1;
        char char_array[str_len];
        payload.toCharArray(char_array, str_len);
        Serial.print("payload: ");
        Serial.println(payload);
        client.publish(topic, char_array);
    }
}
