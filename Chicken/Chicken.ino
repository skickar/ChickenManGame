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

// [Libraries]
 #include <SimpleCLI.h>

// [Local files]
#include "Bird.h"
#include "LED.h"
#include "Web.h"
#include "EEPROMHelper.h"

#include "config.h"
#include "hardware.h"
#include "types.h"

// ========== Global Variables ========== //

LED  led;  // LED
Bird bird; // Chicken game instance
Web  web;  // Web interface

// Command Line Interface
SimpleCLI cli;
Command   cmdFlag;
Command   cmdPoints;
Command   cmdReset;

int pointBlinkCounter = 0;

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
            return bird.getPointsString(true);
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

// ========== Setup ========== //
void setup() {
    // If resetted 3 times in a row
    if (!EEPROMHelper::checkBootNum(EEPROM_SIZE, EEPROM_BOOT_ADDR)) {
        // Erase (overwrite) old stats
        game_stats emptyStats;
        EEPROMHelper::saveObject(EEPROM_SIZE, EEPROM_STATS_ADDR, emptyStats);
    }

    // Random seed
    randomSeed(os_random());

    // Start Serial
    Serial.begin(SERIAL_BAUD_RATE);
    Serial.println("Starting Bird");

    // Setup LED(s)
    led.begin();

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

    // Start web interface
    web.begin();

    // Set boot counter back to 1
    EEPROMHelper::resetBootNum(EEPROM_SIZE, EEPROM_BOOT_ADDR);
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
    web.update();
}