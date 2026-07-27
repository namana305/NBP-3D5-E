#include <Arduino.h>
#include "SPI.h"

#include "machine.h"
// put function declarations here:
uint32_t ms_now;
uint32_t start_setup_ms;
machine VELO;

bool led_Run_state = false;

void expandio_transfer();
void start_extracting(uint8_t group, uint8_t key);
void stop_extracting(uint8_t group, uint8_t key);
void start_hotwater_dispensing(uint8_t group);
void stop_hotwater_dispensing(uint8_t group);
void stop_pump();
void setup()
{
  // put your setup code here, to run once:
  VELO.init(255);
  Serial.begin(115200);

  SPI.setClockDivider(SPI_CLOCK_DIV32);
  SPI.begin();
  VELO.KEY_LED_NORMAL_STATE = 1;

  pinMode(EXPANDIO_ENABLE_PIN, OUTPUT);
  pinMode(EXPANDIO_LATCH_PIN, OUTPUT);
  pinMode(EXPANDIO_CLEAR_PIN, OUTPUT);
  digitalWrite(EXPANDIO_ENABLE_PIN, HIGH);
  digitalWrite(EXPANDIO_CLEAR_PIN, HIGH);
  delay(1000);
  digitalWrite(EXPANDIO_ENABLE_PIN, LOW);
  VELO.GR[0].STATE = READY_STATE;
  VELO.GR[1].STATE = READY_STATE;
  VELO.GR[2].STATE = READY_STATE;
}

