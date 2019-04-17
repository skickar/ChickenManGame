// ========== Includes ========== //
#include "Bird.h"

// ========= Private ======== //

String Bird::getSSID() {
    return String(SSID_PREFIX) + String(DIFFICULTY[stats.level]) + String(SSID_SUFFIX[stats.id]);
}

String Bird::getPassword() {
    if (stats.level == 3) return String(SUPER_SECRET);

    const char** pswdList[] = { EASY_PSWD, MEDIUM_PSWD, HARD_PSWD };
    return String(pswdList[stats.level][stats.id]);
}

int Bird::getChannel() {
    return (stats.id % MAX_CHANNEL) + 1;
}

void Bird::createAP() {
    // Disconnect clients
    WiFi.softAPdisconnect();

    // Set IP Adress
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0));

    // Create SSID and password
    String ssid     = getSSID();
    String password = getPassword();

    strncpy(stats.ssid, ssid.c_str(), 32);
    strncpy(stats.pswd, password.c_str(), 64);

    // Start access point
    WiFi.softAP(stats.ssid, stats.pswd, getChannel(), HIDDEN_SSID && stats.level == 2, MAX_CONNECTIONS);

    // Print infos
    Serial.println("--------------------------------------------");
    Serial.printf("Chicken Level: %d\n", stats.level);
    Serial.printf("Chicken ID:    %d\n", stats.id);
    Serial.printf("Channel:       %d\n", getChannel());
    Serial.printf("MAC-Address:   %s\n", WiFi.softAPmacAddress().c_str());
    Serial.printf("IP-Address:    %s\n", WiFi.softAPIP().toString().c_str());
    Serial.printf("SSID:          %s\n", stats.ssid);
    Serial.printf("Password:      %s\n", stats.pswd);
    Serial.println("---------------------------------------------");
}

// Team-Point-Calculator
void Bird::updatePoints() {
    if (millis() - lastPointUpdate >= POINT_INTERVAL * 1000) {
        lastPointUpdate = millis();

        if (stats.level > 3) stats.level = 3;
        if (stats.flag > 2) stats.flag = 1;

        if (stats.level != 3) { // Not locked
            stats.points[stats.flag] += POINTS_PER_SECOND[stats.level];
        }

        // Save stats
        EEPROM.begin(4095);
        EEPROM.put(STATS_ADDR, stats);
        EEPROM.commit();
        EEPROM.end();
    }
}

void Bird::createID() {
    stats.id = random(0, NUM_PASSWORDS);

    int s = random(1, 5);

    Serial.printf("Delay for %d seconds\n", s);

    // random delay to lower the risk of game pieces that start at the same time
    delay(s*1000);

    Serial.println("Scanning for networks");

    // Go into station mode
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    // Scan for networks
    int networks       = WiFi.scanNetworks();
    unsigned int tries = 0;

    // Find a network that's part of the game
    for (int i = 0; i < networks && tries < NUM_PASSWORDS; i++) {
        String ssid = WiFi.SSID(i);

        if (ssid.startsWith(SSID_PREFIX)) {
            // If found network has the same ID, take next one
            if (ssid.endsWith(SSID_SUFFIX[stats.id])) {
                stats.id = (stats.id+1) % NUM_PASSWORDS;
                ++tries;
            }
        }

        Serial.printf("%d: '%s' (%d) %s\n", i, WiFi.SSID(i).c_str(), WiFi.RSSI(i), (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
    }

    if (tries >= NUM_PASSWORDS) {
        stats.id = 0;
        error    = true;
        Serial.println("ERROR: All IDs already in use");
    } else {
        Serial.printf("ID set to %d\n", stats.id);
    }
}

// ========= Public ========= //
Bird::Bird() {
    stats.magic_num = MAGIC_NUM;

    stats.id    = 0;
    stats.level = 0;
    stats.flag  = 1;

    for (int i = 0; i<3; i++) stats.points[i] = 0;

    for (int i = 0; i<33; i++) stats.ssid[i] = '\0';

    for (int i = 0; i<65; i++) stats.pswd[i] = '\0';
}

void Bird::begin() {
    // Try to recover previous game stats
    game_stats tmpStats;

    EEPROM.begin(4095);
    EEPROM.get(STATS_ADDR, tmpStats);
    EEPROM.end();

    if (tmpStats.magic_num == MAGIC_NUM) {
        Serial.println("Revived bird");
        this->stats = tmpStats;
    } else {
        Serial.println("Created new bird");
        createID();
    }

    createAP();
}

void Bird::update() {
    updatePoints();
}

int Bird::getConnections() {
    return WiFi.softAPgetStationNum();
}

void Bird::setFlag(unsigned int flag) {
    if ((flag <= 2) && (stats.level < 3)) {
        stats.flag = flag;
        if (stats.level < 4) {
            ++stats.level;
            createAP();
        }
    }
}

int Bird::getPoints(unsigned int team) {
    team = (team > 2) ? 1 : team;
    return stats.points[team];
}

String Bird::getPointsString() {
    return '(' + String(stats.points[0]) + ',' + String(stats.points[1]) + ',' + String(stats.points[2]) + ')';
}

int Bird::getLevel() {
    return stats.level;
}

int Bird::getFlag() {
    return stats.flag;
}

bool Bird::reset(String password) {
    if (password == SUPER_SECRET) {
        Serial.println("Resetting Game");

        for (int i = 0; i < 3; i++) stats.points[i] = 0;

        stats.level = 0;
        stats.flag  = 1;

        createID();
        createAP();

        return true;
    }
    return false;
}

bool Bird::errored() {
    return error;
}