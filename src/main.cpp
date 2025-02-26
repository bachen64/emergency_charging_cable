#include "open_evse.h"

#include <WDT.h>

#define TEXTIFY(A) #A
#define ESCAPEQUOTE(A) TEXTIFY(A)

uint8_t  g_PlugType;        // for BtnHandler::ChkBtn() in open_evse/main.cpp
uint8_t  g_CircuitRating;   // for BtnHandler::ChkBtn() in open_evse/main.cpp
uint8_t  g_ContinuousLoad;  // for J1772EVSEController::GetMaxCurrentCapacity() and J1772EVSEController::SetCurrentCapacity()
uint16_t g_ButtonPressed;   // for Btn::read() in open_evse/main.cpp

static uint8_t evseState = EVSE_STATE_UNKNOWN;

int getChar(FILE *fp)
{
  while (!Serial.available());
  return Serial.read();
}

int putChar(char c, FILE *fp)
{
  if (c == '\n') putChar((char)'\r', fp);
  Serial.write(c);
  return 0;
}

void setup()
{
  Serial.begin(115200);
  fdevopen(putChar, getChar);

#if defined(ECC_GB_T)
  // GB/T 16A domestic: 0, 500,  8A/16A
  // GB/T 16A CEE blue: 0, 500, 16A/16A
  // GB/T 32A CEE blue: 0, 500,  8A/16A/20A/32A
  g_PlugType = 0;
  #if defined(ECC_32A)
    g_CircuitRating = 32;
    g_ContinuousLoad = 32;
  #else
    g_CircuitRating = 16;
    #if defined(ECC_16A_DOMESTIC)
      g_ContinuousLoad = 8;
    #else
      g_ContinuousLoad = 16;
    #endif
  #endif
  g_ButtonPressed = 500;
#elif defined(ECC_TYPE_2)
  // Type 2 16A domestic: 0, 1800, 10A/16A
  // Type 2 16A CEE blue: 0, 1800, 16A/16A
  // Type 2 32A CEE blue: 0, 1800,  8A/16A/20A/32A
  g_PlugType = 2;
  #if defined(ECC_32A)
    g_CircuitRating = 32;
    g_ContinuousLoad = 32;
  #else
    g_CircuitRating = 16;
    #if defined(ECC_16A_DOMESTIC)
      g_ContinuousLoad = 10; // EU: 10A
    #else
      g_ContinuousLoad = 16;
    #endif
  #endif
  g_ButtonPressed = 1800;
#else // ECC_TYPE_1
  // Type 1 16A domestic: 0, 1200, 13A/16A
  // Type 1 16A CEE blue: 0, 1200, 16A/16A
  // Type 1 32A CEE blue: 0, 1200,  8A/16A/20A/32A
  g_PlugType = 1;
  #if defined(ECC_32A)
    g_CircuitRating = 32;
    g_ContinuousLoad = 32;
  #else
    g_CircuitRating = 16;
    #if defined(ECC_16A_DOMESTIC)
      g_ContinuousLoad = 13; // US: a continuous load = 80% of the circuit rating
    #else
      g_ContinuousLoad = 16;
    #endif
  #endif
  g_ButtonPressed = 1200;
#endif
  printf("OpenEVSE based Emergency Charging Cable %s, %dA ", g_PlugType == 0 ? "GB/T" : g_PlugType == 2 ? "Type 2" : "Type 1", g_CircuitRating);
  if ((g_CircuitRating == 32) || (g_CircuitRating == 16 && g_ContinuousLoad == 16)) {
    printf("CEE blue, adjustable ");
  }
  if (g_CircuitRating == 32) {
    printf("8A/16A/20A/%uA\n", g_CircuitRating);
  }
  else if (g_CircuitRating == 16 && g_ContinuousLoad == 16) {
    printf("%uA/%uA\n", g_PlugType == 0 ? 8 : g_PlugType == 2 ? 10 : 13, g_CircuitRating);
  }
  else if (g_CircuitRating == 16 && g_ContinuousLoad < 16) {
    printf("domestic, fixed %uA\n", g_ContinuousLoad);
  }
  printf("Build enviroment: %s\n", ESCAPEQUOTE(BUILD_ENV_NAME));
  printf("Build date: %s %s\n", __DATE__, __TIME__);

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
