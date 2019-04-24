// ========== Includes ========== //
#include "Bird.h"

// ========= Private ======== //

// Calculates points per team
void Bird::updatePoints() {
    // Only update every givin interval (i.e. one second)
    if (millis() - lastPointUpdate >= POINT_INTERVAL * 1000) {
        lastPointUpdate = millis(); // Update time variable

        // Add configured amount of points
        if (stats.level < LOCKED) {
            stats.points[stats.flag] += POINTS_PER_SECOND[stats.level];
        } else {
            stats.points[stats.flag] += POINTS_PER_SECOND[HARD];
        }

        saveStats();
    }
}

// Save game stats in EEPROM
void Bird::saveStats() {
    EEPROMHelper::saveObject(EEPROM_SIZE, EEPROM_STATS_ADDR, stats);

    /*
       EEPROM.begin(4095); // Start EEPROM

       // Write game stats
       EEPROM.put(EEPROM_STATS_ADDR, stats);
       EEPROM.commit();

       EEPROM.end(); // End EEPROM
     */
}

// Recover game stats from EEPROM
bool Bird::recoverStats() {
    game_stats tmpStats;

    EEPROMHelper::getObject(EEPROM_SIZE, EEPROM_STATS_ADDR, tmpStats);

    /*
       EEPROM.begin(4095); // Start EEPROM

       // Read game stats from memory
       game_stats tmpStats;
       EEPROM.get(EEPROM_STATS_ADDR, tmpStats);

       EEPROM.end(); // End EEPROM
     */
    // Check if memory valid
    if (tmpStats.magic_num == GAME_MAGIC_NUM) {
        stats = tmpStats; // Set game stats
        return true;      // Success
    }

    return false;         // Failed
}

// Creates a new access point based on ID and level
void Bird::createAP() {
    // Disconnect clients
    WiFi.softAPdisconnect();

    // Go into station mode
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    // Create SSID and password
    String ssid     = getSSID();
    String password = getPassword();

    // Copy SSID and password to the stats (so it can be saved and recovered)
    strncpy(stats.ssid, ssid.c_str(), 32);
    strncpy(stats.pswd, password.c_str(), 64);

    // Go into AP mode
    WiFi.mode(WIFI_AP);

    // Set IP Adress, Gateway and Subnetmask
    WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0));

    // Start access point
    WiFi.softAP(stats.ssid, stats.pswd, getChannel(), HIDDEN_SSID && stats.level == 2, MAX_CONNECTIONS);

    // Print infos
    Serial.println("--------------------------------------------");
    Serial.printf("Flag:          %d\n", getFlag());
    Serial.printf("Chicken Level: %s\n", DIFFICULTY[stats.level]);
    Serial.printf("Chicken ID:    %d\n", stats.id);
    Serial.printf("Channel:       %d\n", getChannel());
    Serial.printf("MAC-Address:   %s\n", WiFi.softAPmacAddress().c_str());
    Serial.printf("IP-Address:    %s\n", WiFi.softAPIP().toString().c_str());
    Serial.printf("SSID:          %s\n", stats.ssid);
    Serial.printf("Password:      %s\n", stats.pswd);
    Serial.println("---------------------------------------------");
}

