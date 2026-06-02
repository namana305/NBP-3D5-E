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
#define FILLING_STATE 6
// config key

// config led

#define TRANSISTOR_GROUP1_POS 0
#define TRANSISTOR_GROUP2_POS 6
#define TRANSISTOR_GROUP3_POS 7
//config ev
#define RELAY_PUMP 1
#define RELAY_EVGR1 2
#define RELAY_EVGR2 3
#define RELAY_EVGR3 4
#define RELAY_RISC 5
#define RELAY_EVTEA 6
#define RELAY_EVFILL 7 
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


