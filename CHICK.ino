/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/SimpleCLI

   This example is based on the HelloServer example for the ESP8266.
   You connect to the Access-Point, go to 192.168.4.1 and type in "led on" or "led off".
   The parsing magic is happening in handleRoot().
 */

#include <SimpleCLI.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "constants.h"

int level = 0; 

// assuming each list has at least NUM_PASSWORDS
const char* getPassword(int diff){
  // invalid difficulty? assume easy
  diff = diff < 0 || diff > 2 ? 0 : diff;
  const char** pass_list[] = {easyPass, mediumPass, hardPass};
  return pass_list[diff][random(0,NUM_PASSWORDS)];
}

const char* getSSIDPrefix(int diff){
  // invalid difficulty? assume easy
  diff = diff < 0 || diff > 2 ? 0 : diff;
  const char* diff_list[] = {prefix0, prefix1, prefix2};
  return diff_list[diff];
}

const char* getSSIDNumber(){
  // assume possible numbers is always same as number of passwords
  return numberAdd[random(0, NUM_PASSWORDS)];
}

void createBird(){
  // get ssid/pass stuff according to difficulty level
  String ssid;
    ssid.concat(getSSIDPrefix(level));
    ssid.concat(getSSIDNumber());
  String password(getPassword(level));

  Serial.println(ssid);
  Serial.println(password);
  WiFi.softAP(ssid.c_str(), password.c_str());
}









ESP8266WebServer server(80);
SimpleCLI cli;

String  answer;
Command cmdLed;

void handleRoot() {
    // If data was sent
    if (server.args() > 0) {
        // Echo the input on the serial interface
        Serial.print("# ");
        Serial.println(server.arg("cmd"));

        // Parse input
        cli.parse(server.arg("cmd"));

        // Check for commands
        if (cli.available()) {
            Command cmd = cli.getCommand();

            if (cmd == cmdLed) {
                String mode = cmd.getArgument("mode").getValue();

                if (mode == "blue") {
                  answer = "why so blue?";
                  digitalWrite(D1, HIGH);
                  digitalWrite(D2, LOW);
                  digitalWrite(D3, LOW);
                  ++level;
                  createBird();
                }else if (mode == "red") {
                  answer = "seeing red?";
                  digitalWrite(D1, LOW);
                  digitalWrite(D2, HIGH);
                  digitalWrite(D3, LOW);
                  ++level;
                  createBird();
                }
                else if(mode == "green") {
                  answer = "the others must be green with envy";
                  digitalWrite(D1, LOW);
                  digitalWrite(D2, LOW);
                  digitalWrite(D3, HIGH);
                  ++level;
                  createBird();
                }
            }
        }

        // Check for errors
        if (cli.errored()) {
            answer = cli.getError().toString();
        }
    }

    // Build HTML string
    String html =
        "<!DOCYTPE html>"
        "<html>"
        "<head>"
          "<meta charset=\"utf-8\">"
          "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
          "<title>LAY YOUR EGG</title>"
        "</head>"
        "<body style=\"display:flex; flex-direction:column;justify-content:center;align-items:center;height:100%; background-color:black;\">"
          "<style>"
            "a.button{display:inline-block;padding:.7em 1.4em;margin:0 .3em .3em 0;border-radius:.15em;box-sizing:border-box;text-decoration:none;font-family:Roboto,sans-serif;text-transform:uppercase;font-weight:400;color:#fff;width:30%;box-shadow:inset 0 -.6em 0 -.35em rgba(0,0,0,.17);text-align:center;position:relative}a.button:active{top:.1em}@media all and (max-width:500px){a.button{display:block;width:50%;margin:.4em auto}}"
          "</style>"
          "<a href=\"index.html?cmd=led+red\" class=\"button\" style=\"background-color:#f8333c\">Red Team</a>"
          "<a href=\"index.html?cmd=led+green\" class=\"button\" style=\"background-color:#44af69\">Green Team</a>"
          "<a href=\"index.html?cmd=led+blue\" class=\"button\" style=\"background-color:#3369ff\">Blue Team</a>"
          "<p>";

          html += answer;
          html +=
          "</p>"
        "</body>"
        "</html>";

    // Send HTML to user
    server.send(200, "text/html", html.c_str());
}

void handleNotFound() {
    String message = "File Not Found\n\n";

    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";

    for (uint8_t i = 0; i < server.args(); i++) {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
}

void setup(void) {
    Serial.begin(115200);
    createBird();

    pinMode(D1, OUTPUT);
    pinMode(D2, OUTPUT);
    pinMode(D3, OUTPUT);

    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    server.on("/", handleRoot);
    server.begin();
    Serial.println("HTTP server started");

    if (MDNS.begin("esp8266")) {
        Serial.println("MDNS responder started");
    }

    server.on("/", handleRoot);
    server.on("/index.html", handleRoot);

    server.on("/inline", []() {
        server.send(200, "text/plain", "this works as well");
    });

    server.onNotFound(handleNotFound);

    cmdLed = cli.addCommand("led");
    cmdLed.addPositionalArgument("mode", "on");

    server.begin();
    Serial.println("HTTP server started");
}

void loop(void) {
    server.handleClient();
    MDNS.update();
}
