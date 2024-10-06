#include "wifi_setup.h"
#include <WiFi.h>

void connectToWiFi() {
    String ssid, password;

    Serial.println("Enter Wi-Fi SSID:");
    while (ssid.length() == 0) {
        if (Serial.available()) {
            ssid = Serial.readStringUntil('\n');
        }
    }

    Serial.println("Enter Wi-Fi Password:");
    while (password.length() == 0) {
        if (Serial.available()) {
            password = Serial.readStringUntil('\n');
        }
    }

    Serial.printf("Connecting to %s...\n", ssid.c_str());
    WiFi.begin(ssid.c_str(), password.c_str());

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nConnected to Wi-Fi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}
