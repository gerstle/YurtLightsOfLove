#include "MeltMode.h"

MeltMode::MeltMode() {
    hueTracker = random16();
    valueTracker = random16();

    threeHueTracker = random16();
    threeValueTracker = random16();

    perlins = new Perlins();
}

MeltMode::~MeltMode() {
    delete perlins;
}

void MeltMode::frame() {
    double hue, value;
    int j = 0;
    hueTracker += 0.001;
    valueTracker += 0.001;
    for (j = 0; j < nLEDS_ONE; j++)
    {
        valueTracker += 0.0001;
        hueTracker += 0.00001;

        value = perlins->pnoise(valueTracker + sin((j + valueTracker) / 2) , cos(valueTracker), valueTracker);
        hue = perlins->pnoise(cos(hueTracker / 2.0) + sin((j + hueTracker) / 10.0) , cos(hueTracker / 5.0), hueTracker);

        leds_one[j] = CHSV((hue * (double)127) + 128, 255, map((value * (double)127) + 128, 0, 255, 100, 255));
        if (j < nLEDS_TWO)
            leds_two[j] = leds[j];
    }

    threeHueTracker += 0.01;
    threeValueTracker += 0.008;
    for (j = 0; j < nLEDS_THREE; j++)
    {
        threeValueTracker += 0.001;
        threeHueTracker += 0.0001;

        value = perlins->pnoise(threeValueTracker + sin((j + threeValueTracker) / 2) , cos(threeValueTracker), threeValueTracker);
        hue = perlins->pnoise(cos(threeHueTracker / 2.0) + sin((j + threeHueTracker) / 10.0) , cos(threeHueTracker / 5.0), threeHueTracker);

        leds_three[j] = CHSV((hue * (double)127) + 128, 255, map((value * (double)127) + 128, 0, 255, 100, 255));
    }
}
