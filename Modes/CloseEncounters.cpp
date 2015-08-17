#include "CloseEncounters.h"

CloseEncounters::CloseEncounters() {
    _name = "closeEncounters";
    _stripeHSV = CHSV(20, 150, 150);
    _blackHSV = CHSV(0, 0, 0);
    _frame = 0;
    _startStage = 0;
    _lastNewBarCheckTime = millis();
    _lastBlueChange = millis();
    _blueDelay = 125;
    _lastNewDotCheckTime = millis();
}

CloseEncounters::~CloseEncounters() {
	for(std::vector<Bar*>::iterator i = _colorBars.begin(); i != _colorBars.end(); ++i )
	{
    	delete *i;
	}
	_colorBars.clear();
}

void CloseEncounters::frame() {
    stripes();
//	for (uint8_t i = 0; i < nLEDS; i++)
//	    leds[i] = CHSV(0, 0, 0);
    colors();
    blue();
}

void CloseEncounters::stripes() {
    byte stage = _startStage;
    for (int i = 0; i < nLEDS_ONE + nLEDS_TWO; i++)
        paintPixel(&stage, i, false);

    stage = _startStage;
    for (int i = nLEDS_ONE + nLEDS_TWO - 1; i >= 0; i--)
        paintPixel(&stage, i, true);

    _frame++;
    if (_frame >= nSTRIPE_FRAMES)
    {
        _frame = 0;
        if (_startStage <= 0)
            _startStage = (STAGE_COUNT - 1);
        else
            _startStage--;
    }
}

void CloseEncounters::colors()
{
    random16_add_entropy(random());

    if ((_lastNewBarCheckTime + 1600) < millis())
    {
        _lastNewBarCheckTime = millis();
        if ((random8() < 140) && (_colorBars.size() < 4))
//        if (_colorBars.size() < 1)
        {
            Bar *newBar = new Bar;
            newBar->color = CHSV(random8(), 255, 255);
            newBar->index = random8(0, nLEDS_ONE + nLEDS_TWO);
            newBar->width = random(10, 25);
            newBar->lifeLength = newBar->width * 4.5 * nCOLOR_FRAMES;
            newBar->birth = millis();
            newBar->frame = 0;
            Serial.printf("created bar w/ width: %d lifeLength: %d\r\n", newBar->width, newBar->lifeLength);

            _colorBars.push_back(newBar);
        }
    }

	for(std::vector<Bar*>::iterator i = _colorBars.begin(); i != _colorBars.end();)
	{
	    if ((*i)->frame >= (*i)->lifeLength)
        {
	        Serial.printf("deleting bar w/ lifeLength %d\r\n", (*i)->lifeLength);
            delete *i;
            i = _colorBars.erase(i);
	        continue;
        }

        CRGB tmp;
        int j;
        uint8_t fullPixelCount;
	    if ((*i)->lifeLength - (*i)->frame <= nCOLOR_FRAMES * (*i)->width)
	    {
	        // <cgerstle> all fade out at once...
//	        Serial.printf("%d\t%d\r\n", nCOLOR_FRAMES - ((*i)->lifeLength - (*i)->frame), nCOLOR_FRAMES);
//            setTransitionColor(&(tmp), nCOLOR_FRAMES - ((*i)->lifeLength - (*i)->frame), nCOLOR_FRAMES, (*i)->color, _blackHSV);
//	        for (int j = (*i)->index - (*i)->width / 2; (j >= 0) && (j < nLEDS_ONE + nLEDS_TWO) && (j < (*i)->index + (*i)->width / 2); j++)
//                leds[j] += tmp;

            int frame = (*i)->lifeLength - (*i)->frame;
            fullPixelCount = frame/ nCOLOR_FRAMES; //(*i)->frame / nCOLOR_FRAMES;
            Serial.printf("%d\t%d\r\n", frame, fullPixelCount);
            for (j = (*i)->index; (j < nLEDS_ONE + nLEDS_TWO) && (j < ((*i)->index + ((*i)->width / 2))) && (j < (*i)->index + fullPixelCount); j++)
                leds[j] += (*i)->color;

            if ((frame % nCOLOR_FRAMES != 0) && (j < nLEDS_ONE + nLEDS_TWO) && (j < ((*i)->index + ((*i)->width / 2))))
            {
                setTransitionColor(&(tmp), frame % nCOLOR_FRAMES, nCOLOR_FRAMES, _blackHSV, (*i)->color);
                leds[j] += tmp;
            }

            for (j = (*i)->index; (j >= 0) && (j > ((*i)->index - ((*i)->width / 2))) && (j > (*i)->index - fullPixelCount); j--)
                leds[j] += (*i)->color;

            if ((frame % nCOLOR_FRAMES!= 0) && (j < (*i)->index) && (j >= 0) && (j > ((*i)->index - ((*i)->width / 2))))
            {
                setTransitionColor(&(tmp), frame % nCOLOR_FRAMES, nCOLOR_FRAMES, _blackHSV, (*i)->color);
                    leds[j] += tmp;
            }
	    }
	    else
	    {
            fullPixelCount = (*i)->frame / nCOLOR_FRAMES;
            for (j = (*i)->index; (j < nLEDS_ONE + nLEDS_TWO) && (j < ((*i)->index + ((*i)->width / 2))) && (j < (*i)->index + fullPixelCount); j++)
                leds[j] += (*i)->color;

            if (((*i)->frame % nCOLOR_FRAMES != 0) && (j < nLEDS_ONE + nLEDS_TWO) && (j < ((*i)->index + ((*i)->width / 2))))
            {
                setTransitionColor(&(tmp), (*i)->frame % nCOLOR_FRAMES, nCOLOR_FRAMES, _blackHSV, (*i)->color);
                leds[j] += tmp;
            }

            for (j = (*i)->index; (j >= 0) && (j > ((*i)->index - ((*i)->width / 2))) && (j > (*i)->index - fullPixelCount); j--)
                leds[j] += (*i)->color;

            if (((*i)->frame % nCOLOR_FRAMES != 0) && (j < (*i)->index) && (j >= 0) && (j > ((*i)->index - ((*i)->width / 2))))
            {
                setTransitionColor(&(tmp), (*i)->frame % nCOLOR_FRAMES, nCOLOR_FRAMES, _blackHSV, (*i)->color);
                leds[j] += tmp;
            }
	    }

        (*i)->frame += 1;
	    ++i;
	}
}

