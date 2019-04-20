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
 #include <ESP8266WebServer.h>
 #include <ESP8266mDNS.h>
 #include <EEPROM.h>

// [External Libraries]
 #include <SimpleCLI.h>

// [Local files]
#include "Bird.h"
#include "LED.h"

#include "config.h"
#include "hardware.h"
#include "types.h"

// ========== Constants ========== //
#define BOOT_ADDR 3210 // Boot counter address in EEPROM

// ========== Global Variables ========== //
ESP8266WebServer webServer(80);

LED  led;
Bird bird; // Game instance

int pointBlinkCounter = 0;

// Command Line Interface
SimpleCLI cli;
Command   cmdFlag;
Command   cmdPoints;
Command   cmdReset;

// ========== Global Functions ========== //

// CLI handler that is called via serial and the web interface
String handleCLI(String input) {
    // Echo the input on the serial interface
    Serial.print("# ");
    Serial.println(input);

    // Parse input
    cli.parse(input);

    // Check for commands
    if (cli.available()) {
        // Get the command that was entered/parsed successfully
        Command cmd = cli.getCommand();

        // Flag
        if (cmd == cmdFlag) {
            // Get the team color that was entered
            String team = cmd.getArgument("team").getValue();

            // Set flags and return a reply string
            if (team.equalsIgnoreCase("blue")) {
                bird.setFlag(BLUE);
                return "Why so blue?";
            } else if (team.equalsIgnoreCase("red")) {
                bird.setFlag(RED);
                return "Seeing red?";
            } else if (team.equalsIgnoreCase("green")) {
                bird.setFlag(GREEN);
                return "The others must be green with envy!";
            } else {
                return "Wrong team, mate!";
            }
        }

        // Points
        else if (cmd == cmdPoints) {
            pointBlinkCounter = 4;
            return bird.getPointsString();
        }

        // Reset
        else if (cmd == cmdReset) {
            if (bird.resetGame(cmd.getArgument("pswd").getValue())) {
                return "Resetted game stats!";
            };
        }
    }

    // Check for errors
    if (cli.errored()) {
        return cli.getError().toString();
    }

    // Return empty string if everything else fails
    return String();
}

// Web interface - index.html
void handleRoot() {
    String answer;

    // When a command argument was attached, run it through the CLI
    if (webServer.hasArg("cmd")) {
        String input = webServer.arg("cmd");
        answer = handleCLI(input);
    }

    // Build HTML string
    String html(HTML_PREFIX);
    html += answer; // Whatever the CLI returned
    html += String(HTML_SUFFIX);

    // Send HTML to user
    webServer.send(200, "text/html", html);
}

// Web interface - points.html
void handlePoints() {
    pointBlinkCounter = 4;                                    // Blink 4 times
    webServer.send(200, "text/html", bird.getPointsString()); // return points string
}

// Web interface - 404 Site Not Found
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

void checkBootCounter() {
    EEPROM.begin(4095); // Start EEPROM

    // Read out previous boot counter
    uint8_t boots = 0;
    EEPROM.get(BOOT_ADDR, boots);

    // Counter too high, reset game stats
    if (boots >= 3) {
        // Only erase the magic_num that is used to check if the memory is valid
        for (int i = STATS_ADDR; i < 4; i++) {
            EEPROM.write(i, 0);
        }
    }

    // Increment the boot counter and save it
    EEPROM.write(BOOT_ADDR, ++boots);
    EEPROM.commit();

    EEPROM.end(); // End EEPROM
}

void resetBootCounter() {
    EEPROM.begin(4095); // Start EEPROM

    // Save boot counter as 1
    EEPROM.put(BOOT_ADDR, 1);
    EEPROM.commit();

    EEPROM.end(); // End EEPROM
}

// ========== Setup ========== //
void setup() {
    // Erase old stats, if resetted 3 times in a row
    checkBootCounter();

    // Random seed
    randomSeed(os_random());

    // Start Serial
    Serial.begin(SERIAL_BAUD_RATE);
    Serial.println("Starting Bird");

    // Setup LED(s)
    led.begin();

    // DNS
    MDNS.begin("ChickenManGame");

    // Web Server
    webServer.on("/", handleRoot);
    webServer.on("/index.html", handleRoot);
    webServer.on("/points.html", handlePoints);
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

    // If something went wrong, blink like crazy for 5s and then restart
    if (bird.errored()) {
        unsigned long startTime = millis();

        while (millis() - startTime >= 5000) {
            led.blink(10, NONE);
        }

        ESP.restart();
    }

    // Set boot counter to 1, to indicate that the the game has successfully started
    resetBootCounter();
}

// ========== Loop ========== //
void loop() {
    // Update game (access point, server, ...)
    bird.update();

    // Blink LED(s) when eggs are gathered
    if (pointBlinkCounter) {
        pointBlinkCounter = led.blink(8, bird.getFlag(), pointBlinkCounter);
    }
    // Otherwise set the current flag color
    else {
        led.setColor(bird.getFlag());
    }

    // Serial CLI
    if (Serial.available()) {
        Serial.println(handleCLI(Serial.readStringUntil(SERIAL_DELIMITER)));
    }

    // Web server
    MDNS.update();
    webServer.handleClient();
}