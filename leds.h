#ifndef _leds_h
#define _leds_h

#include "FastLED.h"

#define nLEDS_ONE 100 // melt has an assumption that no strips are bigger than this.
#define nLEDS_TWO 100
#define nLEDS_THREE 50
#define nLEDS nLEDS_ONE + nLEDS_TWO + nLEDS_THREE

#define LED_DEFAULT_BRIGHTNESS 100

extern CRGB leds[];
extern CRGB *leds_one;
extern CRGB *leds_two;
extern CRGB *leds_three;

//#define led_init() FastLED.addLeds<NEOPIXEL, 2>(__fast_led_strip, nLEDs, 0)
#define led_init() FastLED.addLeds<P9813, 6, 7, RGB, DATA_RATE_MHZ(15)>(leds_one, nLEDS_ONE, 0); \
    FastLED.addLeds<P9813, 8, 9, RGB, DATA_RATE_MHZ(15)>(leds_two, nLEDS_TWO, 0); \
    FastLED.addLeds<P9813, 10, 11, RGB, DATA_RATE_MHZ(15)>(leds_three, nLEDS_THREE, 0)
#define led_show() FastLED.show()
#define led_print(n) Serial.printf("RGB(%d, %d, %d)\n", leds[n].r, leds[n].g, leds[n].b)
#define hsv_print(color) Serial.printf("HSV(%d, %d, %d)\n", color.hue, color.sat, color.val)

#endif
