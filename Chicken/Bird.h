#ifndef BIRD_H
#define BIRD_H

// ========== Includes ========== //
#include <Arduino.h>     // String Class
#include <ESP8266WiFi.h> // Scan WiFi
#include <EEPROM.h>      // Save stats

#include "types.h"       // TEAM, LEVEL enums

// From config.h
extern const unsigned int NUM_PASSWORDS;
extern const char* SUPER_SECRET PROGMEM;
extern const unsigned int POINTS_PER_SECOND[];
extern const unsigned int POINT_INTERVAL;
extern const bool HIDDEN_SSID;
extern const unsigned int MAX_CONNECTIONS;
extern const unsigned int MAX_CHANNEL;
extern const char* DIFFICULTY[] PROGMEM;
extern const char* SSID_PREFIX PROGMEM;
extern const char* SSID_SUFFIX[] PROGMEM;
extern const char* EASY_PSWD[] PROGMEM;
extern const char* MEDIUM_PSWD[] PROGMEM;
extern const char* HARD_PSWD[];

// ========== Game Stats ========== //
#define MAGIC_NUM 123456789
#define STATS_ADDR 1

typedef struct game_stats {
    unsigned int magic_num; // Used to safely read and write to EEPROM

    unsigned int id;        // Random generated ID
    LEVEL        level;     // 0: Easy, 1: Medium, 2: Hard, 3: Locked
    TEAM         flag;      // 0: Red, 1: Green, 3: Blue

    unsigned int points[3]; // Red-Points, Green-Points, Blue-Points

    char ssid[33];          // SSID of access point
    char pswd[65];          // Password of access point
} game_stats;

// ========== Bird Class ========== //

class Bird {
    private:
        game_stats stats;

        unsigned long lastPointUpdate = 0; // Last time the team points were updated

        bool error = false;

        String getSSID();
        String getPassword();
        int getChannel();

        void createID();
        void createAP();

        void updatePoints();

    public:
        Bird();

        void begin();
        void update();

        int getConnections();

        void setFlag(TEAM flag);

        int getPoints(TEAM team);
        String getPointsString(bool reset = true);

        LEVEL getLevel();
        TEAM getFlag();

        bool resetGame(String password);

        bool errored();
};


#endif /* ifndef BIRD_H */