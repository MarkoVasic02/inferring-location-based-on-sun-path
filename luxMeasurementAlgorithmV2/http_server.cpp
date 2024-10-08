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

void listFiles() {
    String fileList = "<html><body><h1>Available Files</h1><ul>";
    File root = SPIFFS.open("/");
    File file = root.openNextFile();
    while (file) {
        fileList += "<li><a href=\"/download?file=" + String(file.name()) + "\">" + String(file.name()) + "</a></li>";
        file = root.openNextFile();
    }
    fileList += "</ul></body></html>";
    server.send(200, "text/html", fileList);
}

void startHTTPServer() {
    server.on("/files", HTTP_GET, listFiles);
    server.on("/download", handleFileDownload);
    server.begin();
    Serial.println("HTTP server started.");
}
