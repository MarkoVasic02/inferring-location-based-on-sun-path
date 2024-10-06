#include "filesystem.h"
#include <FS.h>
#include <SPIFFS.h>
#include <string>

void initFileSystem() {
    if (!SPIFFS.begin(true)) {
        Serial.println("An error has occurred while mounting SPIFFS");
        return;
    }
    Serial.println("SPIFFS mounted successfully.");
}

void resetData() {
    File file = SPIFFS.open("/light_data.txt", FILE_WRITE);
    if (!file) {
        Serial.println("Failed to open file for overwriting");
        return;
    }
    file.println("");  // Clear the file
    file.close();
    Serial.println("File emptied.");
}

// Save the table data to SPIFFS file
void saveTableToFile(String table) {
  File file = SPIFFS.open("/light_data.txt", FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  file.println(table);
  file.close();
  Serial.println("Table saved to light_data.txt");
}
