

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

}
