#include <Arduino.h>
#include "SPI.h"

#include "machine.h"
// put function declarations here:
uint32_t ms_now;
uint8_t led_dimming_now;
uint8_t isLedTopDimming;
machine VELO;

bool led_Run_state = false;

void expandio_transfer();
void start_extracting(uint8_t group, uint8_t key);
void stop_extracting(uint8_t group, uint8_t key);
void start_hotwater_dispensing(uint8_t group);
void stop_hotwater_dispensing(uint8_t group);
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  SPI.begin();
  VELO.LED_NORMAL_STATE=1;
  pinMode(EXPANDIO_ENABLE_PIN, OUTPUT);
  pinMode(EXPANDIO_LATCH_PIN, OUTPUT);
  pinMode(EXPANDIO_CLEAR_PIN, OUTPUT);
  digitalWrite(EXPANDIO_ENABLE_PIN, HIGH);
  digitalWrite(EXPANDIO_CLEAR_PIN, HIGH);
  delay(1000);
  digitalWrite(EXPANDIO_ENABLE_PIN, LOW);
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (millis() - ms_now >= 500)
  {
    led_Run_state ? TXLED0 : TXLED1;
    led_Run_state = !led_Run_state;
    ms_now = millis();
  }
  switch (VELO.scan_step)
  {
  case 0: // on gr1
    VELO.RELAY_REGISTER |= (1UL << (TRANSISTOR_GROUP1_POS));
    VELO.LED_REGISTER &= ~(1UL << (TRANSISTOR_GROUP2_POS));
    VELO.LED_REGISTER &= ~(1UL << (TRANSISTOR_GROUP3_POS));
    if (VELO.KEY_LED_NORMAL_STATE)
    {
      VELO.LED_REGISTER |= (1UL << (0));
      VELO.LED_REGISTER |= (1UL << (1));
      VELO.LED_REGISTER |= (1UL << (2));
      VELO.LED_REGISTER |= (1UL << (3));
      VELO.LED_REGISTER |= (1UL << (4));
      VELO.LED_REGISTER |= (1UL << (5));
    }
    else
    {
      VELO.LED_REGISTER &= ~(1UL << (0));
      VELO.LED_REGISTER &= ~(1UL << (1));
      VELO.LED_REGISTER &= ~(1UL << (2));
      VELO.LED_REGISTER &= ~(1UL << (3));
      VELO.LED_REGISTER &= ~(1UL << (4));
      VELO.LED_REGISTER &= ~(1UL << (5));
    }

    expandio_transfer();
    _delay_us(470);
    if (PINF != VELO.GR1_PINF_BUFFER)
    {
      Serial.print("GR1:");
      Serial.println(PINF, BIN);
      switch (PINF)
      {
      case 0B00000000:

        VELO.GR1.key1.IsPressing ? ((millis() - VELO.GR1.key1.StartPressingMS >= 120) && VELO.GR1.STATE == READY_STATE) ? start_extracting(1, 1) : stop_extracting(1, 1) : VELO.GR1.key2.IsPressing ? ((millis() - VELO.GR1.key2.StartPressingMS >= 120) && VELO.GR1.STATE == READY_STATE) ? start_extracting(1, 2) : stop_extracting(1, 2)
                                                                                                                                                                       : VELO.GR1.key3.IsPressing   ? ((millis() - VELO.GR1.key3.StartPressingMS >= 120) && VELO.GR1.STATE == READY_STATE) ? start_extracting(1, 3) : stop_extracting(1, 3)
                                                                                                                                                                       : VELO.GR1.key4.IsPressing   ? ((millis() - VELO.GR1.key4.StartPressingMS >= 120) && VELO.GR1.STATE == READY_STATE) ? start_extracting(1, 4) : stop_extracting(1, 4)
                                                                                                                                                                       : VELO.GR1.key5.IsPressing   ? ((millis() - VELO.GR1.key5.StartPressingMS >= 120) && VELO.GR1.STATE == READY_STATE) ? start_extracting(1, 5) : stop_extracting(1, 5)
                                                                                                                                                                       : VELO.GR1.key6.IsPressing   ? ((millis() - VELO.GR1.key6.StartPressingMS >= 120) && VELO.GR1.STATE == READY_STATE) ? start_hotwater_dispensing(1) : stop_hotwater_dispensing(1)
                                                                                                                                                                                                    : null_function();

        (VELO.GR1.isCleaningPressing && millis() - VELO.GR1.cleaningPressingStartMS >= 120 && VELO.GR1.STATE == READY_STATE) ? VELO.GR1.STATE = CLEANING_SATE : VELO.null_variable = 0;
        (VELO.GR1.isCleaningPressing && millis() - VELO.GR1.cleaningPressingStartMS >= 120 && VELO.GR1.STATE == CLEANING_SATE) ? VELO.GR1.STATE = READY_STATE : VELO.null_variable = 0;
        VELO.GR1.key1.IsPressing = false;
        VELO.GR1.key2.IsPressing = false;
        VELO.GR1.key3.IsPressing = false;
        VELO.GR1.key4.IsPressing = false;
        VELO.GR1.key5.IsPressing = false;
        VELO.GR1.key6.IsPressing = false;
        VELO.GR1.isCleaningPressing = false;
        break;
      case 0B00000001:
        if (!VELO.GR1.key1.IsPressing)
        {
          VELO.GR1.key1.StartPressingMS = millis();
          VELO.GR1.key1.IsPressing = true;
        }

        break;
      case 0B00000010:
        if (!VELO.GR1.key2.IsPressing)
        {
          VELO.GR1.key2.StartPressingMS = millis();
          VELO.GR1.key2.IsPressing = true;
        }

        break;
      case 0B00010000:
        if (!VELO.GR1.key3.IsPressing)
        {
          VELO.GR1.key3.StartPressingMS = millis();
          VELO.GR1.key3.IsPressing = true;
        }

        break;
      case 0B00100000:
        if (!VELO.GR1.key4.IsPressing)
        {
          VELO.GR1.key4.StartPressingMS = millis();
          VELO.GR1.key4.IsPressing = true;
        }

        break;
      case 0B01000000:
        if (!VELO.GR1.key5.IsPressing)
        {
          VELO.GR1.key5.StartPressingMS = millis();
          VELO.GR1.key5.IsPressing = true;
        }

        break;
      case 0B10000000:
        if (!VELO.GR1.key6.IsPressing)
        {
          VELO.GR1.key6.StartPressingMS = millis();
          VELO.GR1.key6.IsPressing = true;
        }

        break;
      case 0B01000001:
        if (!VELO.GR1.isCleaningPressing)
        {
          VELO.GR1.key1.StartPressingMS = millis();
          VELO.GR1.key1.IsPressing = false;
          VELO.GR1.key2.StartPressingMS = millis();
          VELO.GR1.key2.IsPressing = false;
          VELO.GR1.key3.StartPressingMS = millis();
          VELO.GR1.key3.IsPressing = false;
          VELO.GR1.key4.StartPressingMS = millis();
          VELO.GR1.key4.IsPressing = false;
          VELO.GR1.key5.StartPressingMS = millis();
          VELO.GR1.key5.IsPressing = false;
          VELO.GR1.key6.StartPressingMS = millis();
          VELO.GR1.key6.IsPressing = false;
          VELO.GR1.cleaningPressingStartMS = millis();
          VELO.GR1.isCleaningPressing = true;
        }

        break;
      default:
        break;
      }
      VELO.GR1_PINF_BUFFER = PINF;
    }
    VELO.SCAN_GROUP_NOW++;
    break;
  case 1: // off
    // (VELO.RELAY_REGISTER &= ~(1UL << (TRANSISTOR_GROUP1_POS));
    // VELO.LED_REGISTER &= ~(1UL << (TRANSISTOR_GROUP2_POS));
    // VELO.LED_REGISTER &= ~(1UL << (TRANSISTOR_GROUP3_POS));
    VELO.LED_REGISTER &= ~(1UL << (1));
    VELO.LED_REGISTER &= ~(1UL << (2));
    VELO.LED_REGISTER &= ~(1UL << (3));
    VELO.LED_REGISTER &= ~(1UL << (4));
    VELO.LED_REGISTER &= ~(1UL << (5));

    expandio_transfer();
    _delay_us(1000);
    VELO.SCAN_GROUP_NOW++;
    break;
  case 2: // oN gr2
    VELO.LED_REGISTER |= (1UL << (TRANSISTOR_GROUP2_POS));
    VELO.RELAY_REGISTER &= ~(1UL << (TRANSISTOR_GROUP1_POS));
    VELO.LED_REGISTER &= ~(1UL << (TRANSISTOR_GROUP3_POS));
    if (VELO.KEY_LED_NORMAL_STATE)
    {
      (VELO.GR2.isExtracting == 1) ? VELO.LED_REGISTER &= ~(1UL << (0)) : VELO.LED_REGISTER |= (1UL << (0));
      (VELO.GR2.isExtracting == 2) ? VELO.LED_REGISTER &= ~(1UL << (1)) : VELO.LED_REGISTER |= (1UL << (1));
      (VELO.GR2.isExtracting == 3) ? VELO.LED_REGISTER &= ~(1UL << (2)) : VELO.LED_REGISTER |= (1UL << (2));
      (VELO.GR2.isExtracting == 4) ? VELO.LED_REGISTER &= ~(1UL << (3)) : VELO.LED_REGISTER |= (1UL << (3));
      (VELO.GR2.isExtracting == 5) ? VELO.LED_REGISTER &= ~(1UL << (4)) : VELO.LED_REGISTER |= (1UL << (4));
      (VELO.GR2.isHotwaterDispensing) ? VELO.LED_REGISTER &= ~(1UL << (5)) : VELO.LED_REGISTER |= (1UL << (5));
    }
    else
    {
      (VELO.GR2.isExtracting == 1) ? VELO.LED_REGISTER |= (1UL << (0)) : VELO.LED_REGISTER &= ~(1UL << (0));
      (VELO.GR2.isExtracting == 2) ? VELO.LED_REGISTER |= (1UL << (1)) : VELO.LED_REGISTER &= ~(1UL << (1));
      (VELO.GR2.isExtracting == 3) ? VELO.LED_REGISTER |= (1UL << (2)) : VELO.LED_REGISTER &= ~(1UL << (2));
      (VELO.GR2.isExtracting == 4) ? VELO.LED_REGISTER |= (1UL << (3)) : VELO.LED_REGISTER &= ~(1UL << (3));
      (VELO.GR2.isExtracting == 5) ? VELO.LED_REGISTER |= (1UL << (4)) : VELO.LED_REGISTER &= ~(1UL << (4));
      (VELO.GR2.isHotwaterDispensing) ? VELO.LED_REGISTER |= (1UL << (5)) : VELO.LED_REGISTER &= ~(1UL << (5));
    }

    expandio_transfer();
    _delay_us(470);
    if (PINF != VELO.GR2_PINF_BUFFER)
    {
      Serial.print("GR2:");
      Serial.println(PINF, BIN);
      VELO.GR2_PINF_BUFFER = PINF;
    }

    VELO.SCAN_GROUP_NOW++;
    break;
  case 3: // off
          // (VELO.RELAY_REGISTER &= ~(1UL << (TRANSISTOR_GROUP1_POS));
          // VELO.LED_REGISTER &= ~(1UL << (TRANSISTOR_GROUP2_POS));
          // VELO.LED_REGISTER &= ~(1UL << (TRANSISTOR_GROUP3_POS));
    VELO.LED_REGISTER &= ~(1UL << (1));
    VELO.LED_REGISTER &= ~(1UL << (2));
    VELO.LED_REGISTER &= ~(1UL << (3));
    VELO.LED_REGISTER &= ~(1UL << (4));
    VELO.LED_REGISTER &= ~(1UL << (5));

    expandio_transfer();
    _delay_us(1000);
    VELO.SCAN_GROUP_NOW++;
    break;

  case 4: // on gr3
    VELO.LED_REGISTER |= (1UL << (TRANSISTOR_GROUP3_POS));
    VELO.RELAY_REGISTER &= ~(1UL << (TRANSISTOR_GROUP1_POS));
    VELO.LED_REGISTER &= ~(1UL << (TRANSISTOR_GROUP2_POS));
    if (VELO.KEY_LED_NORMAL_STATE)
    {
      (VELO.GR3.isExtracting == 1) ? VELO.LED_REGISTER &= ~(1UL << (0)) : VELO.LED_REGISTER |= (1UL << (0));
      (VELO.GR3.isExtracting == 2) ? VELO.LED_REGISTER &= ~(1UL << (1)) : VELO.LED_REGISTER |= (1UL << (1));
      (VELO.GR3.isExtracting == 3) ? VELO.LED_REGISTER &= ~(1UL << (2)) : VELO.LED_REGISTER |= (1UL << (2));
      (VELO.GR3.isExtracting == 4) ? VELO.LED_REGISTER &= ~(1UL << (3)) : VELO.LED_REGISTER |= (1UL << (3));
      (VELO.GR3.isExtracting == 5) ? VELO.LED_REGISTER &= ~(1UL << (4)) : VELO.LED_REGISTER |= (1UL << (4));
      (VELO.GR3.isHotwaterDispensing) ? VELO.LED_REGISTER &= ~(1UL << (5)) : VELO.LED_REGISTER |= (1UL << (5));
    }
    else
    {
      (VELO.GR3.isExtracting == 1) ? VELO.LED_REGISTER |= (1UL << (0)) : VELO.LED_REGISTER &= ~(1UL << (0));
      (VELO.GR3.isExtracting == 2) ? VELO.LED_REGISTER |= (1UL << (1)) : VELO.LED_REGISTER &= ~(1UL << (1));
      (VELO.GR3.isExtracting == 3) ? VELO.LED_REGISTER |= (1UL << (2)) : VELO.LED_REGISTER &= ~(1UL << (2));
      (VELO.GR3.isExtracting == 4) ? VELO.LED_REGISTER |= (1UL << (3)) : VELO.LED_REGISTER &= ~(1UL << (3));
      (VELO.GR3.isExtracting == 5) ? VELO.LED_REGISTER |= (1UL << (4)) : VELO.LED_REGISTER &= ~(1UL << (4));
      (VELO.GR3.isHotwaterDispensing) ? VELO.LED_REGISTER |= (1UL << (5)) : VELO.LED_REGISTER &= ~(1UL << (5));
    }
    expandio_transfer();
    _delay_us(470);
    if (PINF != VELO.GR3_PINF_BUFFER)
    {
      Serial.print("GR3:");
      Serial.println(PINF, BIN);
      VELO.GR3_PINF_BUFFER = PINF;
    }

    VELO.SCAN_GROUP_NOW++;
    break;
  case 5: // off
    // (VELO.RELAY_REGISTER &= ~(1UL << (TRANSISTOR_GROUP1_POS));
    // VELO.LED_REGISTER &= ~(1UL << (TRANSISTOR_GROUP2_POS));
    // VELO.LED_REGISTER &= ~(1UL << (TRANSISTOR_GROUP3_POS));
    VELO.LED_REGISTER &= ~(1UL << (1));
    VELO.LED_REGISTER &= ~(1UL << (2));
    VELO.LED_REGISTER &= ~(1UL << (3));
    VELO.LED_REGISTER &= ~(1UL << (4));
    VELO.LED_REGISTER &= ~(1UL << (5));

    expandio_transfer();
    _delay_us(1000);
    VELO.SCAN_GROUP_NOW = 0;
    break;

  default:
    break;
  }
}

