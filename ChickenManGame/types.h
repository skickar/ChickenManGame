#ifndef TYPES_H
#define TYPES_H

typedef enum TEAM {
    RED     = 0,
    GREEN   = 1,
    BLUE    = 2,
    NO_TEAM = 3
} TEAM;

typedef enum LEVEL {
    EASY   = 0,
    MEDIUM = 1,
    HARD   = 2,
    LOCKED = 3
} LEVEL;

typedef enum GAME_TYPE {
    CHICKEN     = 0,
    CHICKEN_MAN = 1
} GAME_TYPE;
// ========== Game Stats ========== //

// Used to verify memory of game_stats objects
#define GAME_MAGIC_NUM 1234567890

typedef struct game_stats {
    unsigned int magic_num : 32; // Used to verify memory

    GAME_TYPE mode;              // 0 = Chicken, 1 = Chicken Man

    unsigned int id : 8;         // [Chicken] Random generated ID

    LEVEL level;                 // [Chicken] 0: Easy, 1: Medium, 2: Hard, 3: Locked

    TEAM flag;                   // 0: Red, 1: Green, 3: Blue

    unsigned int points[3];      // Red-Points, Green-Points, Blue-Points

    char ssid[33];               // [Chicken] SSID of access point
    char pswd[65];               // [Chicken] Password of access point
} game_stats;


#endif /* ifndef TYPES_H */