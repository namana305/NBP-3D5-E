#include "variable.h"
#define KEY_ONE_ESPRESSO 1
#define KEY_TWO_ESPRESSO 2
#define KEY_ONE_COFFEE 3
#define KEY_TWO_COFFEE 4
#define KEY_MANUAL 5
#define KEY_HOTWATER 6
#define OFF 0
#define DIMMING 1
#define TOGLE 2
class KEY
{
private:
    /* data */
public:
    uint8_t Type;
    uint8_t Num;
    uint8_t State;
    uint8_t ParentGroup;
    uint8_t IsPressing;
    uint8_t was_setup;
    uint16_t ledPower;
    uint32_t StartPressingMS;
    uint32_t isLedDimming; // OFF||DIMMING||TOGLE
    uint32_t EEPROM_ADDR_START_ADDRESS;
    uint32_t EEPROM_ADDR_PREINFUSION_START_TIME;
    uint32_t EEPROM_ADDR_PREINFUSION_WAIT_TIME;
    uint32_t EEPROM_ADDR_EXTRACTING_TIME;
    uint32_t EEPROM_ADDR_TOP_PULSE_TIME;
    uint32_t EEPROM_ADDR_TOTAL_PULSE;
    float EEPROM_ADDR_TOTAL_ML;
    uint32_t EEPROM_ADDR_HOTWATER_DISPENSING_TIME;

    uint32_t DATA_PREINFUSION_START_TIME;
    uint32_t DATA_PREINFUSION_WAIT_TIME;
    uint32_t DATA_EXTRACTING_TIME;
    uint32_t DATA_TOP_PULSE_TIME;
    uint32_t DATA_TOTAL_PULSE;
    float DATA_TOTAL_ML;
    uint32_t DATA_HOTWATER_DISPENSING_TIME;
    void init(uint8_t type, uint8_t num, uint8_t parentGroup, uint32_t eeprom_addr_start_address, uint16_t led_max_power);
    void loadDataFromEEPROM();
    void saveDataToEEPROM();

    KEY();
    ~KEY();
};
void KEY::init(uint8_t type, uint8_t num, uint8_t parentGroup, uint32_t eeprom_addr_start_address, uint16_t led_max_power)
{
    Type = type;
    Num = num;
    ParentGroup = parentGroup;
    EEPROM_ADDR_START_ADDRESS = eeprom_addr_start_address;
    EEPROM_ADDR_PREINFUSION_START_TIME = eeprom_addr_start_address;
    EEPROM_ADDR_PREINFUSION_WAIT_TIME = eeprom_addr_start_address + 4;
    EEPROM_ADDR_EXTRACTING_TIME = eeprom_addr_start_address + 8;
    EEPROM_ADDR_TOP_PULSE_TIME = eeprom_addr_start_address + 12;
    EEPROM_ADDR_TOTAL_PULSE = eeprom_addr_start_address + 16;
    EEPROM_ADDR_TOTAL_ML = eeprom_addr_start_address + 20;
    EEPROM_ADDR_HOTWATER_DISPENSING_TIME = eeprom_addr_start_address + 24;
    ledPower = led_max_power;
}
KEY::KEY()
{
}

KEY::~KEY()
{
}
void KEY::loadDataFromEEPROM()
{
    EEPROM.get(EEPROM_ADDR_PREINFUSION_START_TIME, this->DATA_PREINFUSION_START_TIME);
    EEPROM.get(EEPROM_ADDR_PREINFUSION_WAIT_TIME, this->DATA_PREINFUSION_WAIT_TIME);
    EEPROM.get(EEPROM_ADDR_EXTRACTING_TIME, this->DATA_EXTRACTING_TIME);
    EEPROM.get(EEPROM_ADDR_TOP_PULSE_TIME, this->DATA_TOP_PULSE_TIME);
    EEPROM.get(EEPROM_ADDR_TOTAL_PULSE, this->DATA_TOTAL_PULSE);
    EEPROM.get(EEPROM_ADDR_TOTAL_ML, this->DATA_TOTAL_ML);
    EEPROM.get(EEPROM_ADDR_HOTWATER_DISPENSING_TIME, DATA_HOTWATER_DISPENSING_TIME);
}
void KEY::saveDataToEEPROM()
{
    EEPROM.put(EEPROM_ADDR_PREINFUSION_START_TIME, this->DATA_PREINFUSION_START_TIME);
    EEPROM.put(EEPROM_ADDR_PREINFUSION_WAIT_TIME, this->DATA_PREINFUSION_WAIT_TIME);
    EEPROM.put(EEPROM_ADDR_EXTRACTING_TIME, this->DATA_EXTRACTING_TIME);
    EEPROM.put(EEPROM_ADDR_TOP_PULSE_TIME, this->DATA_TOP_PULSE_TIME);
    EEPROM.put(EEPROM_ADDR_TOTAL_PULSE, this->DATA_TOTAL_PULSE);
    EEPROM.put(EEPROM_ADDR_TOTAL_ML, this->DATA_TOTAL_ML);
    EEPROM.put(EEPROM_ADDR_HOTWATER_DISPENSING_TIME, this->DATA_HOTWATER_DISPENSING_TIME);
}