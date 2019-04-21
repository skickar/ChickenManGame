#ifndef WEB_H
#define WEB_H

// ========== Includes ========== //

// [ESP8266 Libraries]
 #include <ESP8266WebServer.h>
 #include <ESP8266mDNS.h>

// From Chicken.ino
extern String handleCLI(String input);

// From config.h
extern const char* HTML_PREFIX PROGMEM;
extern const char* HTML_SUFFIX PROGMEM;

// ========== Web Class ========== //

class Web {
    public:
        void begin();
        void update();
};

#endif /* ifndef WEB_H */