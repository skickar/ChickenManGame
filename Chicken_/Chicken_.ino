/*
   Copyright (c) 2019 Kody Kinzie, Brandon Paiz for Leadership In Technology & RETIA.IO
   This software is licensed under the MIT License. See the license file for details.
   TUNNEL SNAKES RULE!

   This example is based on the simpleCLI example for the ESP8266, Copyright (c) 2019 Stefan Kremser

   The Chicken Man Game is a multi-player Wi-Fi cracking game. You can use 1, 2, or 3 players (or teams).
   The esp8266 checks the difficulty level and creates a Wi-Fi network using a password from that difficulty list
   Once the player cracks the password and logs in to the Wi-Fi network, they can set the LED to their team color.
   Setting the LED causes the esp8266 to close the connection and create a new Wi-Fi network with a harder password.
   Players can then attempt to steal the flag by hacking the harder password and setting the LED to their team color.
   Currently supports easy, medium, and hard mode. Password lists can be expanded to avoid collisions.
 */

// ========== Includes ========== //

// [ESP8266 Libraries]
 #include <ESP8266WebServer.h> // ESP8266WebServer
 #include <ESP8266mDNS.h>      // MDNS

// [External Libraries]
 #include <SimpleCLI.h>        // SimpleCLI

// [Local files]
#include "Bird_.h"
#include "LED_.h"

#include "config.h"
#include "hardware.h"


// ========== Global Variables ========== //
ESP8266WebServer webServer(80); // Web Server

SimpleCLI cli;                  // Command Line Interface
Command   cmdFlag;
Command   cmdPoints;
Command   cmdReset;

LED  led;
Bird bird; // Game instance

// ========== Global Functions ========== //
String handleCLI(String input) {
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

            if (team.equalsIgnoreCase("blue")) {
                bird.setFlag(2);
                return "Why so blue?";
            } else if (team.equalsIgnoreCase("red")) {
                bird.setFlag(0);
                return "Seeing red?";
            } else if (team.equalsIgnoreCase("green")) {
                bird.setFlag(1);
                return "The others must be green with envy!";
            } else {
                return "Wrong team, mate!";
            }
        } else if (cmd == cmdPoints) {
            return bird.getPointsString();
        } else if (cmd == cmdReset) {
            if (bird.reset(cmd.getArgument("pswd").getValue())) {
                return "Resetted game stats!";
            };
        }
    }

    // Check for errors
    if (cli.errored()) {
        return cli.getError().toString();
    }

    return String();
}

void handleRoot() {
    String answer;

    // If data was sent
    if ((webServer.args() > 0) && webServer.hasArg("cmd")) {
        String input = webServer.arg("cmd");
        answer = handleCLI(input);
    }

    // Build HTML string
    String html(HTML_PREFIX);
    html += answer;
    html += String(HTML_SUFFIX);

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

// ========== Setup ========== //
void setup() {
    // Random seed
    randomSeed(os_random());

    // Start Serial
    Serial.begin(115200 /*SERIAL_BAUD_RATE*/);
    Serial.println("Starting Bird");

    // Setup LED(s)
    led.begin();
    led.setFlagColor(1);

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

    // Setup Game Access Point
    bird.begin();
}

// ========== Loop ========== //
void loop() {
    // Update game (access point, server, ...)
    bird.update();

    // Blink when devices are connected
    int connections = bird.getConnections();
    if (connections) {
        led.blink(connections);
    } else {
        led.setFlagColor(bird.getFlag());
    }

    // Serial CLI
    if (Serial.available()) {
        Serial.println(handleCLI(Serial.readStringUntil('\n' /*SERIAL_DELIMITER*/)));
    }

    // Web server
    MDNS.update();
    webServer.handleClient();
}