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
#include "config.h" // SERIAL_BAUD_RATE
#include "led.h"    // setupLED, setColor
#include "bird.h"   // createBird, updateBird

// ========== Global Variables ========== //

// ========== Global Functions ========== //

// ========== Setup ========== //
void setup() {
    // Random seed
    randomSeed(os_random());

    // Start Serial
    Serial.begin(SERIAL_BAUD_RATE);
    Serial.println("Starting Bird");

    // Setup LED(s)
    setupLED();

    // Setup Game
    createBird();

    // Set initial flag
    setFlagColor(flag);
}

// ========== Loop ========== //
void loop() {
    // Update game (access point, server, ...)
    updateBird();

    // Blink when devices are connected
    int connections = getConnections();
    if (connections) {
        blink(connections);
    } else {
        setFlagColor(flag);
    }

    // Serial CLI
    if (Serial.available()) {
        Serial.println(handleCLI(Serial.readStringUntil(SERIAL_DELIMITER)));
    }
}