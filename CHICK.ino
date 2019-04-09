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













int level = 0; 

const char* easyPass[] PROGMEM = {
"password\0",
"123456789\0",
"12345678\0",
"1q2w3e4r\0",
"sunshine\0",
"football\0",
"1234567890\0",
"computer\0",
"superman\0",
"internet\0",
"iloveyou\0",
"1qaz2wsx\0",
"baseball\0",
"whatever\0",
"princess\0"};

const char* mediumPass[] PROGMEM = {
"sweetheart\0",
"overlord\0",
"michaela\0",
"meredith\0",
"buttercup\0",
"abc12345\0",
"aardvark\0",
"Passw0rd\0",
"12345678910\0",
"universal\0",
"trinidad\0",
"thursday\0",
"standard\0",
"pearljam\0",
"anonymous\0",};

const char* hardPass[] PROGMEM = {
"prairie chicken\0",
"chicken-breasted\0",
"chicken broth\0",
"chicken cholera\0",
"chickenpox\0",
"chicken pox\0",
"chicken roost\0",
"Chicken scratch\0",
"chicken septicemia\0",
"CHICKEN SHACK\0",
"chicken snake\0",
"chickensoup\0",
"chicken-spirited\0",
"chickens-toes\0",
"chicken thief\0",};

const char* numberAdd[] PROGMEM = {
"One\0",
"Two\0",
"Three\0",
"Four\0",
"Five\0",
"Six\0",
"Seven\0",
"Eight\0",
"Nine\0",
"Ten\0",
"Eleven\0",
"Twelve\0",
"Thirteen\0",
"Fourteen\0",
"Fifteen\0"};


void createBird(){
  int randNumber = random(0, 14);
  Serial.print("Random Number:");
  Serial.println(randNumber);
  char ChickenNumber = char(randNumber);
  const char* password = easyPass[randNumber];
  const char* ssid = numberAdd[randNumber];
  Serial.print("Creating the bird with SSID ");Serial.print(ssid);Serial.print(" and password ");Serial.println(password);

  char buff[50];
  const char s0[] = "Chicken_Easy_";
  const char s1[] = "Chicken_Medium_";
  const char s2[] = "Chicken_Hard_";
  int i = 0;

  
   switch(level){

    
    case 0:
    Serial.println("Setting soft-AP:");
    // concatenate s0 and ssid
    for(int j = 0; s0[j]; ++j, ++i)
      buff[i]=s0[j];
    for(int j = 0; ssid[j]; ++j, ++i)
      buff[i]=ssid[j];
    buff[i]='\0';
    Serial.println(buff);
    Serial.println(WiFi.softAP(buff, password) ? "Ready" : "Failed!");
    break;

    
    case 1:
    Serial.println("Setting soft-AP:");
    // concatenate s1 and ssid
    for(int j = 0; s1[j]; ++j, ++i)
      buff[i]=s1[j];
    for(int j = 0; ssid[j]; ++j, ++i)
      buff[i]=ssid[j];
    buff[i]='\0';
    Serial.println(buff);
    Serial.println(WiFi.softAP(buff, password) ? "Ready" : "Failed!");
    break;

    
    case 2:
    Serial.println("Setting soft-AP:");
    // concatenate s2 and ssid
    for(int j = 0; s2[j]; ++j, ++i)
      buff[i]=s2[j];
    for(int j = 0; ssid[j]; ++j, ++i)
      buff[i]=ssid[j];
    buff[i]='\0';
    Serial.println(buff);
    Serial.println(WiFi.softAP(buff, password) ? "Ready" : "Failed!");
    break;    
    default:
    // statements
    break;
  }
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

                if (mode == "on") {
                    pinMode(2, HIGH);
                    answer = "turned LED on";
                } else if (mode == "off") {
                    pinMode(2, LOW);
                    answer = "turned LED off";
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
        "<title>SimpleCLI</title>"
        "</head>"
        "<body>"
        "<p>";

    html += answer;

    html +=
        "</p>"
        "<form action=\"index.html\" method=\"GET\">"
        "<input type=\"text\" name=\"cmd\"></input>"
        "<input type=\"submit\"></input>"
        "</form>"
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
