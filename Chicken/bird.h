#ifndef BIRD_H
#define BIRD_H

// ========== Includes ========== //

// [ESP8266 Libraries]
#include <ESP8266WebServer.h>   // ESP8266WebServer
#include <ESP8266mDNS.h>        // MDNS

// [External Libraries]
#include <SimpleCLI.h>          // SimpleCLI

// [Local files]
#include "config.h"             // NUM_PASSWORDS, SUPER_SECRET, ...

// ========== Global Variables ========== //
ESP8266WebServer webServer(80); // Web Server

SimpleCLI cli;                  // Command Line Interface
Command   cmdFlag;
Command   cmdPoints;
Command   cmdReset;

String ssid;                         // SSID of access point
String pswd;                         // Password of access point

unsigned long lastPointUpdate = 0;   // Last time the team points were updated

unsigned int id    = 0;              // Random generated ID
unsigned int level = 0;              // 0: Easy, 1: Medium, 2: Hard, 3: Locked
unsigned int flag  = 1;              // 0: Red, 1: Green, 3: Blue

unsigned int points[] = { 0, 0, 0 }; // Red-Points, Green-Points, Blue-Points

// ========== Global Functions ========== //

// Team-Point-Calculator
int calculatePoints(unsigned int level) {
    if (millis() - lastPointUpdate >= POINT_INTERVAL_SECONDS * 1000) {
        lastPointUpdate = millis();

        level = level > 3 ? 0 : level; // Invalid level? Assume easy!

        if (level != 3) {              // Not locked
            const int point_allocations[] = { EASY_POINTS_PER_SECOND, MEDIUM_POINTS_PER_SECOND, HARD_POINTS_PER_SECOND };
            return point_allocations[level];
        }
    }

    return 0;
}

// Access-Point-Helper-Functions
String getPassword(unsigned int level, unsigned int id) {
    level = level > 3 ? 0 : level;        // Invalid level? Assume easy!
    id    = id >= NUM_PASSWORDS ? 0 : id; // Invalid ID? Assume ID 0!

    if (level == 3) return String(SUPER_SECRET);

    const char** pswdList[] = { easyPass, mediumPass, hardPass };
    return String(pswdList[level][id]);
}

String getSSIDPrefix(unsigned int level) {
    level = level > 3 ? 0 : level; // Invalid level? Assume easy!

    const char* prefixList[] = { prefix0, prefix1, prefix2, prefix3 };
    return String(prefixList[level]);
}

String getSSIDNumber(unsigned int id) {
    if (id >= NUM_PASSWORDS) return String("_?");  // Invalid ID

    return String(numberAdd[id]);
}

int getConnections() {
    return WiFi.softAPgetStationNum();
}

void createAP() {
    // Disconnect clients
    WiFi.softAPdisconnect();

    // Set IP Adress
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0));

    // Create SSID and password
    ssid = getSSIDPrefix(level) + getSSIDNumber(id);
    pswd = getPassword(level, id);

    // Start access point
    WiFi.softAP(ssid.c_str(), pswd.c_str(), id % 11, false, 3);

    // Print infos
    Serial.println("--------------------------------------------");
    Serial.printf("Chicken Level: \t%d\n", level);
    Serial.printf("Chicken ID: \t%d\n", id);
    Serial.printf("Channel: \t\t%d\n", id % 11);
    Serial.printf("IP-Address: \t%s\n", WiFi.softAPIP().toString().c_str());
    Serial.printf("SSID: \t\t"); Serial.println(ssid);
    Serial.printf("Password: \t\t"); Serial.println(pswd);
    Serial.println("---------------------------------------------");
}

String handleCLI(String input) {
    String answer;

    // Echo the input on the serial interface
    Serial.print("# ");
    Serial.println(input);

    // Parse input
    cli.parse(input);

    // Check for commands
    if (cli.available()) {
        Command cmd = cli.getCommand();

        if (cmd == cmdFlag) {
            String team = cmd.getArgument("team").getValue();

            if (level < 3) {
                if (team.equalsIgnoreCase("blue")) {
                    answer = "Why so blue?";
                    flag   = 2;
                    ++level;
                    createAP();
                } else if (team.equalsIgnoreCase("red")) {
                    answer = "Seeing red?";
                    flag   = 0;
                    if (level < 3) ++level;
                    createAP();
                } else if (team.equalsIgnoreCase("green")) {
                    answer = "The others must be green with envy!";
                    flag   = 1;
                    if (level < 3) ++level;
                    createAP();
                } else {
                    answer = "Wrong team, mate!";
                }
            }
        } else if (cmd == cmdPoints) {
            for (int i = 0; i < 3; i++) {
                answer += String(points[i]) + String(',');
            }
        } else if (cmd == cmdReset) {
            Argument argPswd = cmd.getArgument("pswd");
            if (argPswd.getValue() == SUPER_SECRET) {
                for (int i = 0; i < 3; i++) {
                    points[i] = 0;
                }
                lastPointUpdate = 0;
                level           = 0;
                flag            = 1;
                createAP();
                answer = "Resetted game stats!";
            }
        }
    }

    // Check for errors
    if (cli.errored()) {
        answer = cli.getError().toString();
    }

    return answer;
}

void handleRoot() {
    String answer;

    // If data was sent
    if ((webServer.args() > 0) && webServer.hasArg("cmd")) {
        String input = webServer.arg("cmd");
        answer = handleCLI(input);
    }

    // Build HTML string
    String html(indexHTMLPrefix);
    html += answer;
    html += String(indexHTMLSuffix);

    // Send HTML to user
    webServer.send(200, "text/html", html.c_str());
}

void handleNotFound() {
    String message = "404 - Chicken Not Found :(\n\n";

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

    webServer.send(404, "text/plain", message);
}

void createBird() {
    // Generate random ID
    id = random(0, NUM_PASSWORDS);

    // DNS
    MDNS.begin("ChickenManGame");

    // Web Server
    webServer.on("/", handleRoot);
    webServer.on("/index.html", handleRoot);
    webServer.onNotFound(handleNotFound);

    webServer.begin();

    // CLI
    cmdFlag = cli.addCommand("flag,color,led");
    cmdFlag.addPositionalArgument("t/eam", "green");

    cmdPoints = cli.addCommand("points");

    cmdReset = cli.addCommand("reset");
    cmdReset.addArgument("p/assword,pswd");

    // Access Point
    createAP();
}

void updateBird() {
    points[flag] += calculatePoints(level);
    MDNS.update();
    webServer.handleClient();
}

#endif /* ifndef BIRD_H */