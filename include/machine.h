#include "variable.h"
#include "SPI.h"
#include "group.h"

class machine
{
private:
    /* data */

public:
    GROUP GR[3];
    // eeprom address 0
    uint8_t KEY_LED_NORMAL_STATE;
    // eeprom address 1
    uint8_t KEY_LED_MAX_POWER;
    // eeprom address 2
    uint8_t isFillWithpump;
    // eeprom address 3
    uint8_t isTeaWithpump;
    // eeprom address 4
    uint8_t isFillWhileExtracting;
    // eeprom address 5
    uint8_t isPreinfusion;
    // eeprom address 6
    uint32_t waterLevelSensorCompareValue = 300;
    // eeprom address 10
    uint8_t language;
    // eeprom address 11
    uint8_t isCrono;
    //eeprom address 12
    uint8_t isTemperatureControl;
    //eeprom address 13
    uint32_t temperatureSetpoint;
    //eeprom address 17
    uint8_t temperatureUnit;
    //----------------
    uint16_t analogWaterLevelMin;
    uint16_t analogWaterLevelMax;
    uint16_t led_dimming_now;
    uint16_t isLedTopDimming;
    uint32_t ledDimmingDuration;
    uint32_t startSetupMs;
    uint8_t FillingUpFlag;
    uint32_t FillingUpStartMs;
    uint32_t FillingStartMs;
    uint8_t STATE;
    uint8_t null_variable;
    uint8_t LED_REGISTER;
    uint8_t RELAY_REGISTER;
    uint8_t GR1_PINF_BUFFER;
    uint8_t GR2_PINF_BUFFER;
    uint8_t GR3_PINF_BUFFER;
    void init(uint16_t key_led_max_power);
    machine();
    ~machine();
    void run();
    String getRegisters();
    uint8_t getKeyLedNormalState();
    uint8_t getKeyLedMaxPower();
    uint8_t getIsFillWithpump();
    uint8_t getIsTeaWithpump();
    uint8_t getIsFillWhileExtracting();
    uint8_t getIsPreinfusion();
    uint8_t getAnalogWaterLevelMin();
    uint8_t getanalogWaterLevelMax();
    uint8_t getGroupState(uint8_t groupNum);
    uint32_t GetwaterLevelSensorCompareValue();
    uint8_t SetKeyLedNormalState(uint8_t state);
    uint8_t SetKeyLedMaxPower(uint8_t power);
    uint8_t SetIsFillWithpump(uint8_t state);
    uint8_t SetIsTeaWithpump(uint8_t state);
    uint8_t SetIsFillWhileExtracting(uint8_t state);
    uint8_t SetIsPreinfusion(uint8_t state);
    uint32_t SetwaterLevelSensorCompareValue(uint32_t level);
};
void machine::init(uint16_t key_led_max_power)
{
    this->KEY_LED_MAX_POWER = key_led_max_power;
    this->GR[0].init(0, 128, key_led_max_power, TRANSISTOR_GROUP1_POS, &this->RELAY_REGISTER, RELAY_EVGR1);
    this->GR[1].init(1, 292, key_led_max_power, TRANSISTOR_GROUP2_POS, &this->LED_REGISTER, RELAY_EVGR2);
    this->GR[2].init(2, 420, key_led_max_power, TRANSISTOR_GROUP3_POS, &this->LED_REGISTER, RELAY_EVGR3);
}
machine::machine()
{
}

machine::~machine()
{
}
void machine::run()
{
}
String machine::getRegisters()
{
    return String("[") + String(RELAY_REGISTER) + String(", ") + String(LED_REGISTER) + String("]");
}
uint8_t machine::getKeyLedNormalState()
{
    this->KEY_LED_NORMAL_STATE = EEPROM.read(0);
    return this->KEY_LED_NORMAL_STATE;
}
uint8_t machine::getKeyLedMaxPower()
{
    this->KEY_LED_MAX_POWER = EEPROM.read(1);
    return this->KEY_LED_MAX_POWER;
}
uint8_t machine::getIsFillWithpump()
{
    this->isFillWithpump = EEPROM.read(2);
    return this->isFillWithpump;
}
uint8_t machine::getIsTeaWithpump()
{
    this->isTeaWithpump = EEPROM.read(3);
    return this->isTeaWithpump;
}
uint8_t machine::getIsFillWhileExtracting()
{
    this->isFillWhileExtracting = EEPROM.read(4);
    return this->isFillWhileExtracting;
}
uint8_t machine::getIsPreinfusion()
{
    this->isPreinfusion = EEPROM.read(5);
    return this->isPreinfusion;
}
uint8_t machine::getAnalogWaterLevelMin()
{
    return this->analogWaterLevelMin;
}
uint8_t machine::getanalogWaterLevelMax()
{
    return this->analogWaterLevelMax;
}
uint8_t machine::getGroupState(uint8_t groupNum)
{
    return this->GR[groupNum].STATE;
}
uint32_t machine::GetwaterLevelSensorCompareValue(){
     EEPROM.get(6,this->waterLevelSensorCompareValue);
    return this->waterLevelSensorCompareValue;
}
uint8_t machine::SetKeyLedNormalState(uint8_t state)
{
    this->KEY_LED_NORMAL_STATE = state;
    EEPROM.write(0, state);
    return this->KEY_LED_NORMAL_STATE;
}
uint8_t machine::SetKeyLedMaxPower(uint8_t power)
{
    this->KEY_LED_MAX_POWER = power;
    EEPROM.write(1, power);
    return this->KEY_LED_MAX_POWER;
}
uint8_t machine::SetIsFillWithpump(uint8_t state)
{
    this->isFillWithpump = state;
    EEPROM.write(2, state);
    return this->isFillWithpump;
}
uint8_t machine::SetIsTeaWithpump(uint8_t state)
{
    this->isTeaWithpump = state;
    EEPROM.write(3, state);
    return this->isTeaWithpump;
}
uint8_t machine::SetIsFillWhileExtracting(uint8_t state)
{
    this->isFillWhileExtracting = state;
    EEPROM.write(4, state);
    return this->isFillWhileExtracting;
}
uint8_t machine::SetIsPreinfusion(uint8_t state)
{
    this->isPreinfusion = state;
    EEPROM.write(5, state);
    return this->isPreinfusion;
}
uint32_t machine::SetwaterLevelSensorCompareValue(uint32_t level)
{
    EEPROM.put(6, level);
    this->waterLevelSensorCompareValue = level;
    return this->waterLevelSensorCompareValue;
}
void null_function()
{
}