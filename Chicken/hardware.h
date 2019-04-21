#ifndef HARDWARE_H
#define HARDWARE_H

// ========== Hardware ========== //

// [LED]
// D1
const int LED_PIN_R = 5;
// D2
const int LED_PIN_G = 4;
// D3
const int LED_PIN_B = 0;

// [UART]
const unsigned int SERIAL_BAUD_RATE = 115200;
const char SERIAL_DELIMITER         = '\n';

// [EEPROM]
const int EEPROM_SIZE       = 4095;
const int EEPROM_STATS_ADDR = 1;
const int EEPROM_BOOT_ADDR  = 3210;

#endif /* ifndef HARDWARE_H */