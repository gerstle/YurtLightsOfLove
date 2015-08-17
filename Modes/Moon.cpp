#include "Moon.h"

Moon::Moon() {
    _name = "moon";
    _skyColor = CHSV(164, 255, 200);
    _moonColor = CHSV(160, 0, 255);
    _lastChange = millis();
    _delay = 125;

    perlins = new Perlins();
    _valueTracker = random16();
}

Moon::~Moon() {
}

void Moon::frame()
{
    moon();
    sky();
}

void Moon::moon()
{
    unsigned long currentTime = millis();
    if (currentTime <= _lastChange + _delay)
        return;

    _lastChange = currentTime;

    int dice = 0;
    CHSV tmp;
	for (int i = 0; i < nLEDS_THREE; i++)
	{
	    dice = random8();

	    if ((dice <= 10) && (_delay > 0))
            tmp = CHSV(_moonColor.hue, _moonColor.sat, random8(200, 255));
	    else
	        tmp = _moonColor;
	    leds_three[i] = tmp;
	}
}

void Moon::sky()
{
    double value;
    _valueTracker += 0.0004;
	for (int i = 0; i < (nLEDS_ONE + nLEDS_TWO); i++)
	{
        _valueTracker += 0.00005;
        value = perlins->pnoise(_valueTracker + sin((i + _valueTracker) / 2) , cos(_valueTracker), _valueTracker);
	    leds[i] = CHSV(_skyColor.hue, _skyColor.sat, (value * (double)127) + 128);
        //leds[i] = _skyColor;
	}
}
