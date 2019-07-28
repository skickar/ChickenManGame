#pragma once

// ========== Hardware ========== //

// ===== LED ===== //

// Enabled/Disabled
#define LED_ENABLE

// Type
#define LED_ANALOG
// #define LED_NEOPIXEL

// === Analog LED settings === //
#if defined(LED_ANALOG)

// Pin for red LED: D1
  #define LED_PIN_R 5
// Pin for green LED: D2
  #define LED_PIN_G 4
// Pin for blue LED: D3
  #define LED_PIN_B 0

// === Neopixel LED settings === //
#elif defined(LED_NEOPIXEL)

// Pin for Neopixels
  #define NEOPIXEL_PIN 5
// Number of Neopixel LEDs
  #define NEOPIXEL_NUM 1
// Brightness of Neopixels ( 0 - 255 )
  #define NEOPIXEL_BRIGHTNESS 90

#endif /* if defined(LED_ANALOG) */

// ===== Switch ===== //
// Enabled/Disabled
#define SWITCH_ENABLE true
// Pin for switch: D7
#define SWITCH_PIN 13

// ===== UART ===== //
#define SERIAL_BAUD_RATE 115200
#define SERIAL_DELIMITER '\n'

// ===== EEPROM ===== //
#define EEPROM_SIZE       4095
#define EEPROM_STATS_ADDR 1
#define EEPROM_BOOT_ADDR  3210