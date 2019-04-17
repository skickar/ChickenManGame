// ========== Includes ========== //
#include "Bird.h"

// ========= Private ======== //

String Bird::getSSID() {
    if (level > 3) level = 3;
    if (id >= NUM_PASSWORDS) id = 0;

    return String(SSID_PREFIX) + String(DIFFICULTY[level]) + String(SSID_SUFFIX[id]);
}

String Bird::getPassword() {
    if (level > 3) level = 3;
    if (id >= NUM_PASSWORDS) id = 0;

    if (level == 3) return String(SUPER_SECRET);

    const char** pswdList[] = { EASY_PSWD, MEDIUM_PSWD, HARD_PSWD };
    return String(pswdList[level][id]);
}

int Bird::getChannel() {
    return (id % MAX_CHANNEL) + 1;
}

void Bird::createAP() {
    // Disconnect clients
    WiFi.softAPdisconnect();

    // Set IP Adress
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0));

    // Create SSID and password
    ssid = getSSID();
    pswd = getPassword();

    // Start access point
    WiFi.softAP(ssid.c_str(), pswd.c_str(), getChannel(), HIDDEN_SSID && level == 2, MAX_CONNECTIONS);

    // Print infos
    Serial.println("--------------------------------------------");
    Serial.printf("Chicken Level: \t%d\n", level);
    Serial.printf("Chicken ID: \t%d\n", id);
    Serial.printf("Channel: \t%d\n", getChannel());
    Serial.printf("MAC-Address: \t%s\n", WiFi.softAPmacAddress().c_str());
    Serial.printf("IP-Address: \t%s\n", WiFi.softAPIP().toString().c_str());
    Serial.printf("SSID: \t\t%s\t\n", ssid.c_str());
    Serial.printf("Password: \t%s\t\n", pswd.c_str());
    Serial.println("---------------------------------------------");
}

// Team-Point-Calculator
void Bird::updatePoints() {
    if (millis() - lastPointUpdate >= POINT_INTERVAL * 1000) {
        lastPointUpdate = millis();

        if (level > 3) level = 3;
        if (flag > 2) flag = 1;

        if (level != 3) { // Not locked
            points[flag] += POINTS_PER_SECOND[level];
        }
    }
}

void Bird::createID() {
    id = random(0, NUM_PASSWORDS);

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
            if (ssid.endsWith(SSID_SUFFIX[id])) {
                id = (id+1) % NUM_PASSWORDS;
                ++tries;
            }
        }

        Serial.printf("%d: '%s' (%d) %s\n", i, WiFi.SSID(i).c_str(), WiFi.RSSI(i), (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
    }

    if (tries >= NUM_PASSWORDS) {
        id    = 0;
        error = true;
        Serial.println("ERROR: All IDs already in use");
    } else {
        Serial.printf("ID set to %d\n", id);
    }
}

// ========= Public ========= //
void Bird::begin() {
    createID();
    createAP();
}

void Bird::update() {
    updatePoints();
}

int Bird::getConnections() {
    return WiFi.softAPgetStationNum();
}

void Bird::setFlag(unsigned int flag) {
    if ((flag <= 2) && (level < 3)) {
        this->flag = flag;
        if (level < 4) {
            ++level;
            createAP();
        }
    }
}

int Bird::getPoints(unsigned int team) {
    team = (team > 2) ? 1 : team;
    return points[team];
}

String Bird::getPointsString() {
    return '(' + String(points[0]) + ',' + String(points[1]) + ',' + String(points[2]) + ')';
}

int Bird::getLevel() {
    return level;
}

int Bird::getFlag() {
    return flag;
}

bool Bird::reset(String password) {
    if (password == SUPER_SECRET) {
        Serial.println("Resetting Game");

        for (int i = 0; i < 3; i++) points[i] = 0;
        level = 0;
        flag  = 1;
        createAP();

        return true;
    }
    return false;
}

bool Bird::errored() {
    return error;
}