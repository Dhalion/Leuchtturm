#include "leuchtturm.h"

CRGB leds[NUM_LEDS];

// *********** LEUCHTTURM CLASS FUNCTIONS ****************

// Constructor
Leuchtturm::Leuchtturm()
{
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    FastLED.setMaxPowerInMilliWatts(10000);
    FastLED.setBrightness(50);

    // Setting some default values
    this->setBrightness(60);
    this->setColor(CRGB::White);
    this->setLEDsActive(true);
}

void Leuchtturm::handle()
{
    switch (_modeOben)
    {
    case modus_t::m_pulse:
        this->pulse(etage_t::oben);
        break;
    case modus_t::m_beacon:
        this->beacon(etage_t::oben);
        break;
    case modus_t::m_blink:
        this->blink(etage_t::oben);
        break;
    case modus_t::m_strobe:
        this->strobe(etage_t::oben);
        break;
    }

    switch (_modeUnten)
    {
    case modus_t::m_beacon:
        this->beacon(etage_t::unten);
        break;
    case modus_t::m_blink:
        this->blink(etage_t::unten);
        break;
    case modus_t::m_pulse:
        this->pulse(etage_t::unten);
        break;
    case modus_t::m_strobe:
        this->strobe(etage_t::unten);
        break;
    }
    FastLED.show();
}

void Leuchtturm::setMode(modus_t mode, etage_t etage)
{
    Serial.printf("Switching Mode in Etage %d to %d\n", etage, mode);
    modus_t *_modeToSet = 0;

    switch (etage)
    {
    case etage_t::oben:
        _modeToSet = &this->_modeOben;
        break;
    case etage_t::unten:
        _modeToSet = &this->_modeUnten;
        break;
    }
    *_modeToSet = mode;
}

void Leuchtturm::setBrightness(int brightness, etage_t etage)
{
    // TODO: Only changes global Brightness!
    this->_globalBrightness = brightness;
    this->_obenBrightness = brightness;
    this->_untenBrightness = brightness;
    FastLED.setBrightness(brightness);
}

void Leuchtturm::setColor(CRGB color, etage_t etage)
{
    switch (etage)
    {
    case etage_t::all:
        this->_globalColor = color;
        this->_obenColor = color;
        this->_untenColor = color;
        break;
    case etage_t::oben:
        this->_obenColor = color;
        break;
    case etage_t::unten:
        this->_untenColor = color;
        break;
    }
}

void Leuchtturm::setLEDsActive(bool status, etage_t etage)
{
    switch (etage)
    {
    case etage_t::all:
        this->_obenActive = status;
        this->_untenActive = status;
        this->_LEDsActive = status;
        if (!status)
        {
            this->clear(etage_t::all);
        }

        break;
    case etage_t::oben:
        this->_obenActive = status;
        if (!status)
        {
            this->clear(etage_t::oben);
        }
        break;
    case etage_t::unten:
        this->_untenActive = status;
        if (!status)
        {
            this->clear(etage_t::unten);
        }
        break;
    }
}

//************** Lighting Functions *****************
void Leuchtturm::beacon(etage_t etage)
{
    CHSV _col;
    static int x = 0;
    const int max = 255;
    const int _scale = 10;

    int sin;

    if (x >= max)
    { // Reset position
        x = 0;
    }

    switch (etage)
    {
    case etage_t::all:
        _col = rgb2hsv_approximate(this->_globalColor);
        // TODO This doesn't work for now because n_Unten=5 != n_Oben=4
        break;

    case etage_t::oben:
        if (!(this->_LEDsActive || this->_untenActive)) // Skip this when LEDs disabled
        {
            return;
        }
        _col = rgb2hsv_approximate(this->_obenColor);

        // Setting actual LED values
        for (int i = TOP_START_AT; i < TOP_START_AT + OBEN_LENGTH; i++)
        {
            for (int j = 0; j < OBEN_HEIGHT; j++)
            {
                sin = sin8(x + (i * (max / OBEN_LENGTH)));

                leds[i + OBEN_LENGTH * j] = CHSV(_col.hue, _col.sat, map(_scale * sin, 0, _scale, 0, 255));
            }
        }
        break;

    case etage_t::unten:

        if ((!this->_LEDsActive || !this->_untenActive)) // Skip this when LEDs disabled
        {
            return;
        }
        _col = rgb2hsv_approximate(this->_untenColor);

        // Setting actual LED values
        for (int i = 0; i < UNTEN_LENGTH; i++)
        {
            for (int j = 0; j < UNTEN_HEIGHT; j++)
            {
                sin = sin8(x + (i * (max / UNTEN_LENGTH))) + 1;
                leds[i + UNTEN_LENGTH * j] = CHSV(_col.hue, _col.sat, map(2 * sin, 0, 2, 0, 255));
            }
        }
        break;
    }

    x++;
    // FastLED.show();
}

