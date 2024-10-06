#include "servo_control.h"
#include "light_sensor.h"
#include "command_handler.h"
#include "filesystem.h"
#include "wifi_setup.h"
#include "http_server.h"
#include "led_control.h" 

void setup() {
    Serial.begin(115200);
    delay(2000);
    Serial.println("\nLightTracker1 - Djordje Herceg, Aleksandar Horvat 9.7.2024.");

    initFileSystem();         // Initialize SPIFFS
    connectToWiFi();          // Connect to Wi-Fi
    initServos();             // Initialize servos
    initLightSensor();        // Initialize BH1750 light sensor
    startHTTPServer();        // Start the HTTP server
    setupLED();

    Serial.println("LightTracker Ready.");
    Serial.println("Type commands: READ, TABLE, START, STOP, RESET_DATA, RESTART");
}

void loop() {
    server.handleClient();    // Handle HTTP server requests
    handleSerialInput();      // Handle serial input for commands
    autoScanCheck();          // Check if it's time for automatic scan
    blinkLED();
}
