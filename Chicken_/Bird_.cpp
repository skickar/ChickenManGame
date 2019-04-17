// ========== Includes ========== //
#include "Bird_.h"

// ========= Private ======== //

String Bird::getSSID() {
    if (level > 3) level = 3;
    if (id >= NUM_PASSWORDS) id = 0;

    return String(SSID_PREFIX[level]) + String(SSID_SUFFIX[id]);
}

String Bird::getPassword() {
    if (level > 3) level = 3;
    if (id >= NUM_PASSWORDS) id = 0;

    if (level == 3) return String(SUPER_SECRET);

    const char** pswdList[] = { EASY_PSWD, MEDIUM_PSWD, HARD_PSWD };
    return String(pswdList[level][id]);
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
    WiFi.softAP(ssid.c_str(), pswd.c_str(), id % 11, level == 2, 3);

    // Print infos
    Serial.println("--------------------------------------------");
    Serial.printf("Chicken Level: \t%d\n", level);
    Serial.printf("Chicken ID: \t%d\n", id);
    Serial.printf("Channel: \t\t%d\n", id % 11);
    Serial.printf("MAC-Address: \t%s\n", WiFi.softAPmacAddress().c_str());
    Serial.printf("IP-Address: \t%s\n", WiFi.softAPIP().toString().c_str());
    Serial.printf("SSID: \t\t%s\t\n", ssid.c_str());
    Serial.printf("Password: \t\t%s\t\n", pswd.c_str());
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

// ========= Public ========= //
void Bird::begin() {
    // Generate random ID
    id = random(0, NUM_PASSWORDS);

    // Access Point
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