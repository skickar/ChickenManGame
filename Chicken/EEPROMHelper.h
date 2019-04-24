#ifndef EEPROMHELPER_H
#define EEPROMHELPER_H

// ========== Includes ========== //

// [Libraries]
#include <EEPROM.h>

// ========== Boot Counter Structure ========== //

// Used for memory verificaiton
#define BOOT_MAGIC_NUM 1234567890

typedef struct boot {
    unsigned int magic_num : 32;
    unsigned int boot_num  : 8;
} boot;

// ========== EEPROM Helper Class ========== //
class EEPROMHelper {
    public:
        template<typename T>
        static void saveObject(const int eepromSize, const int address, const T& t) {
            EEPROM.begin(eepromSize);

            EEPROM.put(address, t);

            EEPROM.commit();
            EEPROM.end();
        }

        template<typename T>
        static void getObject(const int eepromSize, const int address, const T& t) {
            EEPROM.begin(eepromSize);

            EEPROM.get(address, t);

            EEPROM.end();
        }

        static bool checkBootNum(const int eepromSize, const int address) {
            EEPROM.begin(eepromSize);

            boot b;
            EEPROM.get(address, b);

            EEPROM.end();

            if ((b.magic_num == BOOT_MAGIC_NUM) && (b.boot_num < 3)) {
                saveObject(eepromSize, address, boot{ BOOT_MAGIC_NUM, ++b.boot_num });
                return true;
            }

            return false;
        }

        static void resetBootNum(const int eepromSize, const int address) {
            EEPROM.begin(eepromSize);

            saveObject(eepromSize, address, boot{ BOOT_MAGIC_NUM, 1 });

            EEPROM.end();
        }
};

#endif /* ifndef EEPROMHELPER_H */