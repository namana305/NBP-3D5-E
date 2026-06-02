#include "variable.h"
#include "SPI.h"
#include "group.h"

class machine
{
private:
    /* data */

public:
    GROUP GR1;
    GROUP GR2;
    GROUP GR3;
    uint8_t KEY_LED_NORMAL_STATE;
    uint8_t KEY_LED_MAX_POWER;
    uint8_t STATE;
    uint8_t null_variable;
    uint8_t LED_REGISTER;
    uint8_t RELAY_REGISTER;

    uint8_t SCAN_GROUP_NOW;
    uint8_t GR1_PINF_BUFFER;
    uint8_t GR2_PINF_BUFFER;
    uint8_t GR3_PINF_BUFFER;
    void init(uint8_t key_led_max_power);
    void null_function();
    machine();
    ~machine();
    void run();
};
void machine::init(uint8_t key_led_max_power)
{
    this->KEY_LED_MAX_POWER=key_led_max_power;
    GR1.init(1, 0,key_led_max_power);
    GR2.init(2, 164,key_led_max_power);
    GR3.init(3, 328,key_led_max_power);
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