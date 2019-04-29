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
    EEPROMHelper::saveObject(EEPROM_STATS_ADDR, stats);
}

// Recover game stats from EEPROM
bool Bird::recoverStats() {
    game_stats tmpStats;

    EEPROMHelper::getObject(EEPROM_STATS_ADDR, tmpStats);

    // Check if memory valid
    if ((tmpStats.magic_num == GAME_MAGIC_NUM) && (tmpStats.mode == CHICKEN)) {
        stats = tmpStats; // Set game stats
        return true;      // Success
    }

    return false;         // Failed
}

// Creates a new access point based on ID and level
void Bird::createAP(bool firstStart) {
    // Save game stats, in case something goes wrong
    saveStats();

    delay(100);

    // Try 5 times to close AP and disconnect all clients
    if (!firstStart) {
        bool success = false;

        for (int i = 0; i<5 && !success; i++) {
            success = WiFi.softAPdisconnect(true);
            Serial.printf("Closing existing access point (%d)...%s\n", i, success ? "OK" : "ERROR");
            delay(100);
        }

        if (!success) {
            error = true;
            return;
        }
    }

    // Create SSID and password
    String ssid     = getSSID();
    String password = getPassword();

    // Copy SSID and password to the stats (so it can be saved and recovered)
    strncpy(stats.ssid, ssid.c_str(), 32);
    strncpy(stats.pswd, password.c_str(), 64);

    // Go into AP mode
    WiFi.mode(WIFI_AP);

    delay(100);

    // Generate and set MAC address (BSSID) of access point
    uint8_t mac[6];
    getMacAddress(mac);
    wifi_set_macaddr(SOFTAP_IF, mac);

    // Start access point
    WiFi.softAP(stats.ssid, stats.pswd, getChannel(), HIDDEN_SSID && stats.level == 2, MAX_CONNECTIONS);

    delay(100);

    // Set IP Adress, Gateway and Subnetmask
    WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0));

    delay(100);

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

    delay(100);
}

// Pick a random ID
void Bird::createID() {
    setID(random(0, NUM_PASSWORDS));
}

// Set ID and make sure it's not already in use
void Bird::setID(unsigned int id) {
    stats.id = id;

    // Go into station mode
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    Serial.println("Scanning for networks");

    // Scan for networks (async=false, show-hidden=true)
    int n = WiFi.scanNetworks(false, true);

    // Array of available IDs
    bool availableIDs[NUM_PASSWORDS];

    // Set all IDs to available by default
    for (unsigned int i = 0; i<NUM_PASSWORDS; i++) availableIDs[i] = true;

    // Check all networks
    for (int i = 0; i<n; i++) {
        // Find a network that's part of the game
        uint8_t* bssid = WiFi.BSSID(i);
        bool     chick = (bssid[0] == 0x18) && (bssid[1] == 0xFE) &&
                         (bssid[2] == 0x34) && (bssid[3] == 0x00) &&
                         (bssid[4] <= (int)LEVEL::LOCKED) &&
                         (bssid[5] <= NUM_PASSWORDS);

        // Make used ID unavailble
        if (chick) {
            int chickenID = bssid[5];
            availableIDs[chickenID] = false;
        }

        String ssid = WiFi.isHidden(i) ? "*Hidden*" : WiFi.SSID(i);

        // Print network
        Serial.printf("%-32s - %s\n", ssid.c_str(), chick ? "Chicken" : "WORTHLESS!");
    }

    // If chosen ID is not available, take next best one that is available
    if (!availableIDs[stats.id]) {
        for (unsigned int i = 0; i<NUM_PASSWORDS; i++) {
            if (availableIDs[i]) {
                stats.id = i;
                Serial.printf("ID set to %d\n", stats.id);
                return;
            }
        }

        // No ID found that isn't already in use
        stats.id = 0;
        error    = true;
        Serial.println("ERROR: All IDs already in use");
    }

    Serial.printf("ID set to %d\n", stats.id);
}

// ========= Public ========= //

// Bird object constructor
Bird::Bird(int id, LEVEL level) {
    // Set game stats to default
    stats.magic_num = GAME_MAGIC_NUM;

    stats.mode = CHICKEN;

    stats.id = id;

    stats.level = level;

    stats.flag = NO_TEAM;

    for (int i = 0; i<3; i++) stats.points[i] = 0;

    for (int i = 0; i<33; i++) stats.ssid[i] = '\0';

    for (int i = 0; i<65; i++) stats.pswd[i] = '\0';
}

// Starts game piece
void Bird::begin() {
    // Try to recover previous game stats
    if (recoverStats()) {
        Serial.println("Revived bird");
        setID(stats.id);
    }
    // Otherwise generate new random ID (= new SSID and password too)
    else {
        Serial.println("Creating new bird...");
        createID();
    }

    // Create access point
    createAP(true);
}

// Updates game piece
void Bird::update() {
    updatePoints();

    if ((flagTime > 0) && (millis() - flagTime >= 2000)) {
        createAP();
        flagTime = 0;
    }
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

// Returns channel, depending on ID and MAX_CHANNEL
int Bird::getChannel() const {
    return (stats.id % MAX_CHANNEL) + 1;
}

// Returns MAC Addrss, depending on ID and MAX_CHANNEL
void Bird::getMacAddress(uint8_t* ptr) const {
    ptr[0] = 0x18;                 // ESP
    ptr[1] = 0xFE;                 // ESP
    ptr[2] = 0x34;                 // ESP
    ptr[3] = 0x00;                 // CHICKEN
    ptr[4] = (uint8_t)stats.level; // LEVEL
    ptr[5] = (uint8_t)stats.id;    // ID
}

// Returns number of connected clients
int Bird::getConnections() const {
    return WiFi.softAPgetStationNum();
}

// Returns the number of points from a given team
int Bird::getPoints(TEAM team) const {
    return (team == NO_TEAM) ? 0 : stats.points[team];
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
        saveStats();
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

    if (stats.level < LOCKED) {
        stats.level = (LEVEL)((int)stats.level + 1);
        flagTime    = millis();
    }

    saveStats();
}

// Resets game stats
bool Bird::resetGame(String password) {
    // Check if the password is correct
    if (password == SUPER_SECRET) {
        // All points to 0
        for (int i = 0; i < 3; i++) stats.points[i] = 0;

        // Beginner level with no team holding it
        stats.level = EASY;
        stats.flag  = NO_TEAM;

        // New ID with new SSID and password
        createID();
        createAP();

        saveStats();

        return true;
    }

    return false;
}