void expandio_transfer()
{
  digitalWrite(EXPANDIO_LATCH_PIN, LOW);
  SPI.transfer(VELO.RELAY_REGISTER);
  SPI.transfer(VELO.LED_REGISTER);
  digitalWrite(EXPANDIO_LATCH_PIN, HIGH);
}
void start_extracting(uint8_t group, uint8_t key)
{
  Serial.print("start extracting group:");
  Serial.print(group);
  Serial.print(" key:");
  Serial.println(key);
  switch (group)
  {
  case 1:
    VELO.GR1.STATE = EXTRACTING_STATE;
    VELO.GR1.isExtracting = key;
    VELO.GR1.loadDataNeededForExtracting(key);
    break;
  case 2:
    VELO.GR2.STATE = EXTRACTING_STATE;
    VELO.GR2.isExtracting = key;
    VELO.GR2.loadDataNeededForExtracting(key);
    break;
  case 3:
    VELO.GR3.STATE = EXTRACTING_STATE;
    VELO.GR3.isExtracting = key;
    VELO.GR3.loadDataNeededForExtracting(key);
    break;

  default:
    break;
  }
}
void stop_extracting(uint8_t group, uint8_t key)
{
  Serial.print("stop extracting group:");
  Serial.print(group);
  Serial.print(" key:");
  Serial.println(key);
  switch (group)
  {
  case 1:
    VELO.GR1.STATE = READY_STATE;
    VELO.GR1.isExtracting = 0;
    break;
  case 2:
    VELO.GR2.STATE = READY_STATE;
    VELO.GR2.isExtracting = 0;
    break;
  case 3:
    VELO.GR3.STATE = READY_STATE;
    VELO.GR3.isExtracting = 0;
    break;
  default:
    break;
  }
}
void start_hotwater_dispensing(uint8_t group)
{
  Serial.print("start hotwater dispensing group:");
  Serial.println(group);
}
void stop_hotwater_dispensing(uint8_t group)
{
  Serial.print("stop hotwater dispensing group:");
  Serial.println(group);
}