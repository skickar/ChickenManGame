// ========== Includes ========== //
#include "Man.h"

// ========= Private ======== //

// Save game stats in EEPROM
void Man::saveStats() {
    EEPROMHelper::saveObject(EEPROM_STATS_ADDR, stats);
}

// Recover game stats from EEPROM
bool Man::recoverStats() {
    game_stats tmpStats;

    EEPROMHelper::getObject(EEPROM_STATS_ADDR, tmpStats);

    // Check if memory valid
    if ((tmpStats.magic_num == GAME_MAGIC_NUM) && (tmpStats.mode == CHICKEN_MAN)) {
        stats = tmpStats; // Set game stats
        return true;      // Success
    }

    return false;         // Failed
}

bool Man::isAChicken(uint8_t* bssid) const {
    return (bssid[0] == 0x18) && (bssid[1] == 0xFE) &&
           (bssid[2] == 0x34) && (bssid[3] == 0x00) &&
           (bssid[4] <= (int)LEVEL::LOCKED) &&
           (bssid[5] <= NUM_PASSWORDS);
}

bool Man::isAChicken(String ssid, uint8_t* bssid) const {
    if (!isAChicken(bssid)) return false;

    unsigned int level = bssid[4];
    unsigned int id    = bssid[5];

    String prefix(SSID_PREFIX);
    String difficulty(DIFFICULTY[level]);
    String suffix(SSID_SUFFIX[id]);

    int prefixPos     = ssid.indexOf(prefix);
    int difficultyPos = ssid.indexOf(difficulty);
    int suffixPos     = ssid.indexOf(suffix);

    bool ssidValid = (prefixPos == 0) && (difficultyPos > 0) && (suffixPos > 0) &&
                     ((unsigned int)difficultyPos == prefix.length()) &&
                     ((unsigned int)suffixPos == (unsigned int)difficultyPos + difficulty.length());

    return ssidValid;
}

String Man::getPassword(uint8_t* bssid) const {
    LEVEL level     = (LEVEL)bssid[4];
    unsigned int id = bssid[5];

    if (id <= NUM_PASSWORDS) {
        switch (level) {
            case EASY:
                return String(EASY_PSWD[id]);
            case MEDIUM:
                return String(MEDIUM_PSWD[id]);
            case HARD:
                return String(HARD_PSWD[id]);
            case LOCKED:
                return String(SUPER_SECRET);
        }
    }

    return String();
}

String Man::getSSID(uint8_t* bssid) const {
    if ((bssid[4] <= (int)LEVEL::LOCKED) && (bssid[5] < NUM_PASSWORDS)) {
        LEVEL level     = (LEVEL)bssid[4];
        unsigned int id = bssid[5];

        return String(SSID_PREFIX) + String(DIFFICULTY[level]) + String(SSID_SUFFIX[id]);
    }
    return String("*Hidden Network*");
}

void Man::addScore(String payload) {
    String rStr;
    String gStr;
    String bStr;

    int comma = 0;

    for (unsigned int i = 0; i<payload.length(); i++) {
        char c = payload.charAt(i);

        if (c == ',') {
            ++comma;
        } else {
            switch (comma) {
                case 0:
                    rStr += c;
                    break;
                case 1:
                    gStr += c;
                    break;
                case 2:
                    bStr += c;
                    break;
            }
        }
    }

    stats.points[0] += rStr.toInt();
    stats.points[1] += gStr.toInt();
    stats.points[2] += bStr.toInt();

    if (stats.points[0] > stats.points[(int)getFlag()]) stats.flag = RED;
    else if (stats.points[1] > stats.points[(int)getFlag()]) stats.flag = GREEN;
    else if (stats.points[2] > stats.points[(int)getFlag()]) stats.flag = BLUE;

    saveStats();
}

// ========= Public ========= //

// Man object constructor
Man::Man() {
    // Set game stats to default
    stats.magic_num = GAME_MAGIC_NUM;

    stats.mode = CHICKEN_MAN;

    stats.id = 0;

    stats.level = EASY;

    stats.flag = NO_TEAM;

    for (int i = 0; i<3; i++) stats.points[i] = 0;

    for (int i = 0; i<33; i++) stats.ssid[i] = '\0';

    for (int i = 0; i<65; i++) stats.pswd[i] = '\0';
}

// Starts Chicken Man
void Man::begin() {
    // Try to recover previous game stats
    if (recoverStats()) {
        Serial.println("Revived Chicken Man");
    } else {
        Serial.println("Created new Chicken Man");
    }

    // Delay to give the user a chance to hit reset
    delay(2000);
}

// Updates Chicken Man
void Man::update() {
    Serial.println("---------------------------------------------------------------");
    Serial.println("The chicken man has awoken!");
    Serial.print("Scanning for Networks...");

    // scan for networks (async=false, show-hidden=true)
    int n = WiFi.scanNetworks(false, true);

    Serial.println(n);
    Serial.println("---------------------------------------------------------------");

    // iterate through networks, connecting to chickens to get point data
    // this will also generate handshakes
    for (int i = 0; i < n; i++) {
        uint8_t* bssid    = WiFi.BSSID(i);
        String   ssid     = WiFi.isHidden(i) ? getSSID(bssid) : WiFi.SSID(i);
        String   password = getPassword(bssid);
        bool     chick    = isAChicken(ssid, bssid);

        Serial.printf("%-32s - %s\n", ssid.c_str(), chick ? "Chicken" : "WORTHLESS!");

        if (!chick) continue;  // Next network

        Serial.printf("-> Connecting to '%s' with password '%s'...", ssid.c_str(), password.c_str());

        WiFi.begin(ssid.c_str(), password.c_str());
        delay(100);
        WiFiClient client;
        delay(100);

        // Try to connect 5 times
        for (int j = 0; j<5; j++) {
            Serial.printf("Connecting [%d]...", j+1);

            // Try to connect for 2s
            unsigned long startTime = millis();

            while (WiFi.status() != WL_CONNECTED && millis() - startTime < 5000) {
                delay(50);
            }

            if (WiFi.status() == WL_CONNECTED) {
                Serial.println("SUCCESS \\o/");
                break;
            } else {
                Serial.println("no luck :(");
                WiFi.disconnect();
            }
        }

        // Open URL to get points
        http.begin(client, "http://192.168.4.1/points.html");

        int httpCode   = http.GET();
        String payload = http.getString();

        if (httpCode > 0) {
            Serial.printf("(%s)\n", payload.c_str());
            addScore(payload);
        } else {
            Serial.printf("ERROR %d\n", httpCode);
        }

        WiFi.disconnect();
    }
    Serial.println("---------------------------------------------------------------");
    Serial.printf("Updated Score: red = %d, green = %d, blue = %d\n", stats.points[0], stats.points[1], stats.points[2]);
}

int Man::getPoints(TEAM team) const {
    if (team == NO_TEAM) return 0;
    return stats.points[(int)team];
}

String Man::getPointsString() const {
    return String(stats.points[0]) + ',' + String(stats.points[1]) + ',' + String(stats.points[2]);
}

TEAM Man::getFlag() const {
    return stats.flag;
}

// Resets game stats
bool Man::resetGame(String password) {
    // Check if the password is correct
    if (password == SUPER_SECRET) {
        // All points to 0
        for (int i = 0; i < 3; i++) stats.points[i] = 0;

        // No team holding the flag
        stats.flag = NO_TEAM;

        saveStats();

        return true;
    }

    return false;
}
