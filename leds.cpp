#include "leds.h"

CRGB leds[nLEDS];
CRGB *leds_one = &(leds[0]);
CRGB *leds_two = &(leds[nLEDS_ONE]);
CRGB *leds_three = &(leds[nLEDS_ONE + nLEDS_TWO]);
