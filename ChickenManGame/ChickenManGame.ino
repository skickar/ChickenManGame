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
#include "Man.h"
#include "LED.h"
#include "Web.h"
#include "EEPROMHelper.h"

#include "config.h"
#include "hardware.h"
#include "types.h"

// ========== Global Variables ========== //

Bird bird; // Chicken game instance
Man  man;  // Chicken man
LED  led;  // LED
Web  web;  // Web interface

// Command Line Interface
SimpleCLI cli;
Command   cmdFlag;
Command   cmdPoints;
Command   cmdReset;

int pointBlinkCounter   = 0;
GAME_TYPE type          = NO_TYPE;
unsigned long sleepTime = 0;

// ========== Global Functions ========== //

// CLI handler that is called via serial and the web interface
String handleCLI(String input) {
    // Parse input
    cli.parse(input);

    // Check for commands
    if (cli.available()) {
        // Get the command that was entered/parsed successfully
        Command cmd = cli.getCommand();

        // Flag
        if ((cmd == cmdFlag) && (type == CHICKEN)) {
            // Get the team color that was entered
            String team = cmd.getArgument("team").getValue();

            // Set flags and return a reply string
            if (team.equalsIgnoreCase("blue")) {
                led.setColor(BLUE);
                bird.setFlag(BLUE);
                return "Why so blue?";
            } else if (team.equalsIgnoreCase("red")) {
                led.setColor(RED);
                bird.setFlag(RED);
                return "Seeing red?";
            } else if (team.equalsIgnoreCase("green")) {
                led.setColor(GREEN);
                bird.setFlag(GREEN);
                return "The others must be green with envy!";
            } else {
                return "Wrong team, mate!";
            }
        }

        // Points
        else if ((cmd == cmdPoints)) {
            if (type == CHICKEN_MAN) {
                return man.getPointsString();
            } else {
                pointBlinkCounter = 4;
                return bird.getPointsString(true);
            }
        }

        // Reset
        else if ((cmd == cmdReset)) {
            String res = "Resetting game...";

            String password = cmd.getArgument("pswd").getValue();
            bool   success;

            if (type == CHICKEN_MAN) {
                success = man.resetGame(password);
            } else {
                success = bird.resetGame(password);
            }

            res += success ? "Done" : "Wrong password";

            return res;
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
    // Start Serial
    Serial.begin(SERIAL_BAUD_RATE);
    Serial.println("Starting");

    // Random seed
    randomSeed(os_random());

    // Start EEPROM
    EEPROMHelper::begin(EEPROM_SIZE);

    // If resetted 3 times in a row
    if (!EEPROMHelper::checkBootNum(EEPROM_BOOT_ADDR)) {
        // Erase (overwrite) old stats
        game_stats emptyStats;
        EEPROMHelper::saveObject(EEPROM_STATS_ADDR, emptyStats);
    }

    // Setup LED(s)
    led.begin();

    // Setup Switch
    pinMode(SWITCH_PIN, INPUT_PULLUP);

    // CLI
    cmdFlag = cli.addCommand("flag,color,led");
    cmdFlag.addPositionalArgument("t/eam", "green");

    cmdPoints = cli.addCommand("points");

    cmdReset = cli.addCommand("reset");
    cmdReset.addArgument("p/assword,pswd");

    // ========== CHICKEN MAN ========== //
    if (digitalRead(SWITCH_PIN) == LOW) {
        Serial.println("Mode: Chicken Man");

        type = CHICKEN_MAN;

        // Make sure it's in station mode
        WiFi.mode(WIFI_STA);
        WiFi.disconnect();

        // Start the chicken man
        man.begin();

        // Set flag color
        led.setColor(man.getFlag());
    }

    // ========== CHICKEN ========== //
    else {
        Serial.println("Mode: Chicken");

        type = CHICKEN;

        // Setup Game Access Point
        bird.begin();

        // Start web interface
        web.begin();

        // Set flag color
        led.setColor(bird.getFlag());
    }

    // Set boot counter back to 1
    EEPROMHelper::resetBootNum(EEPROM_BOOT_ADDR);
}

// ========== Loop ========== //
void loop() {
    // Serial CLI
    if (Serial.available()) {
        String input = Serial.readStringUntil(SERIAL_DELIMITER);

        // Echo the input on the serial interface
        Serial.print("# ");
        Serial.println(input);

        Serial.println(handleCLI(input));
    }

    // ========== CHICKEN MAN ========== //
    if (type == CHICKEN_MAN) {
        unsigned long difference = millis() - sleepTime;

        if ((sleepTime == 0) || (difference >= CHICKEN_MAN_INTERVAL * 1000)) {
            sleepTime = millis();

            Serial.println();

            man.update();

            pointBlinkCounter = 4;

            Serial.printf("Going to sleep for %lus...", 30 - ((millis() - sleepTime) / 1000));
        } else if (difference % 2000 == 0) {
            Serial.print("Z");
        } else if (difference % 1000 == 0) {
            Serial.print("z");
        }

        // Blink LED(s) when eggs are gathered
        if (pointBlinkCounter) {
            pointBlinkCounter = led.blink(4, man.getFlag(), pointBlinkCounter);
            if (pointBlinkCounter == 0) led.setColor(man.getFlag());
        }

        delay(1);
    }

    // ========== CHICKEN ========== //
    else {
        // If something went wrong, blink like crazy for 5s and then restart
        if (bird.errored()) {
            unsigned long startTime = millis();

            while (millis() - startTime >= 5000) {
                led.blink(10, NO_TEAM);
            }

            ESP.restart();
        }

        // Web server
        web.update();

        // Update game (access point, server, ...)
        bird.update();

        // Blink LED(s) when eggs are gathered
        if (pointBlinkCounter) {
            pointBlinkCounter = led.blink(4, bird.getFlag(), pointBlinkCounter);
            if (pointBlinkCounter == 0) led.setColor(bird.getFlag());
        }
    }
}
