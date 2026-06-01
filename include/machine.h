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
    uint8_t STATE;
    uint8_t null_variable;
    uint8_t LED_REGISTER;
    uint8_t RELAY_REGISTER;
    bool LED_NORMAL_STATE;
    uint8_t scan_step;
    uint8_t SCAN_GROUP_NOW;
    uint8_t GR1_PINF_BUFFER;
    uint8_t GR2_PINF_BUFFER;
    uint8_t GR3_PINF_BUFFER;
    void init();
    void null_function();
    machine();
    ~machine();
    void run();
};
void machine::init()
{
    GR1.init(1, 0);
    GR2.init(2, 164);
    GR3.init(3, 328);
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