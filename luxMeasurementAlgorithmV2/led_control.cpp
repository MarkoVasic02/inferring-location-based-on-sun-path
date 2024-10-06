#include "led_control.h"
#include <Arduino.h>

#define LED 33

void setupLED() {
    pinMode(LED, OUTPUT);
    digitalWrite(LED, HIGH);
    delay(250);
    digitalWrite(LED, LOW);
}

void blinkLED() {
    static unsigned long lastTime = millis();
    static bool ledState = LOW;
    
    if (millis() - lastTime >= 1000) {  // Blink every 1 second
        lastTime = millis();
        ledState = !ledState;
        digitalWrite(LED, ledState);
    }
}
