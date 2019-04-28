#include "Web.h"

// ========== Global (but not really) Variables ========== //
ESP8266WebServer webServer(80);

// ========== Global (but not really) Functions ========== //
// index.html
void handleRoot() {
    String answer;

    // When a command argument was attached, run it through the CLI
    if (webServer.hasArg("cmd")) {
        String input = webServer.arg("cmd");
        Serial.printf("[Web]# %s\n", input.c_str());
        answer = handleCLI(input);
        Serial.println(answer);
    }

    // Build HTML string
    String html(HTML_PREFIX);
    html += answer; // Whatever the CLI returned
    html += String(HTML_SUFFIX);

    // Send HTML to user
    webServer.send(200, "text/html", html);
}

// points.html
void handlePoints() {
    Serial.println("[Web]# points");
    String answer = handleCLI("points");
    webServer.send(200, "text/html", answer);
    Serial.println(answer);
}

// 404 Site Not Found
void handleNotFound() {
    // Build error message
    String message = "404 - Chicken Not Found :(\n\n";

    // Attack infos about the URL and its arguments
    message += "URI: ";
    message += webServer.uri();
    message += "\nMethod: ";
    message += (webServer.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += webServer.args();
    message += "\n";

    for (int i = 0; i < webServer.args(); i++) {
        message += " " + webServer.argName(i) + ": " + webServer.arg(i) + "\n";
    }

    // Send message to user
    webServer.send(404, "text/plain", message);
}

// ========== Public ========== //
void Web::begin() {
    // DNS
    MDNS.begin("Chicken");

    // Web Server
    webServer.on("/", handleRoot);
    webServer.on("/index.html", handleRoot);
    webServer.on("/points.html", handlePoints);
    webServer.onNotFound(handleNotFound);

    webServer.begin();
}

void Web::update() {
    MDNS.update();
    webServer.handleClient();
}
