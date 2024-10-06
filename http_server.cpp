#include "http_server.h"
#include <WebServer.h>
#include <SPIFFS.h>

WebServer server(80);

void handleFileDownload() {
    if (server.uri() == "/download") {
        File file = SPIFFS.open("/light_data.txt", "r");
        if (!file) {
            server.send(500, "text/plain", "File not available");
            return;
        }
        server.streamFile(file, "text/plain");
        file.close();
    }
}

void startHTTPServer() {
    server.on("/download", handleFileDownload);
    server.begin();
    Serial.println("HTTP server started.");
}
