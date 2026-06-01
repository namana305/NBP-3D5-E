// this is all global variable using for machine
#include <Arduino.h>
#include "EEPROM.h"
//config group

//config machine
#define SETUP_STATE 0
#define READY_STATE 1
#define EXTRACTING_STATE 2
#define SETUP_WATER_STATE 3
#define CLEANING_SATE 4
#define TRANFERRING_DATA_STATE 5

// config key

#define KEYPORT DDRF
#define KEY1 (uint8_t)23U // A5
#define KEY2 (uint8_t)22U // A4
#define KEY3 (uint8_t)21U // A3
#define KEY4 (uint8_t)20U // A2
#define KEY5 (uint8_t)19U // A1
#define KEY6 (uint8_t)18U // A0
// config led
#define GR1_LED1_PORT _SFR_IO8(0x0F)
#define GR1_LED1_PIN 0
#define GR1_LED2_PORT _SFR_IO8(0x0F)
#define GR1_LED2_PIN 1
#define GR1_LED3_PORT _SFR_IO8(0x0F)
#define GR1_LED3_PIN 4
#define GR1_LED4_PORT _SFR_IO8(0x0F)
#define GR1_LED4_PIN 5
#define GR1_LED5_PORT _SFR_IO8(0x0F)
#define GR1_LED5_PIN 6
#define GR1_LED6_PORT _SFR_IO8(0x0F)
#define GR1_LED6_PIN 7

#define GR2_LED1_PORT _SFR_IO8(0x0F)
#define GR2_LED1_PIN 0
#define GR2_LED2_PORT _SFR_IO8(0x0F)
#define GR2_LED2_PIN 1
#define GR2_LED3_PORT _SFR_IO8(0x0F)
#define GR2_LED3_PIN 4
#define GR2_LED4_PORT _SFR_IO8(0x0F)
#define GR2_LED4_PIN 5
#define GR2_LED5_PORT _SFR_IO8(0x0F)
#define GR2_LED5_PIN 6
#define GR2_LED6_PORT _SFR_IO8(0x0F)
#define GR2_LED6_PIN 7

#define GR3_LED1_PORT _SFR_IO8(0x0F)
#define GR3_LED1_PIN 0
#define GR3_LED2_PORT _SFR_IO8(0x0F)
#define GR3_LED2_PIN 1
#define GR3_LED3_PORT _SFR_IO8(0x0F)
#define GR3_LED3_PIN 4
#define GR3_LED4_PORT _SFR_IO8(0x0F)
#define GR3_LED4_PIN 5
#define GR3_LED5_PORT _SFR_IO8(0x0F)
#define GR3_LED5_PIN 6
#define GR3_LED6_PORT _SFR_IO8(0x0F)
#define GR3_LED6_PIN 7

#define FREE_LED_OFF 0
#define FREE_LED_ON 1
#define TRANSISTOR_GROUP1_POS 0
#define TRANSISTOR_GROUP2_POS 6
#define TRANSISTOR_GROUP3_POS 7

#define LED_WATER_LEVEL_PORT _SFR_IO8(0x09)
#define LED_WATER_LEVEL_PIN 0
// config expand IO
#define T_SPI 0
#define T_SHIFTOUT 1
#define T_I2C 2
#define EXPANDIO_CLOCK_PIN SCK
#define EXPANDIO_MISO_PIN MISO
#define EXPANDIO_MOSI_PIN MOSI
#define EXPANDIO_LATCH_PIN 10
#define EXPANDIO_ENABLE_PIN 13
#define EXPANDIO_CLEAR_PIN 5
#define EXPANDIO_TRANFER_TYPE SPI

// config water level
#define WATER_LEVEL_MAX_PIN A6
#define WATER_LEVEL_MIN_PIN A11
#define WATER_LEVEL_CHECK_INTERVAL 500
#define WATER_LEVEL_STILL_PUMP_AFTER 2000