// Generates a random ID and checks that it's not already in use
void Bird::createID() {
    stats.id = random(0, NUM_PASSWORDS); // Pick a random ID to start with

    // Go into station mode
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    // Scan for networks
    Serial.println("Scanning for networks");

    int networks       = WiFi.scanNetworks();
    unsigned int tries = 0;

    // Check all networks
    for (int i = 0; i < networks && tries < NUM_PASSWORDS; i++) {
        // Find a network that's part of the game
        String ssid = WiFi.SSID(i);

        if (ssid.startsWith(SSID_PREFIX)) {
            // If network has the same ID, take next one
            if (ssid.endsWith(SSID_SUFFIX[stats.id])) {
                stats.id = (stats.id+1) % NUM_PASSWORDS;
                ++tries;
            }
        }

        // Print network
        Serial.printf("%d: '%s' (%d) %s\n", i, WiFi.SSID(i).c_str(), WiFi.RSSI(i), (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
    }

    // No ID found that isn't already in use
    if (tries >= NUM_PASSWORDS) {
        stats.id = 0;
        error    = true;
        Serial.println("ERROR: All IDs already in use");
    }

    // Everything is fine :D
    else {
        Serial.printf("ID set to %d\n", stats.id);
    }
}

// ========= Public ========= //

// Bird object constructor
Bird::Bird(int id, LEVEL level) {
    // Set game stats to default
    stats.magic_num = GAME_MAGIC_NUM;
    stats.id        = id;
    stats.level     = level;
    stats.flag      = NONE;

    for (int i = 0; i<3; i++) stats.points[i] = 0;

    for (int i = 0; i<33; i++) stats.ssid[i] = '\0';

    for (int i = 0; i<65; i++) stats.pswd[i] = '\0';
}

// Starts game piece
void Bird::begin() {
    // Try to recover previous game stats
    if (recoverStats()) {
        Serial.println("Revived bird");
    }
    // Otherwise generate new random ID (= new SSID and password too)
    else {
        Serial.println("Creating new bird...");
        createID();
    }

    // Create access point
    createAP();
}

// Updates game piece
void Bird::update() {
    updatePoints();
}

// Builds SSID (i.e. "Chicken_" + "easy" + "_01"), depending on level and ID
String Bird::getSSID() const {
    return String(SSID_PREFIX) + String(DIFFICULTY[stats.level]) + String(SSID_SUFFIX[stats.id]);
}

// Returns password, depending on level and ID
String Bird::getPassword() const {
    if (stats.level == 3) return String(SUPER_SECRET);

    const char** pswdList[] = { EASY_PSWD, MEDIUM_PSWD, HARD_PSWD };
    return String(pswdList[stats.level][stats.id]);
}

// Returns channel, deoending on ID and MAX_CHANNEL
int Bird::getChannel() const {
    return (stats.id % MAX_CHANNEL) + 1;
}

// Returns number of connected clients
int Bird::getConnections() const {
    return WiFi.softAPgetStationNum();
}

// Returns the number of points from a given team
int Bird::getPoints(TEAM team) const {
    return (team == NONE) ? 0 : stats.points[team];
}

// Returns string with the points of all teams i.e. "12,0,58"
String Bird::getPointsString() const {
    return String(stats.points[0]) + ',' + String(stats.points[1]) + ',' + String(stats.points[2]);
}

String Bird::getPointsString(bool reset) {
    String pointsStr = getPointsString();

    if (reset) {
        stats.points[0] = 0;
        stats.points[1] = 0;
        stats.points[2] = 0;
    }

    return pointsStr;
}

// Returns current level/difficulty of this game piece
LEVEL Bird::getLevel() const {
    return stats.level;
}

// Returns the team that is holding this game piece/flag
TEAM Bird::getFlag() const {
    return stats.flag;
}

// Checks for errors
bool Bird::errored() const {
    return error;
}

// Sets a new flag and restarts the access point
void Bird::setFlag(TEAM flag) {
    stats.flag = flag;

    switch (stats.level) {
        case EASY:
            stats.level = MEDIUM;
            break;
        case MEDIUM:
            stats.level = HARD;
            break;
        case HARD:
            stats.level = LOCKED;
            break;
        case LOCKED:
            break;
    }

    createAP();
}

// Resets game stats
bool Bird::resetGame(String password) {
    Serial.print("Resetting Game...");

    // Check if the password is correct
    if (password == SUPER_SECRET) {
        // All points to 0
        for (int i = 0; i < 3; i++) stats.points[i] = 0;

        // Beginner level with no team holding it
        stats.level = EASY;
        stats.flag  = NONE;

        // New ID with new SSID and password
        createID();
        createAP();

        Serial.println("Done");

        return true;
    }

    Serial.println("Wrong Password");

    return false;
}