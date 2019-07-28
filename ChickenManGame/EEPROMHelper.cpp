#include "EEPROMHelper.h"

#define BOOT_MAGIC_NUM 1234567890

// Used to verify memory
typedef struct boot {
    unsigned int magic_num : 32;
    unsigned int boot_num  : 8;
} boot;

namespace eeprom {
    void begin(const int eepromSize) {
        EEPROM.begin(eepromSize);
    }

    void end() {
        EEPROM.end();
    }

    bool checkBootNum(const int address) {
        boot b;

        EEPROM.get(address, b);

        if ((b.magic_num == BOOT_MAGIC_NUM) && (b.boot_num < 3)) {
            saveObject(address, boot{ BOOT_MAGIC_NUM, ++b.boot_num });
            return true;
        }

        return false;
    }

    void resetBootNum(const int address) {
        saveObject(address, boot{ BOOT_MAGIC_NUM, 1 });
    }
};