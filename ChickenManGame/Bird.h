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
extern const int EEPROM_STATS_ADDR;

// For wifi_set_macaddr()
extern "C" {
  #include "user_interface.h"
}

// ========== Bird Class ========== //

class Bird {
    private:
        game_stats stats;

        unsigned long lastPointUpdate = 0; // Last time the team points were updated

        bool error = false;

        unsigned long flagTime = 0;

        void updatePoints();

        void saveStats();
        bool recoverStats();

        void createID();
        void setID(unsigned int id);
        void createAP();

    public:
        Bird(int id = 0, LEVEL level = EASY);

        void begin();
        void update();

        // Getter
        String getSSID() const;
        String getPassword() const;
        int getChannel() const;
        void getMacAddress(uint8_t* ptr) const;

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