void CloseEncounters::blue()
{
    unsigned long currentTime = millis();
    if (currentTime <= _lastBlueChange + _blueDelay)
        return;

    _lastBlueChange = currentTime;

    int dice = 0;
    CHSV tmp;
	for (int i = 0; i < nLEDS_THREE; i++) {
	    dice = random8();

	    if ((dice <= 5) && (_blueDelay > 0))
            tmp = CHSV(160, 255, random8(20, 23));
	    else
	        tmp = CHSV(160, 255, 23);
	    leds_three[i] = tmp;
	}

    if ((_lastNewDotCheckTime + 1000) < millis())
    {
        Serial.println("checking");
        _lastNewDotCheckTime = millis();
        if ((random8() < 180) && (_dots.size() < 18))
//        if (_dots.size() < 1)
        {
            BlueDot *newDot = new BlueDot;
            if (random8() < 85)
               newDot->color = CHSV(0, random8(127, 255), 6);
            else
               newDot->color = CHSV(160, 255, 6);
            newDot->index = random8(0, nLEDS_THREE);
            newDot->goingDown = false;
            newDot->speed = random8(5, 12);

            _dots.push_back(newDot);
        }
    }

	for(std::vector<BlueDot*>::iterator i = _dots.begin(); i != _dots.end();)
	{
	    if ((*i)->color.val >= 250)
	    {
	        Serial.printf("val >= 250: %d\r\n", (*i)->color.val);
	        (*i)->goingDown = true;
	    }

	    if ((*i)->color.val <= 5)
	    {
	        Serial.println("deleting dot");
            delete *i;
            i = _dots.erase(i);
	        continue;
	    }

	    if ((*i)->goingDown)
	    {
	        Serial.printf("subtracting %d to val of %d = ", (*i)->speed, (*i)->color.val);
	        (*i)->color.val -= (*i)->speed;
	    }
	    else
	    {
	        Serial.printf("adding %d to val of %d = ", (*i)->speed, (*i)->color.val);
	        (*i)->color.val = qadd8((*i)->color.val, (*i)->speed);
	    }
	    Serial.printf("%d\r\n", (*i)->color.val);

	    leds_three[(*i)->index] = CHSV((*i)->color.hue, (*i)->color.sat, (*i)->color.val);

	    ++i;
	}
}

void CloseEncounters::paintPixel(byte (*stage), int index, bool add)
{
    CRGB tmpPixel;
    if ((*stage) == 0)
    {
        setTransitionColor(&(tmpPixel), _frame, nSTRIPE_FRAMES, _blackHSV, _stripeHSV);
        if (add)
            leds[index] += tmpPixel;
        else
            leds[index] = tmpPixel;
    }
    else if (((*stage) >= 1) && ((*stage) <= 9))
    {
        if (!add)
        leds[index] = _blackHSV;
    }
    else if ((*stage) == 10)
    {
        setTransitionColor(&(tmpPixel), _frame, nSTRIPE_FRAMES, _stripeHSV, _blackHSV);
        if (add)
            leds[index] += tmpPixel;
        else
            leds[index] = tmpPixel;
    }
    else if ((*stage) == 11)
    {
        if (add)
            leds[index] += _stripeHSV;
        else
            leds[index] = _stripeHSV;
    }

    (*stage)++;
    if ((*stage) >= STAGE_COUNT)
        (*stage) = 0;
}

void CloseEncounters::setTransitionColor(CRGB* led, float numerator, float denomenator, CHSV fromColor, CHSV toColor)
{
    CRGB from = fromColor;
    CRGB to = toColor;

    CloseEncounters::setTransitionColor(led, numerator, denomenator, from, to);
}

void CloseEncounters::setTransitionColor(CRGB* led, float numerator, float denomenator, CRGB fromColor, CRGB toColor)
{
    if (numerator <= 0)
        *led = fromColor;
    else if (numerator >= denomenator)
        *led = toColor;
    else
    {
        CRGB toDelta = toColor;
        toDelta %= (byte)(255 * ((float)numerator/(float)denomenator));

        CRGB fromDelta = fromColor;
        fromDelta %= (byte)(255 * ((float)(denomenator - numerator)/(float)denomenator));

        *led = CRGB::Black;
        *led += fromDelta;
        *led += toDelta;
    }
}
