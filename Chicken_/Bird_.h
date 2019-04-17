#ifndef BIRD_H
#define BIRD_H

// ========== Includes ========== //
#include <Arduino.h>     // String
#include <ESP8266WiFi.h> // WiFi

// From config.h
extern const unsigned int NUM_PASSWORDS;
extern const char* SUPER_SECRET PROGMEM;
extern const unsigned int POINTS_PER_SECOND[];
extern const unsigned int POINT_INTERVAL;
extern const char* SSID_PREFIX[] PROGMEM;
extern const char* SSID_SUFFIX[] PROGMEM;
extern const char* EASY_PSWD[] PROGMEM;
extern const char* MEDIUM_PSWD[] PROGMEM;
extern const char* HARD_PSWD[];

// ========== Bird Class ========== //

class Bird {
    private:
        String ssid;                          // SSID of access point
        String pswd;                          // Password of access point

        unsigned long lastPointUpdate = 0;    // Last time the team points were updated

        unsigned int id    = 0;               // Random generated ID
        unsigned int level = 0;               // 0: Easy, 1: Medium, 2: Hard, 3: Locked
        unsigned int flag  = 1;               // 0: Red, 1: Green, 3: Blue

        unsigned int points[3] = { 0, 0, 0 }; // Red-Points, Green-Points, Blue-Points

        String getSSID();

        String getPassword();

        void createAP();

        void updatePoints();

    public:
        void begin();
        void update();

        int getConnections();

        void setFlag(unsigned int flag);

        int getPoints(unsigned int team);
        String getPointsString();

        int getLevel();
        int getFlag();

        bool reset(String password);
};


#endif /* ifndef BIRD_H */