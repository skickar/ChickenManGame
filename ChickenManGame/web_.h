#pragma once

// [ESP8266 Libraries]
 #include <ESP8266WebServer.h>
 #include <ESP8266mDNS.h>

// From Chicken.ino
extern String handleCLI(String input);

// From config.h
extern const char* HTML_PREFIX PROGMEM;
extern const char* HTML_SUFFIX PROGMEM;

namespace web {
    void begin();
    void update();
};