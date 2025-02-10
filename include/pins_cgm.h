#pragma once

#include "pins_arduino.h"

/*
                        NANO V3.1
                    +---------------+
                    |  SWD RST SWC  |
                    |  N/C +5V GND  |
              TX----|TX          VIN|----12V
              RX----|RX          GND|----GND
               X----|RST         RST|----X
             GND----|GND          5V|----5V
            TRIP----|D2           A7|----CURRENT3_PIN
         ACLINE1----|D3           A6|----CURRENT2_PIN            VCC_PIN
         ACLINE2----|D4           A5|----SCL                        |
             CAL----|D5           A4|----SDA                        1K
      CHARGINGAC----|D6           A3|----BTN                        |
       GREEN_LED----|D7           A2|----PP_PIN-------------+--10K--+
             STP----|D8           A1|----PILOT_PIN          |       |
KEY_LOCK/VCC_PIN----|D9           A0|----CURRENT_PIN      1N4148  PP/CC
        SS/PILOT----|D10         REF|----X                  |       |
  MOSI/LOCK_PINA----|D11         3V3|----3V3               GND     GND
  MISO/LOCK_PINB----|D12         D13|----RED_LED/SCK/SLEEP_STATUS
                    |               |
                    +---------------+
*/
  // Proximity circuit Type 1
  //  VCC
  //  1K
  //  PP    Btn pressed   voltage   adcval
  //  150                 1.6v      [880, 1780)
  //  330
  //  GND

  // Proximity circuit GB/T
  //  VCC
  //  1K
  //  PP    Btn pressed   voltage   adcval
  //  680                 3.9v      [2710, 3610)
  //  2K7
  //  GND

  // Proximity circuit Type 2
  //  VCC
  //  1K
  //  PP    PP-PE closed  voltage   adcval
  //                      0.0v      [0, 450]
  //  GND

// left side pins
#define TX            D1      // PD1
#define RX            D0      // PD1
//                    RST
//                    GND
#define TRIP          D2      // PD2
#define ACLINE1       D3      // PD3
#define ACLINE2       D4      // PD4
#define CAL           D5      // PD5
#define CHARGINGAC    D6      // PD6
#define GREEN_LED     D7      // PD7
#define STP           D8      // PB0
#define VCC_PIN       D9      // PB1      KEY_LOCK
#define PILOT         D10     // PB2      SS
#define LOCK_PINA     D11     // PB3      MOSI
#define LOCK_PINB     D12     // PB4      MISO

// right side pins
//                    VIN
//                    GND
//                    RST
//                    5V
#define CURRENT3_PIN  A7      // PE3
#define CURRENT2_PIN  A6      // PE1
//#define SCL         A5
//#define SDA         A4
#define BTN           A3      // PC3
#define PP_PIN        A2      // PC2      PP/CC
#define PILOT_PIN     A1      // PC1
#define CURRENT_PIN   A0      // PC0
//                    REF
//                    3V3
#define RED_LED       D13     // PB5      SCK/SLEEP_STATUS

// used pins
#define RED_LED_REG   &PINB
#define RED_LED_IDX   5
#define GREEN_LED_REG &PIND
#define GREEN_LED_IDX 7
