#include "variable.h"
#include "key.h"
class GROUP
{
private:
    /* data */
public:
    KEY key[6];
    uint8_t STATE;
    uint8_t PINF_BUFFER;
    uint8_t TRANSISTOR_POS;
    uint8_t *TRANSISTOR_REG;
    uint8_t SOLENOID_RELAY_POS;
    uint8_t isExtracting;
    uint8_t isCleaningPressing;
    uint32_t cleaningPressingStartMS;
    uint32_t extractingStartMS;
    uint32_t NEED_PREINFUSION_START_TIME;
    uint32_t NEED_PREINFUSION_WAIT_TIME;
    uint32_t NEED_EXTRACTING_TIME;
    uint32_t NEED_TOP_PULSE_TIME;
    uint32_t NEED_TOTAL_PULSE;
    float NEED_TOTAL_ML;
    uint32_t hotWaterDispensingStartMS;
    uint8_t isHotwaterDispensing;
    uint32_t NEED_HOT_WATER_TIME;
    uint32_t NEED_HOTWATER_DISPENSING_TIME;
    void loadDataNeededForExtracting(uint8_t keyNum);

    bool isCleaning;

    uint8_t GroupNum;
    uint32_t EEPROM_ADDR_START_ADDRESS;

    uint32_t led_power_count_now;
    void init(uint8_t groupNum, uint32_t eeprom_addr_start_address, uint16_t led_max_power, uint8_t transistor_pos, uint8_t *transistor_register, uint8_t solenoid_pos);
    GROUP(/* args */);
    ~GROUP();
};
void GROUP::init(uint8_t groupNum, uint32_t eeprom_addr_start_address, uint16_t led_max_power, uint8_t transistor_pos, uint8_t *transistor_register, uint8_t solenoid_pos)
{
    GroupNum = groupNum;
    EEPROM_ADDR_START_ADDRESS = eeprom_addr_start_address;
    this->key[0].init(KEY_ONE_ESPRESSO, 0, groupNum, eeprom_addr_start_address, led_max_power);
    this->key[1].init(KEY_TWO_ESPRESSO, 1, groupNum, eeprom_addr_start_address + 32, led_max_power);
    this->key[2].init(KEY_ONE_COFFEE, 2, groupNum, eeprom_addr_start_address + 64, led_max_power);
    this->key[3].init(KEY_TWO_COFFEE, 3, groupNum, eeprom_addr_start_address + 96, led_max_power);
    this->key[4].init(KEY_MANUAL, 4, groupNum, eeprom_addr_start_address + 128, led_max_power);
    this->key[5].init(KEY_HOTWATER, 5, groupNum, eeprom_addr_start_address + 160, led_max_power);
    TRANSISTOR_POS = transistor_pos;
    TRANSISTOR_REG = transistor_register;
    SOLENOID_RELAY_POS = solenoid_pos;
}
GROUP::GROUP(/* args */)
{
}

GROUP::~GROUP()
{
}
void GROUP::loadDataNeededForExtracting(uint8_t keyNum)
{
    switch (keyNum)
    {
    case 0:
        this->NEED_PREINFUSION_START_TIME = this->key[0].DATA_PREINFUSION_START_TIME;
        this->NEED_PREINFUSION_WAIT_TIME = this->key[0].DATA_PREINFUSION_WAIT_TIME;
        this->NEED_EXTRACTING_TIME = this->key[0].DATA_EXTRACTING_TIME;
        this->NEED_TOP_PULSE_TIME = this->key[0].DATA_TOP_PULSE_TIME;
        this->NEED_TOTAL_PULSE = this->key[0].DATA_TOTAL_PULSE;
        this->NEED_TOTAL_ML = this->key[0].DATA_TOTAL_ML;
        // this->NEED_HOTWATER_DISPENSING_TIME = this->key[1].DATA_HOTWATER_DISPENSING_TIME;

        break;
    case 1:
        /* code */
        this->NEED_PREINFUSION_START_TIME = this->key[1].DATA_PREINFUSION_START_TIME;
        this->NEED_PREINFUSION_WAIT_TIME = this->key[1].DATA_PREINFUSION_WAIT_TIME;
        this->NEED_EXTRACTING_TIME = this->key[1].DATA_EXTRACTING_TIME;
        this->NEED_TOP_PULSE_TIME = this->key[1].DATA_TOP_PULSE_TIME;
        this->NEED_TOTAL_PULSE = this->key[1].DATA_TOTAL_PULSE;
        this->NEED_TOTAL_ML = this->key[1].DATA_TOTAL_ML;
        // this->NEED_HOTWATER_DISPENSING_TIME = this->key[6].DATA_HOTWATER_DISPENSING_TIME;
        break;
    case 2:
        /* code */
        this->NEED_PREINFUSION_START_TIME = this->key[2].DATA_PREINFUSION_START_TIME;
        this->NEED_PREINFUSION_WAIT_TIME = this->key[2].DATA_PREINFUSION_WAIT_TIME;
        this->NEED_EXTRACTING_TIME = this->key[2].DATA_EXTRACTING_TIME;
        this->NEED_TOP_PULSE_TIME = this->key[2].DATA_TOP_PULSE_TIME;
        this->NEED_TOTAL_PULSE = this->key[2].DATA_TOTAL_PULSE;
        this->NEED_TOTAL_ML = this->key[2].DATA_TOTAL_ML;
        // this->NEED_HOTWATER_DISPENSING_TIME = this->key[6].DATA_HOTWATER_DISPENSING_TIME;
        break;
    case 3:
        /* code */
        this->NEED_PREINFUSION_START_TIME = this->key[3].DATA_PREINFUSION_START_TIME;
        this->NEED_PREINFUSION_WAIT_TIME = this->key[3].DATA_PREINFUSION_WAIT_TIME;
        this->NEED_EXTRACTING_TIME = this->key[3].DATA_EXTRACTING_TIME;
        this->NEED_TOP_PULSE_TIME = this->key[3].DATA_TOP_PULSE_TIME;
        this->NEED_TOTAL_PULSE = this->key[3].DATA_TOTAL_PULSE;
        this->NEED_TOTAL_ML = this->key[3].DATA_TOTAL_ML;
        // this->NEED_HOTWATER_DISPENSING_TIME = this->key[6].DATA_HOTWATER_DISPENSING_TIME;
        break;
    case 4:
        /* code */
        this->NEED_PREINFUSION_START_TIME = this->key[4].DATA_PREINFUSION_START_TIME;
        this->NEED_PREINFUSION_WAIT_TIME = this->key[4].DATA_PREINFUSION_WAIT_TIME;
        this->NEED_EXTRACTING_TIME = this->key[4].DATA_EXTRACTING_TIME;
        this->NEED_TOP_PULSE_TIME = this->key[4].DATA_TOP_PULSE_TIME;
        this->NEED_TOTAL_PULSE = this->key[4].DATA_TOTAL_PULSE;
        this->NEED_TOTAL_ML = this->key[4].DATA_TOTAL_ML;
        // this->NEED_HOTWATER_DISPENSING_TIME = this->key[6].DATA_HOTWATER_DISPENSING_TIME;
        break;
    case 5:
        /* code */
        this->NEED_HOTWATER_DISPENSING_TIME = this->key[5].DATA_HOTWATER_DISPENSING_TIME;
        break;
    default:

        break;
    }
}
