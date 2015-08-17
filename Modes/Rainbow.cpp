/*
 * Rainbow.cpp
 *
 *  Created on: Aug 16, 2015
 *      Author: cgerstle
 */

#include "Rainbow.h"

Rainbow::Rainbow() {
    _name = "rainbow";
    _lastChangeTime = millis();
    _lastStartHue = 0;
    _increment = (float)255/(float) (nLEDS_ONE + nLEDS_TWO);
    _delay = 60;

    _lastTopChangeTime = millis();
    _lastTopStartHue = 0;
    _topIncrement = (float)255/(float)nLEDS_THREE;
    _topDelay = 8;
}

Rainbow::~Rainbow() {
}

void Rainbow::frame()
{
    unsigned long currentTime = millis();
    if (currentTime > (_lastChangeTime + _delay))
    {
        _lastChangeTime = currentTime;

        for (int i = 0; i < nLEDS_ONE; i++)
            leds_one[i] = CHSV(_lastStartHue + (((float)i) * _increment), 255, 255);
        for (int i = 0; i < nLEDS_TWO; i++)
            leds_two[nLEDS_TWO - i - 1] = CHSV(_lastStartHue + (((float)(i + nLEDS_ONE)) * _increment), 255, 255);

        // <gerstle> let it roll over
        _lastStartHue++;
    }

    if (currentTime > (_lastTopChangeTime + _topDelay))
    {
        _lastTopChangeTime = currentTime;

        for (int i = 0; i < nLEDS_THREE; i++)
            leds_three[i] = CHSV(_lastTopStartHue + (((float)i) * _topIncrement), 255, 255);

        // <gerstle> let it roll over
        _lastTopStartHue++;
    }
}
