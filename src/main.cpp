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
  VELO.init(10);
  Serial.begin(115200);
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
  if (millis() - ms_now >= WATER_LEVEL_CHECK_INTERVAL)
  {
   // led_Run_state ? TXLED0 : TXLED1;
    led_Run_state = !led_Run_state;
    VELO.analogWaterLevelMin = analogRead(WATER_LEVEL_MIN_PIN);
    VELO.analogWaterLevelMax = analogRead(WATER_LEVEL_MAX_PIN);

    if (VELO.analogWaterLevelMax > 300)
    {
      VELO.RELAY_REGISTER |= (1UL << (RELAY_PUMP));
      VELO.RELAY_REGISTER |= (1UL << (RELAY_EVFILL));
      VELO.RELAY_REGISTER &= ~(1UL << (RELAY_RISC));
      VELO.STATE = FILLING_STATE;
      VELO.FillingUpFlag = false;
    }
    else
    {
      if (VELO.FillingUpFlag == false)
      {
        VELO.FillingUpFlag = true;
        VELO.FillingUpStartMs = millis();
        // Serial.print("water level min:");
        // Serial.println(VELO.analogWaterLevelMin);
        // Serial.print("water level max:");
        // Serial.println(VELO.analogWaterLevelMax);
      }
    }
    if (VELO.FillingUpFlag)
    {
      if (millis() - VELO.FillingUpStartMs >= WATER_LEVEL_STILL_PUMP_AFTER)
      {
        VELO.STATE = READY_STATE;
        VELO.RELAY_REGISTER |= (1UL << (RELAY_RISC));
        VELO.LED_REGISTER &= ~(1UL << (RELAY_EVFILL));
        stop_pump();
      }
    }
    ms_now = millis();
  }
  for (uint8_t g = 0; g < 3; g++)
  {
    *(uint8_t *)VELO.GR[g].TRANSISTOR_REG |= (1UL << (VELO.GR[g].TRANSISTOR_POS));

    (VELO.GR[g].led_power_count_now > 21 && VELO.GR[g].led_power_count_now > 0) ? VELO.GR[g].led_power_count_now-- : (VELO.GR[g].led_power_count_now < 21 && VELO.GR[g].led_power_count_now >= 0) ? VELO.GR[g].led_power_count_now++
                                                                                                                                                                                                  : VELO.GR[g].led_power_count_now = 0;
    VELO.GR[g].key[0].ledPower > VELO.GR[g].led_power_count_now ? VELO.LED_REGISTER |= (1UL << (0)) : VELO.LED_REGISTER &= ~(1UL << (0));
    VELO.GR[g].key[1].ledPower > VELO.GR[g].led_power_count_now ? VELO.LED_REGISTER |= (1UL << (1)) : VELO.LED_REGISTER &= ~(1UL << (1));
    VELO.GR[g].key[2].ledPower > VELO.GR[g].led_power_count_now ? VELO.LED_REGISTER |= (1UL << (2)) : VELO.LED_REGISTER &= ~(1UL << (2));
    VELO.GR[g].key[3].ledPower > VELO.GR[g].led_power_count_now ? VELO.LED_REGISTER |= (1UL << (3)) : VELO.LED_REGISTER &= ~(1UL << (3));
    VELO.GR[g].key[4].ledPower > VELO.GR[g].led_power_count_now ? VELO.LED_REGISTER |= (1UL << (4)) : VELO.LED_REGISTER &= ~(1UL << (4));
    VELO.GR[g].key[5].ledPower > VELO.GR[g].led_power_count_now ? VELO.LED_REGISTER |= (1UL << (5)) : VELO.LED_REGISTER &= ~(1UL << (5));
    expandio_transfer();
    _delay_us(50);
    if (PINF != VELO.GR[g].PINF_BUFFER)
    {
      Serial.print("GR[");
      Serial.print(g);
      Serial.print("]:");
      Serial.println(PINF, BIN);
      switch (PINF)
      {
      case 0B00000000:
        for (uint8_t k = 0; k < 6; k++)
        {
          if (VELO.GR[g].key[k].IsPressing && millis() - VELO.GR[g].key[k].StartPressingMS >= 120)
          {
            Serial.print(millis() - VELO.GR[g].key[k].StartPressingMS);
            Serial.println("ms");

            if (VELO.GR[g].key[k].Type == KEY_HOTWATER)
            {
              if (VELO.GR[g].isHotwaterDispensing)
              {
                start_hotwater_dispensing(g);
              }
              else
              {
                stop_hotwater_dispensing(g);
              }
            }
            else if (VELO.GR[g].STATE == READY_STATE)
            {
              start_extracting(g, k);
            }
            else if (VELO.GR[g].isExtracting == k || k == 4)
            {
              stop_extracting(VELO.GR[g].GroupNum, VELO.GR[g].isExtracting);
            }
          }
        }

        (VELO.GR[g].isCleaningPressing && millis() - VELO.GR[g].cleaningPressingStartMS >= 120 && VELO.GR[g].STATE == READY_STATE) ? VELO.GR[g].STATE = CLEANING_SATE : VELO.null_variable = 0;
        (VELO.GR[g].isCleaningPressing && millis() - VELO.GR[g].cleaningPressingStartMS >= 120 && VELO.GR[g].STATE == CLEANING_SATE) ? VELO.GR[g].STATE = READY_STATE : VELO.null_variable = 0;
        VELO.GR[g].key[0].IsPressing = false;
        VELO.GR[g].key[1].IsPressing = false;
        VELO.GR[g].key[2].IsPressing = false;
        VELO.GR[g].key[3].IsPressing = false;
        VELO.GR[g].key[4].IsPressing = false;
        VELO.GR[g].key[5].IsPressing = false;
        VELO.GR[g].isCleaningPressing = false;
        break;
      case 0B00000001:
        if (!VELO.GR[g].key[0].IsPressing)
        {
          VELO.GR[g].key[0].StartPressingMS = millis();
          VELO.GR[g].key[0].IsPressing = true;
        }

        break;
      case 0B00000010:
        if (!VELO.GR[g].key[1].IsPressing)
        {
          VELO.GR[g].key[1].StartPressingMS = millis();
          VELO.GR[g].key[1].IsPressing = true;
        }

        break;
      case 0B00010000:
        if (!VELO.GR[g].key[2].IsPressing)
        {
          VELO.GR[g].key[2].StartPressingMS = millis();
          VELO.GR[g].key[2].IsPressing = true;
        }

        break;
      case 0B00100000:
        if (!VELO.GR[g].key[3].IsPressing)
        {
          VELO.GR[g].key[3].StartPressingMS = millis();
          VELO.GR[g].key[3].IsPressing = true;
        }

        break;
      case 0B01000000:
        if (!VELO.GR[g].key[4].IsPressing)
        {
          VELO.GR[g].key[4].StartPressingMS = millis();
          VELO.GR[g].key[4].IsPressing = true;
        }

        break;
      case 0B10000000:
        if (!VELO.GR[g].key[5].IsPressing)
        {
          VELO.GR[g].key[5].StartPressingMS = millis();
          VELO.GR[g].key[5].IsPressing = true;
        }

        break;
      case 0B01000001:
        if (!VELO.GR[g].isCleaningPressing)
        {
          VELO.GR[g].key[0].StartPressingMS = millis();
          VELO.GR[g].key[0].IsPressing = false;
          VELO.GR[g].key[1].StartPressingMS = millis();
          VELO.GR[g].key[1].IsPressing = false;
          VELO.GR[g].key[2].StartPressingMS = millis();
          VELO.GR[g].key[2].IsPressing = false;
          VELO.GR[g].key[3].StartPressingMS = millis();
          VELO.GR[g].key[3].IsPressing = false;
          VELO.GR[g].key[4].StartPressingMS = millis();
          VELO.GR[g].key[4].IsPressing = false;
          VELO.GR[g].key[5].StartPressingMS = millis();
          VELO.GR[g].key[5].IsPressing = false;
          VELO.GR[g].cleaningPressingStartMS = millis();
          VELO.GR[g].isCleaningPressing = true;
        }

        break;
      default:
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
    _delay_us(100);
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
  VELO.GR[group].STATE = EXTRACTING_STATE;
  VELO.GR[group].isExtracting = key+1;
  VELO.GR[group].loadDataNeededForExtracting(key);
  VELO.GR[group].key[0].Num == key ? VELO.GR[group].key[0].ledPower = VELO.KEY_LED_MAX_POWER : VELO.GR[group].key[0].ledPower = 0;
  VELO.GR[group].key[1].Num == key ? VELO.GR[group].key[1].ledPower = VELO.KEY_LED_MAX_POWER : VELO.GR[group].key[1].ledPower = 0;
  VELO.GR[group].key[2].Num == key ? VELO.GR[group].key[2].ledPower = VELO.KEY_LED_MAX_POWER : VELO.GR[group].key[2].ledPower = 0;
  VELO.GR[group].key[3].Num == key ? VELO.GR[group].key[3].ledPower = VELO.KEY_LED_MAX_POWER : VELO.GR[group].key[3].ledPower = 0;
  VELO.GR[group].key[4].Num == key ? VELO.GR[group].key[4].ledPower = VELO.KEY_LED_MAX_POWER : VELO.GR[group].key[4].ledPower = 0;
  VELO.GR[group].key[4].ledPower = VELO.KEY_LED_MAX_POWER;
  
  VELO.RELAY_REGISTER |= (1UL << (RELAY_PUMP));
  VELO.RELAY_REGISTER |= (1UL << (VELO.GR[group].SOLENOID_RELAY_POS));
}
void stop_extracting(uint8_t group, uint8_t key)
{
  if (VELO.GR[group].STATE == EXTRACTING_STATE)
  {
    Serial.print("stop extracting group:");
    Serial.print(group);
    Serial.print(" key:");
    Serial.println(key);
    VELO.GR[group].STATE = READY_STATE;
    VELO.GR[group].isExtracting=0;
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
  Serial.print("start hotwater dispensing group:");
  Serial.println(group);
  VELO.GR[group].isHotwaterDispensing = 1;
  VELO.GR[group].hotWaterDispensingStartMS = millis();
  VELO.RELAY_REGISTER |= (1UL << (RELAY_EVTEA));
  VELO.isFillWithpump ? VELO.RELAY_REGISTER |= (1UL << (RELAY_PUMP)) : VELO.null_variable;
}
void stop_hotwater_dispensing(uint8_t group)
{
  Serial.print("stop hotwater dispensing group:");
  Serial.println(group);
  VELO.GR[group].isHotwaterDispensing = 0;
  VELO.RELAY_REGISTER &= ~(1UL << (RELAY_EVTEA));
  stop_pump();
}
void led_dimming_control()
{
  if (VELO.GR[0].isLedTopDimming)
  {
    VELO.GR[0].led_dimming_now--;
    if (VELO.GR[0].led_dimming_now <= 0)
    {
      VELO.GR[0].isLedTopDimming = false;
    }
  }
  else
  {
    VELO.GR[0].led_dimming_now++;
    if (VELO.GR[0].led_dimming_now >= 10)
    {
      VELO.GR[0].isLedTopDimming = true;
    }
  }
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