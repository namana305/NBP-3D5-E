#include "variable.h"
#include "key.h"
class GROUP
{
private:
    /* data */
public:
    KEY key1;
    KEY key2;
    KEY key3;
    KEY key4;
    KEY key5;
    KEY key6;
    uint8_t STATE;
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
    uint32_t NEED_HOTWATER_DISPENSING_TIME;
    void loadDataNeededForExtracting(uint8_t keyNum);

    bool isHotwaterDispensing;
    bool isCleaning;

    uint8_t GroupNum;
    uint32_t EEPROM_ADDR_START_ADDRESS;

    void init(uint8_t groupNum, uint32_t eeprom_addr_start_address);
    GROUP(/* args */);
    ~GROUP();
};
void GROUP::init(uint8_t groupNum, uint32_t eeprom_addr_start_address)
{
    GroupNum = groupNum;
    EEPROM_ADDR_START_ADDRESS = eeprom_addr_start_address;
    key1.init(KEY_ONE_ESPRESSO, 1, groupNum, eeprom_addr_start_address);
    key2.init(KEY_TWO_ESPRESSO, 2, groupNum, eeprom_addr_start_address + 32);
    key3.init(KEY_ONE_COFFEE, 3, groupNum, eeprom_addr_start_address + 64);
    key4.init(KEY_TWO_COFFEE, 4, groupNum, eeprom_addr_start_address + 96);
    key5.init(KEY_MANUAL, 5, groupNum, eeprom_addr_start_address + 128);
    key6.init(KEY_HOTWATER, 6, groupNum, eeprom_addr_start_address + 160);
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
    case 1:
        this->NEED_PREINFUSION_START_TIME = key1.DATA_PREINFUSION_START_TIME;
        this->NEED_PREINFUSION_WAIT_TIME = key1.DATA_PREINFUSION_WAIT_TIME;
        this->NEED_EXTRACTING_TIME = key1.DATA_EXTRACTING_TIME;
        this->NEED_TOP_PULSE_TIME = key1.DATA_TOP_PULSE_TIME;
        this->NEED_TOTAL_PULSE = key1.DATA_TOTAL_PULSE;
        this->NEED_TOTAL_ML = key1.DATA_TOTAL_ML;
        //this->NEED_HOTWATER_DISPENSING_TIME = key1.DATA_HOTWATER_DISPENSING_TIME;

        break;
    case 2:
        /* code */
        this->NEED_PREINFUSION_START_TIME = key2.DATA_PREINFUSION_START_TIME;
        this->NEED_PREINFUSION_WAIT_TIME = key2.DATA_PREINFUSION_WAIT_TIME;
        this->NEED_EXTRACTING_TIME = key2.DATA_EXTRACTING_TIME;
        this->NEED_TOP_PULSE_TIME = key2.DATA_TOP_PULSE_TIME;
        this->NEED_TOTAL_PULSE = key2.DATA_TOTAL_PULSE;
        this->NEED_TOTAL_ML = key2.DATA_TOTAL_ML;
        //this->NEED_HOTWATER_DISPENSING_TIME = key6.DATA_HOTWATER_DISPENSING_TIME;
        break;
    case 3:
        /* code */
        this->NEED_PREINFUSION_START_TIME = key3.DATA_PREINFUSION_START_TIME;
        this->NEED_PREINFUSION_WAIT_TIME = key3.DATA_PREINFUSION_WAIT_TIME;
        this->NEED_EXTRACTING_TIME = key3.DATA_EXTRACTING_TIME;
        this->NEED_TOP_PULSE_TIME = key3.DATA_TOP_PULSE_TIME;
        this->NEED_TOTAL_PULSE = key3.DATA_TOTAL_PULSE;
        this->NEED_TOTAL_ML = key3.DATA_TOTAL_ML;
        //this->NEED_HOTWATER_DISPENSING_TIME = key6.DATA_HOTWATER_DISPENSING_TIME;
        break;
    case 4:
        /* code */
        this->NEED_PREINFUSION_START_TIME = key4.DATA_PREINFUSION_START_TIME;
        this->NEED_PREINFUSION_WAIT_TIME = key4.DATA_PREINFUSION_WAIT_TIME;
        this->NEED_EXTRACTING_TIME = key4.DATA_EXTRACTING_TIME;
        this->NEED_TOP_PULSE_TIME = key4.DATA_TOP_PULSE_TIME;
        this->NEED_TOTAL_PULSE = key4.DATA_TOTAL_PULSE;
        this->NEED_TOTAL_ML = key4.DATA_TOTAL_ML;
        //this->NEED_HOTWATER_DISPENSING_TIME = key6.DATA_HOTWATER_DISPENSING_TIME;
        break;
    case 5:
        /* code */
        this->NEED_PREINFUSION_START_TIME = key5.DATA_PREINFUSION_START_TIME;
        this->NEED_PREINFUSION_WAIT_TIME = key5.DATA_PREINFUSION_WAIT_TIME;
        this->NEED_EXTRACTING_TIME = key5.DATA_EXTRACTING_TIME;
        this->NEED_TOP_PULSE_TIME = key5.DATA_TOP_PULSE_TIME;
        this->NEED_TOTAL_PULSE = key5.DATA_TOTAL_PULSE;
        this->NEED_TOTAL_ML = key5.DATA_TOTAL_ML;
        //this->NEED_HOTWATER_DISPENSING_TIME = key6.DATA_HOTWATER_DISPENSING_TIME;
        break;
    case 6:
        /* code */
        this->NEED_HOTWATER_DISPENSING_TIME = key6.DATA_HOTWATER_DISPENSING_TIME;
        break;
    default:

        break;
    }
}
