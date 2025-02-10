#include "open_evse.h"

#include <WDT.h>

uint8_t  g_PlugType;    // for main.cpp, J1772EVSEController::GetMaxCurrentCapacity() and J1772EVSEController::SetCurrentCapacity()
uint8_t  g_PlugAmps;    // for main.cpp, BtnHandler::ChkBtn() in open_evse/main.cpp
uint16_t g_BtnPressed;  // for Btn::read() in open_evse/main.cpp

static uint8_t evseState = EVSE_STATE_UNKNOWN;

void setup()
{
  Serial.begin(115200);

#if defined(ECC_GB_T)
  g_PlugType = 0;
#if defined(ECC_32A)
  g_PlugAmps = 32;
  g_BtnPressed = 900;
#else
  g_PlugAmps = 16;
  g_BtnPressed = 1800;
#endif

#elif defined(ECC_TYPE_2)
  g_PlugType = 2;
#if defined(ECC_32A)
  g_PlugAmps = 32;
#else // 
  g_PlugAmps = 16;
#endif // ECC_32A
  g_BtnPressed = 1800;

#else // ECC_TYPE_1
  g_PlugType = 1;
#if defined(ECC_32A)
  g_PlugAmps = 32;
#else
  g_PlugAmps = 16;
#endif
  g_BtnPressed = 1200;
#endif
  Serial.print(F("OpenEVSE based Emergency Charging Cable "));
  Serial.print(g_PlugType == 0 ? F("GB/T") : g_PlugType == 1 ? F("Type 1") : F("Type 2"));
  Serial.print(F(", "));
  Serial.print(g_PlugAmps);
  Serial.print(F("A "));
  if (g_PlugAmps == 16) {
#if defined(ECC_16A_DOMESTIC)
    Serial.print(F("domestic"));
#else
    Serial.print(F("CEE blue"));
#endif
  }
  Serial.println(F(" plug"));

  open_evse_setup();
}

void loop()
{
  uint8_t state = g_EvseController.GetState();
  if (evseState != state) { // state transition
    evseState = state;
    if (EVSE_STATE_A == state) {
      pinMode(VCC_PIN, OUTPUT);
      digitalWrite(VCC_PIN, HIGH);
    } else {
      digitalWrite(VCC_PIN, LOW);
      pinMode(VCC_PIN, INPUT);
    }
  }

  open_evse_loop();
}
