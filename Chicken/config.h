#ifndef CONFIG_H
#define CONFIG_H

// ========== Hardware ========== //

// [UART]
#define SERIAL_BAUD_RATE 115200
#define SERIAL_DELIMITER '\n'

// [LED]
// D1
#define LED_PIN_R 5
// D2
#define LED_PIN_G 4
// D3
#define LED_PIN_B 0

// ========== Game Config ========== //

// Password lists (below) must provide at least that amount!
#define NUM_PASSWORDS 15

// To unlock locked birds
#define SUPER_SECRET "edfd8e8160383696120dfb444a8b43f1"

// Points
#define EASY_POINTS_PER_SECOND 1
#define MEDIUM_POINTS_PER_SECOND 2
#define HARD_POINTS_PER_SECOND 5

// How often to increment team points
#define POINT_INTERVAL_SECONDS 1

// SSID Prefix
const char prefix0[] PROGMEM = "Chicken_Easy_";
const char prefix1[] PROGMEM = "Chicken_Medium_";
const char prefix2[] PROGMEM = "Chicken_Hard_";
const char prefix3[] PROGMEM = "CHICKEN_COOKED_";

// SSID Suffix
const char* numberAdd[] PROGMEM = {
    "00",
    "01",
    "02",
    "03",
    "04",
    "05",
    "06",
    "07",
    "08",
    "09",
    "10",
    "11",
    "12",
    "13",
    "14",
    "15"
};

// Passwords
const char* easyPass[] PROGMEM = {
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

const char* mediumPass[] PROGMEM = {
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

const char* hardPass[] PROGMEM = {
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

// HTML

const char* indexHTMLPrefix PROGMEM =
    "<!DOCYTPE html>"
    "<html>"
    "<head>"
    "<meta charset=\"utf-8\">"
    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
    "<title>LAY YOUR EGG</title>"
    "</head>"
    "<body style=\"display:flex; flex-direction:column;justify-content:center;align-items:center;height:100%; background-color:black;\">"
    "<style>"
    "a.button{display:inline-block;padding:.7em 1.4em;margin:0 .3em .3em 0;border-radius:.15em;box-sizing:border-box;text-decoration:none;font-family:Roboto,sans-serif;text-transform:uppercase;font-weight:400;color:#fff;width:30%;box-shadow:inset 0 -.6em 0 -.35em rgba(0,0,0,.17);text-align:center;position:relative}a.button:active{top:.1em}@media all and (max-width:500px){a.button{display:block;width:50%;margin:.4em auto}}"
    "</style>"
    "<a href=\"index.html?cmd=led+red\" class=\"button\" style=\"background-color:#f8333c\">Red Team</a>"
    "<a href=\"index.html?cmd=led+green\" class=\"button\" style=\"background-color:#44af69\">Green Team</a>"
    "<a href=\"index.html?cmd=led+blue\" class=\"button\" style=\"background-color:#3369ff\">Blue Team</a>"
    "<p>";

const char* indexHTMLSuffix PROGMEM =
    "</p>"
    "</body>"
    "</html>";

#endif /* ifndef CONFIG_H */