void loop()
{
  // put your main code here, to run repeatedly:
  // watch water level
  if (millis() - ms_now >= WATER_LEVEL_CHECK_INTERVAL)
  {
    led_Run_state ? TXLED0 : TXLED1;
    led_Run_state = !led_Run_state;
    VELO.analogWaterLevelMin = analogRead(WATER_LEVEL_MIN_PIN);
    VELO.analogWaterLevelMax = analogRead(WATER_LEVEL_MAX_PIN);

    if (VELO.analogWaterLevelMax > VELO.waterLevelSensorCompareValue)
    {
      if (VELO.isFillWhileExtracting)
      {
        if (VELO.GR[0].STATE == READY_STATE || VELO.GR[1].STATE == READY_STATE || VELO.GR[2].STATE == READY_STATE)
        {
          VELO.RELAY_REGISTER |= (1UL << (RELAY_PUMP));
          VELO.RELAY_REGISTER |= (1UL << (RELAY_EVFILL));
          VELO.RELAY_REGISTER &= ~(1UL << (RELAY_RISC));
          VELO.STATE = FILLING_STATE;
          VELO.FillingUpFlag = false;
        }

      }
    }
    else
    {
      if (VELO.FillingUpFlag == false)
      {
        VELO.FillingUpFlag = true;
        VELO.FillingUpStartMs = millis();
        // //Serial.print("water level min:");
        // //Serial.println(VELO.analogWaterLevelMin);
        // //Serial.print("water level max:");
        // //Serial.println(VELO.analogWaterLevelMax);
      }
    }
    if (VELO.FillingUpFlag)
    {
      if (millis() - VELO.FillingUpStartMs >= WATER_LEVEL_STILL_PUMP_AFTER)
      {
        VELO.STATE = READY_STATE;
        VELO.RELAY_REGISTER |= (1UL << (RELAY_RISC));
        VELO.RELAY_REGISTER &= ~(1UL << (RELAY_EVFILL));
        stop_pump();
      }
    }
    ms_now = millis();
  }
  // dimming

  if (VELO.isLedTopDimming)
  {
    VELO.led_dimming_now--;
    for (uint8_t g = 0; g < 3; g++)
    {
      for (uint8_t kk = 0; kk < 6; kk++)
      {
        switch (VELO.GR[g].key[kk].isLedDimming)
        {
        case DIMMING:
          VELO.GR[g].key[kk].ledPower = VELO.led_dimming_now;
          break;
        case TOGLE:
          VELO.GR[g].key[kk].ledPower = VELO.KEY_LED_MAX_POWER;
          break;
        default:
          break;
        }
      }
    }

    if (VELO.led_dimming_now <= 0)
    {
      VELO.isLedTopDimming = false;
    }
  }
  else
  {
    VELO.led_dimming_now++;
    for (uint8_t g = 0; g < 3; g++)
    {
      for (uint8_t kk = 0; kk < 6; kk++)
      {
        switch (VELO.GR[g].key[kk].isLedDimming)
        {
        case DIMMING:
          VELO.GR[g].key[kk].ledPower = VELO.led_dimming_now;
          break;
        case TOGLE:
          VELO.GR[g].key[kk].ledPower = 0;
          break;
        default:
          break;
        }
      }
    }
    if (VELO.led_dimming_now >= VELO.KEY_LED_MAX_POWER)
    {
      VELO.isLedTopDimming = true;
    }
  }
  //   Serial.println(VELO.led_dimming_now);
  //   Serial.println(VELO.GR[0].key[0].ledPower);
  //   Serial.println(((float)VELO.GR[0].key[0].ledPower / (float)(VELO.KEY_LED_MAX_POWER) * 25));
  //   Serial.println((uint8_t)((float)VELO.GR[0].key[1].ledPower / (float)VELO.KEY_LED_MAX_POWER * 25));
  //   Serial.println((uint8_t)((float)VELO.GR[0].key[2].ledPower / (float)VELO.KEY_LED_MAX_POWER * 25));
  //   VELO.ledDimmingDuration = millis();
  // watch hot water
  for (uint8_t g = 0; g < 3; g++)
  {
    if (VELO.GR[g].isHotwaterDispensing)
    {
      if (VELO.STATE == SETUP_WATER_STATE)
      {
      }
      else
      {
        if (millis() - VELO.GR[g].hotWaterDispensingStartMS > VELO.GR[g].NEED_HOT_WATER_TIME)
        {
          stop_hotwater_dispensing(g);
        }
      }
    }
  }
  // watch for setup_state led
  Serial.println(VELO.STATE);
  delay(500);
  if (VELO.STATE == SETUP_WATER_STATE)
  {
    // Serial.println("setup");
    // delay(1000);
    for (uint8_t g = 0; g < 3; g++)
    {
      if (VELO.GR[g].STATE == READY_STATE)
      {
        for (uint8_t k = 0; k < 6; k++)
        {
          if (VELO.GR[g].key[k].was_setup)
          {
            VELO.GR[g].key[k].isLedDimming = DIMMING;
            // Serial.println(VELO.GR[g].key[k].isLedDimming);
            // delay(1000);
          }
        }
      }
    }
  }

  //  scan key
  for (uint8_t g = 0; g < 3; g++)
  {
    *(uint8_t *)VELO.GR[g].TRANSISTOR_REG |= (1UL << (VELO.GR[g].TRANSISTOR_POS));
    VELO.GR[g].led_power_count_now < 25 ? VELO.GR[g].led_power_count_now++ : VELO.GR[g].led_power_count_now = 0;

    for (uint8_t ll = 0; ll < 6; ll++)
    {
      if ((uint8_t)((float)VELO.GR[g].key[ll].ledPower / (float)VELO.KEY_LED_MAX_POWER * 25) > VELO.GR[g].led_power_count_now)
      {
        VELO.LED_REGISTER |= (1UL << (ll));
      }
      else
      {
        VELO.LED_REGISTER &= ~(1UL << (ll));
      }
    }

    expandio_transfer();

    if (PINF != VELO.GR[g].PINF_BUFFER)
    {
      // Serial.print("GR[");
      // Serial.print(g);
      // Serial.print("]:");
      // Serial.println(PINF, BIN);

      switch (PINF)
      {
      case 0B00000000:
        for (uint8_t k = 0; k < 6; k++)
        {
          if (VELO.GR[g].key[k].IsPressing && millis() - VELO.GR[g].key[k].StartPressingMS >= 120)
          {
            // Serial.print(millis() - VELO.GR[g].key[k].StartPressingMS);
            // Serial.println("ms");
            switch (VELO.GR[g].key[k].Type)
            {
            case KEY_HOTWATER:
              if (VELO.STATE == READY_STATE)
              {
                if (VELO.GR[g].isHotwaterDispensing == 0)
                {
                  start_hotwater_dispensing(g);
                }
                else
                {
                  stop_hotwater_dispensing(g);
                }
              }
              break;
            case KEY_MANUAL:
              if (VELO.STATE == SETUP_WATER_STATE)
              {
                for (uint8_t gg = 0; gg < 3; gg++)
                {
                  for (uint8_t kk = 0; kk < 6; kk++)
                  {
                    VELO.GR[gg].key[kk].was_setup = false;
                  }
                }
                VELO.startSetupMs = millis();
              }
              else
              {
                if (VELO.GR[g].STATE == READY_STATE)
                {
                  start_extracting(g, k);
                }
                else
                {
                  stop_extracting(VELO.GR[g].GroupNum, VELO.GR[g].isExtracting - 1);
                }
              }
              break;
            default:
              if (VELO.GR[g].STATE == READY_STATE)
              {
                start_extracting(g, k);
              }
              else
              {
                stop_extracting(VELO.GR[g].GroupNum, VELO.GR[g].isExtracting - 1);
              }
              break;
            }
          }
        }
        if (VELO.GR[g].isCleaningPressing)
        {
          if (VELO.STATE != SETUP_STATE && VELO.STATE != SETUP_WATER_STATE)
          {
            if (millis() - VELO.GR[g].cleaningPressingStartMS >= 120)
            {
              if (VELO.GR[g].STATE == READY_STATE)
              {
                VELO.GR[g].STATE = CLEANING_SATE;
                VELO.GR[g].cleaningPressingStartMS = millis();
                VELO.GR[g].key[0].isLedDimming = DIMMING;
                VELO.GR[g].key[1].ledPower = 0;
                VELO.GR[g].key[1].ledPower = 0;
                VELO.GR[g].key[2].ledPower = 0;
                VELO.GR[g].key[3].ledPower = 0;
                VELO.GR[g].key[4].ledPower = 0;
                VELO.GR[g].key[4].isLedDimming = DIMMING;
              }
              else if (VELO.GR[g].STATE == CLEANING_SATE)
              {
                VELO.GR[g].STATE = READY_STATE;
                if (VELO.KEY_LED_NORMAL_STATE == 1)
                {
                  VELO.GR[g].key[0].isLedDimming = OFF;
                  VELO.GR[g].key[4].isLedDimming = OFF;
                  VELO.GR[g].key[0].ledPower = VELO.KEY_LED_MAX_POWER;
                  VELO.GR[g].key[1].ledPower = VELO.KEY_LED_MAX_POWER;
                  VELO.GR[g].key[2].ledPower = VELO.KEY_LED_MAX_POWER;
                  VELO.GR[g].key[3].ledPower = VELO.KEY_LED_MAX_POWER;
                  VELO.GR[g].key[4].ledPower = VELO.KEY_LED_MAX_POWER;
                }
                else
                {
                  VELO.GR[g].key[0].isLedDimming = OFF;
                  VELO.GR[g].key[4].isLedDimming = OFF;
                  VELO.GR[g].key[0].ledPower = 0;
                  VELO.GR[g].key[1].ledPower = 0;
                  VELO.GR[g].key[2].ledPower = 0;
                  VELO.GR[g].key[3].ledPower = 0;
                  VELO.GR[g].key[4].ledPower = 0;
                }
              }
            }
            VELO.GR[g].isCleaningPressing = false;
          }
        }
        VELO.GR[g].key[0].IsPressing = false;
        VELO.GR[g].key[1].IsPressing = false;
        VELO.GR[g].key[2].IsPressing = false;
        VELO.GR[g].key[3].IsPressing = false;
        VELO.GR[g].key[4].IsPressing = false;
        VELO.GR[g].key[5].IsPressing = false;
        VELO.GR[g].isCleaningPressing = false;
        break;
      case 0B00000001:

        if (VELO.GR[g].key[0].IsPressing == false)
        {
          VELO.GR[g].key[0].StartPressingMS = millis();
          VELO.GR[g].key[0].IsPressing = true;
        }

        break;
      case 0B00000010:

        if (VELO.GR[g].key[1].IsPressing == false)
        {
          VELO.GR[g].key[1].StartPressingMS = millis();
          VELO.GR[g].key[1].IsPressing = true;
        }

        break;
      case 0B00010000:

        if (VELO.GR[g].key[2].IsPressing == false)
        {
          VELO.GR[g].key[2].StartPressingMS = millis();
          VELO.GR[g].key[2].IsPressing = true;
        }

        break;
      case 0B00100000:

        if (VELO.GR[g].key[3].IsPressing == false)
        {
          VELO.GR[g].key[3].StartPressingMS = millis();
          VELO.GR[g].key[3].IsPressing = true;
        }

        break;
      case 0B01000000:

        if (VELO.GR[g].key[4].IsPressing == false)
        {
          VELO.GR[g].key[4].StartPressingMS = millis();
          VELO.GR[g].key[4].IsPressing = true;
        }
        if (millis() - VELO.GR[g].key[4].StartPressingMS > 5000)
        {
          if (VELO.GR[0].STATE == READY_STATE && VELO.GR[1].STATE == READY_STATE && VELO.GR[2].STATE == READY_STATE)
          {
            if (VELO.GR[0].isHotwaterDispensing == 0 && VELO.GR[1].isHotwaterDispensing == 0 && VELO.GR[2].isHotwaterDispensing == 0)
            {
              if (VELO.GR[0].isCleaning == 0 && VELO.GR[1].isCleaning == 0 && VELO.GR[2].isCleaning == 0)
              {
                if (VELO.STATE != SETUP_WATER_STATE)
                {
                  VELO.STATE = SETUP_WATER_STATE;
                  VELO.startSetupMs = millis();
                  Serial.println("setup");
                }
              }
            }
          }
        }
        break;
      case 0B10000000:

        if (VELO.GR[g].key[5].IsPressing == false)
        {
          VELO.GR[g].key[5].StartPressingMS = millis();
          VELO.GR[g].key[5].IsPressing = true;
        }

        break;
      case 0B01000001:
        if (!VELO.GR[g].isCleaningPressing)
        {
          uint32_t ms = millis();
          VELO.GR[g].key[0].StartPressingMS = ms;
          VELO.GR[g].key[0].IsPressing = false;
          VELO.GR[g].key[1].StartPressingMS = ms;
          VELO.GR[g].key[1].IsPressing = false;
          VELO.GR[g].key[2].StartPressingMS = ms;
          VELO.GR[g].key[2].IsPressing = false;
          VELO.GR[g].key[3].StartPressingMS = ms;
          VELO.GR[g].key[3].IsPressing = false;
          VELO.GR[g].key[4].StartPressingMS = ms;
          VELO.GR[g].key[4].IsPressing = false;
          VELO.GR[g].key[5].StartPressingMS = ms;
          VELO.GR[g].key[5].IsPressing = false;
          VELO.GR[g].cleaningPressingStartMS = ms;
          VELO.GR[g].isCleaningPressing = true;
        }
        break;
      default:
        uint32_t ms = millis();
        VELO.GR[g].key[0].StartPressingMS = ms;
        VELO.GR[g].key[0].IsPressing = false;
        VELO.GR[g].key[1].StartPressingMS = ms;
        VELO.GR[g].key[1].IsPressing = false;
        VELO.GR[g].key[2].StartPressingMS = ms;
        VELO.GR[g].key[2].IsPressing = false;
        VELO.GR[g].key[3].StartPressingMS = ms;
        VELO.GR[g].key[3].IsPressing = false;
        VELO.GR[g].key[4].StartPressingMS = ms;
        VELO.GR[g].key[4].IsPressing = false;
        VELO.GR[g].key[5].StartPressingMS = ms;
        VELO.GR[g].key[5].IsPressing = false;
        VELO.GR[g].cleaningPressingStartMS = ms;
        VELO.GR[g].isCleaningPressing = false;
        break;
      }
      VELO.GR[g].PINF_BUFFER = PINF;
    }

    VELO.RELAY_REGISTER &= ~(1UL << (TRANSISTOR_GROUP1_POS));
    VELO.LED_REGISTER &= ~(1UL << (TRANSISTOR_GROUP2_POS));
    VELO.LED_REGISTER &= ~(1UL << (TRANSISTOR_GROUP3_POS));
    VELO.LED_REGISTER &= ~(1UL << (0));
    VELO.LED_REGISTER &= ~(1UL << (1));
    VELO.LED_REGISTER &= ~(1UL << (2));
    VELO.LED_REGISTER &= ~(1UL << (3));
    VELO.LED_REGISTER &= ~(1UL << (4));
    VELO.LED_REGISTER &= ~(1UL << (5));
    expandio_transfer();
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
  // Serial.print("start extracting group:");
  // Serial.print(group);
  // Serial.print(" key:");
  // Serial.println(key);
  VELO.GR[group].STATE = EXTRACTING_STATE;
  VELO.GR[group].isExtracting = key + 1;
  if (VELO.STATE == SETUP_WATER_STATE)
  {
    VELO.startSetupMs = millis(); //refresh setuptime
  }
  else
  {
    VELO.GR[group].loadDataNeededForExtracting(key);
  }

  for (uint8_t z = 0; z < 5; z++)
  {
    if (z == key)
    {
      VELO.GR[group].key[key].ledPower = VELO.KEY_LED_MAX_POWER;
    }
    else
    {
      VELO.GR[group].key[z].ledPower = 0;
    }
  }
  VELO.GR[group].key[4].ledPower = VELO.KEY_LED_MAX_POWER;

  VELO.RELAY_REGISTER |= (1UL << (RELAY_PUMP));
  VELO.RELAY_REGISTER |= (1UL << (VELO.GR[group].SOLENOID_RELAY_POS));
}
void stop_extracting(uint8_t group, uint8_t key)
{
  if (VELO.GR[group].STATE == EXTRACTING_STATE)
  {
    // Serial.print("stop extracting group:");
    // Serial.print(group);
    // Serial.print(" key:");
    // Serial.println(key);
    if (VELO.STATE == SETUP_WATER_STATE)
    {
      VELO.startSetupMs = millis();
    }
    VELO.GR[group].STATE = READY_STATE;
    VELO.GR[group].isExtracting = 0;
    if (VELO.KEY_LED_NORMAL_STATE)
    {
      VELO.GR[group].key[0].ledPower = VELO.KEY_LED_MAX_POWER;
      VELO.GR[group].key[1].ledPower = VELO.KEY_LED_MAX_POWER;
      VELO.GR[group].key[2].ledPower = VELO.KEY_LED_MAX_POWER;
      VELO.GR[group].key[3].ledPower = VELO.KEY_LED_MAX_POWER;
      VELO.GR[group].key[4].ledPower = VELO.KEY_LED_MAX_POWER;
    }
    else
    {
      VELO.GR[group].key[0].ledPower = 0;
      VELO.GR[group].key[1].ledPower = 0;
      VELO.GR[group].key[2].ledPower = 0;
      VELO.GR[group].key[3].ledPower = 0;
      VELO.GR[group].key[4].ledPower = 0;
    }
    VELO.RELAY_REGISTER &= ~(1UL << (VELO.GR[group].SOLENOID_RELAY_POS));
    stop_pump();
  }
}
void start_hotwater_dispensing(uint8_t group)
{
  // Serial.print("start hotwater dispensing group:");
  // Serial.println(group);
  if (VELO.STATE == SETUP_WATER_STATE)
  {
    VELO.startSetupMs = millis();
  }
  else
  {
    for (uint8_t k = 0; k < 6; k++)
    {
      if (VELO.GR[group].key[k].Type == KEY_HOTWATER)
      {
        VELO.GR[group].NEED_HOT_WATER_TIME = VELO.GR[group].key[k].DATA_HOTWATER_DISPENSING_TIME;
      }
    }
  }
  VELO.GR[group].isHotwaterDispensing = 1;
  VELO.GR[group].hotWaterDispensingStartMS = millis();
  VELO.RELAY_REGISTER |= (1UL << (RELAY_EVTEA));
  VELO.isFillWithpump ? VELO.RELAY_REGISTER |= (1UL << (RELAY_PUMP)) : VELO.null_variable;
  VELO.GR[group].key[5].isLedDimming = DIMMING;
}
void stop_hotwater_dispensing(uint8_t group)
{
  // Serial.print("stop hotwater dispensing group:");
  // Serial.println(group);
  if (VELO.STATE == SETUP_WATER_STATE)
  {
    VELO.startSetupMs = millis();
  }
  VELO.GR[group].isHotwaterDispensing = 0;
  VELO.RELAY_REGISTER &= ~(1UL << (RELAY_EVTEA));
  VELO.GR[group].key[5].isLedDimming = OFF;
  if (VELO.KEY_LED_NORMAL_STATE == 1)
  {
    VELO.GR[group].key[5].ledPower = VELO.KEY_LED_MAX_POWER;
  }
  else
  {
    VELO.GR[group].key[5].ledPower = 0;
  }

  stop_pump();
}

void stop_pump()
{
  if (VELO.GR[0].STATE == READY_STATE && VELO.GR[1].STATE == READY_STATE && VELO.GR[2].STATE == READY_STATE)
  {
    if (VELO.isTeaWithpump)
    {
      if (VELO.GR[0].isHotwaterDispensing == 0 && VELO.GR[1].isHotwaterDispensing == 0 && VELO.GR[1].isHotwaterDispensing == 0)
      {
        if (VELO.STATE != FILLING_STATE)
        {
          VELO.RELAY_REGISTER &= ~(1UL << (RELAY_PUMP));
        }
      }
    }
    else if (VELO.STATE != FILLING_STATE)
    {
      VELO.RELAY_REGISTER &= ~(1UL << (RELAY_PUMP));
    }
  }
}