

#include "leuchtturm.h"
#include "wifi.h"

Leuchtturm lt;

void setup()
{
  Serial.begin(115200);
  lt.setMode(modus_t::m_pulse, etage_t::oben);
  lt.setMode(modus_t::m_beacon, etage_t::unten);

  lt.setBrightness(50);
  lt.setColor(CRGB::White, etage_t::unten);
  lt.setColor(CRGB::Green, etage_t::oben);

}

void loop()
{
  lt.handle();
  delay(20);
  // testLEDs();

    // Serial.println("Changing color");
  
  // EVERY_N_MILLISECONDS(20) beacon_low(240,1);
  // EVERY_N_MILLISECONDS(1000) strobe_high(100);
  // beacon(20, 240, 1);
}
