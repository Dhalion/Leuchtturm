#pragma once

#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_INTERRUPT_RETRY_COUNT 2

#include <Arduino.h>
#define FastLED_INTERNAL
#include <FastLED.h>

#define NUM_LEDS 40 // 20 LOW + 12 TOP
#define TOP_START_AT 20
#define DATA_PIN D5

#define UNTEN_HEIGHT 4
#define UNTEN_LENGTH 5
#define OBEN_HEIGHT 3
#define OBEN_LENGTH 4


enum modus_t{ 
    m_beacon, m_strobe, m_blink, m_pulse
};

enum etage_t {oben, unten, all};

class Leuchtturm{
    public:
        Leuchtturm();   
        void setMode(modus_t mode, etage_t etage=etage_t::all);
        void setBrightness(int brightness, etage_t etage = etage_t::all);
        void setColor(CRGB color, etage_t etage = etage_t::all);
        void setLEDsActive(bool status, etage_t etage = etage_t::all);
        void handle();
        void clear(etage_t etage = etage_t::all);
    private:
        modus_t _modeOben;
        modus_t _modeUnten;
        bool    _obenActive         = false;
        bool    _untenActive        = false;
        bool    _LEDsActive         = false;
        int     _globalBrightness   = 0;
        int     _obenBrightness     = 0;
        int     _untenBrightness    = 0;
        CRGB    _obenColor          = CRGB::White;
        CRGB    _untenColor         = CRGB::White;
        CRGB    _globalColor        = CRGB::White;

        void beacon(etage_t etage);
        void strobe(etage_t etage);
        void blink(etage_t etage=etage_t::all, int onTime=500, int offTime=1000);
        void pulse(etage_t etage);
};


void setup_Leuchtturm();
void beacon_low(uint8_t hue, uint8_t steps);
void strobe_high(uint8_t hue);
void testLEDs();