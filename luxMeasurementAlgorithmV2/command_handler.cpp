#include "command_handler.h"
#include "servo_control.h"
#include "light_sensor.h"
#include "filesystem.h"
#include <Arduino.h>
#include <string>

char serialbuf[80];
bool autoScan = false;  // Flag to control the automatic scan
unsigned long lastScanTime = 0;  // To keep track of the last scan time
unsigned long scanInterval = 15 * 60 * 1000;  // Default to 15 minutes in milliseconds

void handleCommand(const char*);
bool parseAngles(const std::string&, int&, int&);
bool parseTableArgs(const std::string&, int&, int&, int&, int&);

void handleSerialInput() {
    if (Serial.available()) {
        int len = Serial.readBytesUntil('\n', serialbuf, 80);
        serialbuf[len] = '\0';
        ///
        Serial.print("\nYou entered: >");
        Serial.print(serialbuf);
        Serial.println("<");
        ///
        handleCommand(serialbuf);
    }
}

void handleCommand(const char* command) {
  std::string s = command;

  if (s.find("READ") == 0) {
    // Handle READ command
    int vangle, hangle;
    if (parseAngles(s.substr(5), vangle, hangle)) {
      move(vangle, hangle);
      int lux = readlight();
      Serial.printf("{%d, %d, %d}\n", vangle, hangle, lux);
    }
  } else if (s.find("TABLE") == 0) {
    // Handle TABLE command
    int vstart, vend, hstart, hend;
    if (parseTableArgs(s.substr(6), vstart, vend, hstart, hend)) {
      scanTable(vstart, vend, hstart, hend);
      move(0,0);
    }
  } else if (s.find("START") == 0) {
    // Check if there's an argument after START for custom interval
    int intervalMinutes = 15;  // Default to 15 minutes
    std::string intervalStr = s.substr(6);
    
    if (!intervalStr.empty()) {
        try {
            intervalMinutes = std::stoi(intervalStr);  // Convert argument to integer
        if (intervalMinutes <= 0) {
            Serial.println("Invalid interval. Must be greater than 0.");
            return;
        }
        } catch (...) {
            Serial.println("Invalid interval format.");
            return;
        }
    }
    scanInterval = intervalMinutes * 60 * 1000;  // Convert minutes to milliseconds
    autoScan = true;
    lastScanTime = millis();  // Reset timer
    Serial.printf("Automatic scanning started. Interval: %d minutes.\n", intervalMinutes);

  } else if (s.find("STOP") == 0) {
    // Stop automatic scanning
    autoScan = false;
    Serial.println("Automatic scanning stopped.");
  } else if (s.find("RESET_DATA") == 0) {
    // FLUSH SPIFFS MEMORY
    resetData();
  } else if (s.find("RESTART") == 0) {
    // Restart the ESP32
    Serial.println("Restarting ESP32...");
    delay(500);  // Give some time for the message to print
    ESP.restart();  // Trigger a restart
  }
}


void autoScanCheck() {
    if (autoScan && millis() - lastScanTime >= scanInterval) {
        lastScanTime = millis();
        Serial.println("Running automatic table scan.");
        scanTable(0, 90, 0, 180); // Implement this function in `servo_control`
    }
}

bool parseAngles(const std::string &drugi, int &vangle, int &hangle) {
  int f = drugi.find(",");
  if (f == -1) return false;
  std::string vstr = drugi.substr(0, f);
  std::string hstr = drugi.substr(f + 1);
  try {
    vangle = std::stoi(vstr);
    hangle = std::stoi(hstr);
    return true;
  } catch (...) {
    Serial.println("Invalid angles");
    return false;
  }
}

bool parseTableArgs(const std::string &drugi, int &vstart, int &vend, int &hstart, int &hend) {
  int f1 = drugi.find(",");
  if (f1 == -1) return false;
  std::string vstartstr = drugi.substr(0, f1);

  int f2 = drugi.find(",", f1 + 1);
  if (f2 == -1) return false;
  std::string vendstr = drugi.substr(f1 + 1, f2 - f1 - 1);

  int f3 = drugi.find(",", f2 + 1);
  if (f3 == -1) return false;
  std::string hstartstr = drugi.substr(f2 + 1, f3 - f2 - 1);
  std::string hendstr = drugi.substr(f3 + 1);

  try {
    vstart = std::stoi(vstartstr);
    vend = std::stoi(vendstr);
    hstart = std::stoi(hstartstr);
    hend = std::stoi(hendstr);
    return true;
  } catch (...) {
    Serial.println("Invalid table args");
    return false;
  }
}
