#ifndef BIRD_H
#define BIRD_H
// ========== Includes ========== //
#include <Arduino.h>     // String Class
#include <ESP8266WiFi.h> // Scan WiFi

#include "EEPROMHelper.h"
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

// From hardware.h
extern const int EEPROM_SIZE;
extern const int EEPROM_STATS_ADDR;

// ========== Game Stats ========== //

// Used to verify memory of game_stats objects
#define GAME_MAGIC_NUM 1234567890

typedef struct game_stats {
    unsigned int magic_num : 32; // Used to safely read and write to EEPROM

    unsigned int id : 8;         // Random generated ID

    LEVEL level;                 // 0: Easy, 1: Medium, 2: Hard, 3: Locked
    TEAM  flag;                  // 0: Red, 1: Green, 3: Blue

    unsigned int points[3];      // Red-Points, Green-Points, Blue-Points

    char ssid[33];               // SSID of access point
    char pswd[65];               // Password of access point
} game_stats;

// ========== Bird Class ========== //

class Bird {
    private:
        game_stats stats;

        unsigned long lastPointUpdate = 0; // Last time the team points were updated

        bool error = false;

        void updatePoints();

        void saveStats();
        bool recoverStats();

        void createID();
        void createAP();

    public:
        Bird(int id = 0, LEVEL level = EASY);

        void begin();
        void update();

        // Getter
        String getSSID() const;
        String getPassword() const;
        int getChannel() const;

        int getConnections() const;

        int getPoints(TEAM team) const;
        String getPointsString() const;
        String getPointsString(bool reset);

        LEVEL getLevel() const;
        TEAM getFlag() const;

        bool errored() const;

        // Setter
        void setFlag(TEAM flag);

        bool resetGame(String password);
};


#endif /* ifndef BIRD_H */