void Leuchtturm::clear(etage_t etage)
{
    switch (etage)
    {
    case etage_t::unten:
        for (int i = 0; i < TOP_START_AT; i++)
        {
            leds[i] = CRGB::Black;
        }
        break;
    case etage_t::oben:
        for (int i = TOP_START_AT; i < 12; i++)
        {
            leds[i] = CRGB::Black;
        }
        break;
    case etage_t::all:
        for (int i = 0; i < NUM_LEDS; i++)
        {
            leds[i] = CRGB::Black;
        }
        break;
    }
}

void Leuchtturm::strobe(etage_t etage)
{
    const int _timeOn = this->_strobeTimeOn;
    const int _timeOff = this->_strobeTimeOn;
    static long _timeCount = 0;
    static int _start = 0;
    static int _end = 0;
    static bool _status = true;

    if (_status)
    {
    }

    switch (etage)
    {
    case etage_t::oben:
        _start = TOP_START_AT;
        _end = NUM_LEDS;
        break;
    case etage_t::unten:
        _start = 0;
        _end = TOP_START_AT - 1;
        break;
    case etage_t::all:
        _start = 0;
        _end = NUM_LEDS;
        break;
    }
}

void Leuchtturm::blink(etage_t etage, int onTime, int offTime)
{
    int _start = 0;
    int _end = 0;
    CRGB _color = CRGB::Green;
    static bool _status = true;
    static long _last = millis();

    if (_status && (millis() - _last >= onTime))
    { // Currently ON
        // Turn off
        _status = false;
        _last = millis();
    }
    else if (!_status && (millis() - _last >= offTime))
    {
        // Turn on
        _status = true;
        _last = millis();
    }
    else
    {
        return; //  Do nothing until Timer runs out
    }

    switch (etage)
    {
    case etage_t::all:
        _start = 0;
        _end = NUM_LEDS;
        _color = this->_globalColor;
        break;
    case etage_t::oben:
        _start = TOP_START_AT;
        _end = NUM_LEDS;
        _color = this->_obenColor;
        break;
    case etage_t::unten:
        _start = 0;
        _end = TOP_START_AT;
        _color = this->_untenColor;
        break;
    }

    // Enable/Disable
    if (!_status)
    {
        _color = CRGB::Black;
    }

    // Set LED Values
    for (int i = _start; i < _end; i++)
    {
        leds[i] = _color;
    }
    // Write to LED
    // FastLED.show();
}

void Leuchtturm::pulse(etage_t etage)
{
    // LED related Params
    int _start = 0;
    int _end = NUM_LEDS;

    // Colors cycling
    CRGB _colorWheel[] = {CRGB::Red, CRGB::Green}; // Set of colors to cycle through
    static uint8_t _currentColor = 0;              // Keep track of current color
    static size_t _length = sizeof(_colorWheel) / sizeof(_colorWheel[0]);

    // Pulsing related parameters
    static int _step = 0;
    const int _speed = this->_pulseSpeed;
    const int _maxSteps = 255;
    static int _value = 0;

    CHSV _col = rgb2hsv_approximate(CRGB::Black);

    int _hue, _sat = 0;

    if (_step >= _maxSteps)
    {
        _step = 0;
    }

    // Set correct range of LEDs
    switch (etage)
    {
    case etage_t::unten:
        _start = 0;
        _end = TOP_START_AT;
        if (this->_changePulseColor)
        {
            _col = rgb2hsv_approximate(_colorWheel[_currentColor]);
        }
        else
        {
            _col = rgb2hsv_approximate(this->_untenColor);
        }
        break;
    case etage_t::oben:
        _start = TOP_START_AT;
        _end = NUM_LEDS;
        if (this->_changePulseColor)
        {
            _col = rgb2hsv_approximate(_colorWheel[_currentColor]);
        }
        else
        {
            _col = rgb2hsv_approximate(this->_obenColor);
        }
        break;
    case etage_t::all:
        _start = 0;
        _end = NUM_LEDS;
        if (this->_changePulseColor)
        {
            _col = rgb2hsv_approximate(_colorWheel[_currentColor]);
        }
        else
        {
            _col = rgb2hsv_approximate(this->_globalColor);
        }
        break;
    }
    // *_useColor = _col;
    // Serial.printf("Using Color Nr. %d\nCurrent Step: %d\n\nValue:%d\n\n", _currentColor, _step, _value);
    // Serial.printf("Color set: h:%d,s:%d,v:%d\n", _col.h, _col.s, _col.v);

    // Change colors stuff
    if (_value <= 2)
    {
        // Serial.printf("Value:%d\nColor:%d\n", _value, _currentColor);
        _currentColor++;
    }

    if (_currentColor >= _length)
    {
        // Serial.printf("Resetting Color.\n");
        _currentColor = 0;
    }

    // Create Sine value for current step and map to 0-255
    _value = sin8(_step);

    // Write LED Values
    for (int i = _start; i < _end; i++)
    {
        leds[i] = CHSV(_col.h, _col.s, _value);
    }
    // Push to LED strip
    // FastLED.show();

    _step += _speed;
}

void Leuchtturm::setParams(int strobeOnTime, int strobeOffTime, int pulseSpeed)
{
    this->_strobeTimeOn = strobeOnTime;
    this->_strobeTimeOff = strobeOffTime;
    this->_pulseSpeed = pulseSpeed;
    return;
}
