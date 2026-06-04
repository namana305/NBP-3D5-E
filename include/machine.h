#include "variable.h"
#include "SPI.h"
#include "group.h"

class machine
{
private:
    /* data */

public:
    GROUP GR[3];
    // eeprom
    uint8_t KEY_LED_NORMAL_STATE;
    uint8_t KEY_LED_MAX_POWER;
    uint8_t isFillWithpump;
    uint8_t isTeaWithpump;
    uint8_t isFillWhileExtracting;
    uint8_t isPreinfusion;
    uint16_t analogWaterLevelMin;
    uint16_t analogWaterLevelMax;
    //----------------
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
};
void machine::init(uint16_t key_led_max_power)
{
    this->KEY_LED_MAX_POWER = key_led_max_power;
    this->GR[0].init(0, 0, key_led_max_power, TRANSISTOR_GROUP1_POS, &this->RELAY_REGISTER, RELAY_EVGR1);
    this->GR[1].init(1, 164, key_led_max_power, TRANSISTOR_GROUP2_POS, &this->LED_REGISTER, RELAY_EVGR2);
    this->GR[2].init(2, 328, key_led_max_power, TRANSISTOR_GROUP3_POS, &this->LED_REGISTER, RELAY_EVGR3);
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
void null_function()
{
}