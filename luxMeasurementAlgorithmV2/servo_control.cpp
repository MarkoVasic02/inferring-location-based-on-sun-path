#include "servo_control.h"
#include "filesystem.h"
#include "light_sensor.h"

#define S1 19  // Vertical servo
#define S2 18  // Horizontal servo

Servo vertical;    // Create servo object for vertical movement
Servo horizontal;  // Create servo object for horizontal movement

int h0, v0;  // Global variables for angle positions

void initServos() {
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    vertical.setPeriodHertz(50);       // Standard 50Hz servo
    vertical.attach(S1, 700, 2900);    // Attach the vertical servo
    horizontal.setPeriodHertz(50);     // Standard 50Hz servo
    horizontal.attach(S2, 700, 2900);  // Attach the horizontal servo
}

void move(int v, int h) {
    if (v < 0) v = 0;
    if (v > 90) v = 90;
    if (h < 0) h = 0;
    if (h > 180) h = 180;

    h0 = h;
    v0 = v;

    vertical.write(90 - v);  // Reverse the vertical angle
    yield();
    horizontal.write(h);
    yield();
}

/*
    {
      { {vangle11, hangle11, lux11}, {vangle12, hangle12, lux12}, {vangle1n, hangle1n, lux1n} },
      { {vangle21, hangle21, lux21}, {vangle22, hangle22, lux22}, {vangle2n, hangle2n, lux2n} },
      ...
      { {vanglem1, hanglem1, luxm1}, {vanglem2, hanglem2, luxm2}, {vanglemn, hanglemn, luxmn} }
    }
*/
void scanTable(int vstart, int vend, int hstart, int hend) {
  String table = "{\n";
  bool reverse = false;

  for (int v = vstart; v <= vend; v += 5) {
    table += "  {";
    if (!reverse) {
      for (int h = hstart; h <= hend; h += 5) {
        move(v, h);
        delay(100);
        int lux = readlight();
        table += String("{") + v + ", " + h + ", " + lux + "}";
        if (h < hend) table += ", ";
      }
    } else {
      for (int h = hend; h >= hstart; h -= 5) {
        move(v, h);
        delay(100);
        int lux = readlight();
        table += String("{") + v + ", " + h + ", " + lux + "}";
        if (h > hstart) table += ", ";
      }
    }
    reverse = !reverse;
    table += " }";
    if (v < vend) table += ",\n";
  }
  table += "\n}\n";

  saveTableToFile(table);
}

void scanTable2(int vstart, int vend, int hstart, int hend) {
  Serial.print("{");  // Start of the list
  bool reverse = false;
  for (int v = vstart; v <= vend; v += 5) {
    Serial.print("{");  // Start of a nested list
    if (!reverse) {
      for (int h = hstart; h <= hend; h += 5) {
        move(v, h);
        delay(100);
        int lux = readlight();
        Serial.print("{");
        Serial.print(v);
        Serial.print(", ");
        Serial.print(h);
        Serial.print(", ");
        Serial.printf("%d", lux);
        Serial.print("}");
        if (h < hend) Serial.print(",");
      }
    } else {
      for (int h = hend; h >= hstart; h -= 5) {
        move(v, h);
        delay(100);
        int lux = readlight();
        Serial.print("{");
        Serial.print(v);
        Serial.print(", ");
        Serial.print(h);
        Serial.print(", ");
        Serial.printf("%d", lux);
        Serial.print("}");
        if (h > hstart) Serial.print(",");
      }
    }
    reverse = !reverse;
    Serial.print("}");
    if (v < vend) Serial.print(",");  // Add comma between nested lists
  }
  Serial.print("}\n");  // End of the list
  // saveTableToFile(table);
}
