#ifndef CONFIG_H
#define CONFIG_H

// ========== Device Config ========== //
// Sets the mode of the device (Chicken or Chicken Man)
// 0 = Chicken, 1 = Chicken-Man
// Only used as fallback if the switch (see hardware.h) is disabled
const int DEFAULT_CHICKEN = 0;

// ========== Game Config ========== //

// Password lists (below) must provide at least that amount!
const unsigned int NUM_PASSWORDS = 15;

// To unlock locked birds
const char* SUPER_SECRET PROGMEM = "edfd8e8160383696120dfb444a8b43f1";

// Interval for chicken man in which it connects to the chickens to generate handshakes
const unsigned long CHICKEN_MAN_INTERVAL = 30; // in seconds

// Points
const unsigned int POINTS_PER_SECOND[] = {
    0, // Points-Per-Second on Easy (Level 0) - no team by default
    1, // Points-Per-Second on Medium (Level 1)
    2, // Points-Per-Second on Hard (Level 2)
    4  // Points-Per-Second on Cooked (Level 3)
};

// How often to increment team points in seconds
const unsigned int POINT_INTERVAL = 1;

// Hide SSID at level 2 (hard)
const bool HIDDEN_SSID = false;

// Max connections to access point at the same time
const unsigned int MAX_CONNECTIONS = 5;

// Max channel for access point (11=US,13=Europe,14=Japan)
const unsigned int MAX_CHANNEL = 11;

// Level names
const char* DIFFICULTY[] PROGMEM = {
    "Easy",
    "Medium",
    "Hard",
    "Cooked"
};

// SSID Prefix
const char* SSID_PREFIX PROGMEM = "Chicken_";

// SSID Suffix
const char* SSID_SUFFIX[] PROGMEM = {
    "_00",
    "_01",
    "_02",
    "_03",
    "_04",
    "_05",
    "_06",
    "_07",
    "_08",
    "_09",
    "_10",
    "_11",
    "_12",
    "_13",
    "_14",
    "_15"
};

// Passwords
const char* EASY_PSWD[] PROGMEM = {
    "password",
    "123456789",
    "12345678",
    "1q2w3e4r",
    "sunshine",
    "football",
    "1234567890",
    "computer",
    "superman",
    "internet",
    "iloveyou",
    "1qaz2wsx",
    "baseball",
    "whatever",
    "princess"
};

const char* MEDIUM_PSWD[] PROGMEM = {
    "sweetheart",
    "overlord",
    "michaela",
    "meredith",
    "buttercup",
    "abc12345",
    "aardvark",
    "Passw0rd",
    "12345678910",
    "universal",
    "trinidad",
    "thursday",
    "standard",
    "pearljam",
    "anonymous"
};

const char* HARD_PSWD[] PROGMEM = {
    "prairie chicken",
    "chicken-breasted",
    "chicken broth",
    "chicken cholera",
    "chickenpox",
    "chicken pox",
    "chicken roost",
    "Chicken scratch",
    "chicken septicemia",
    "CHICKEN SHACK",
    "chicken snake",
    "chickensoup",
    "chicken-spirited",
    "chickens-toes",
    "chicken thief"
};

// ========== Web Interface ========== //
const char* HTML_PREFIX PROGMEM =
    "<!DOCYTPE html>"
    "<html>"
    "<head>"
    "<meta charset=\"utf-8\">"
    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
    "<title>LAY YOUR EGG</title>"
    "</head>"
    "<body>"
    "<style>"
    "body{display:flex; flex-direction:column;justify-content:center;align-items:center;height:100%; background-color:black;}"
    "a.button{display:inline-block;padding:.7em 1.4em;margin:0 .3em .3em 0;border-radius:.15em;box-sizing:border-box;text-decoration:none;font-family:Roboto,sans-serif;text-transform:uppercase;font-weight:400;color:#fff;width:30%;box-shadow:inset 0 -.6em 0 -.35em rgba(0,0,0,.17);text-align:center;position:relative}a.button:active{top:.1em}@media all and (max-width:500px){a.button{display:block;width:50%;margin:.4em auto}}"
    "p{color:#fff}"
    "</style>"
    "<a href=\"index.html?cmd=led+red\" class=\"button\" style=\"background-color:#f8333c\">Red Team</a>"
    "<a href=\"index.html?cmd=led+green\" class=\"button\" style=\"background-color:#44af69\">Green Team</a>"
    "<a href=\"index.html?cmd=led+blue\" class=\"button\" style=\"background-color:#3369ff\">Blue Team</a>"
    "<p>";

const char* HTML_SUFFIX PROGMEM =
    "</p>"
    "</body>"
    "<script>history.replaceState('Nothing to see here, please move on', 'LAY YOUR EGG', '/index.html');</script>"
    "</html>";

#endif /* ifndef CONFIG_H */