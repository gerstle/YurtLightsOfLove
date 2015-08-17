#ifndef CLOSEENCOUNTERS_H_
#define CLOSEENCOUNTERS_H_

#include "BaseMode.h"
#include <vector>

#define STAGE_COUNT 12
#define nSTRIPE_FRAMES 160
#define nCOLOR_FRAMES 8

struct Bar {
    CHSV color;
    uint8_t index;
    uint8_t width;
    uint16_t lifeLength;
    unsigned long birth;
    uint16_t frame;
};

struct BlueDot {
    CHSV color;
    uint8_t index;
    boolean goingDown;
    uint8_t speed;
};

class CloseEncounters: public BaseMode {
public:
    CloseEncounters();
    virtual ~CloseEncounters();

public:
    virtual void frame();
    virtual const char* name() { return _name; }

private:
    const char *_name;

    byte _startStage;
    int _frame;
    unsigned long _lastNewBarCheckTime;
    CHSV _stripeHSV;
    CHSV _blackHSV;

    std::vector<Bar*> _colorBars;

    unsigned long _lastBlueChange;
    long _blueDelay;
    unsigned long _lastNewDotCheckTime;
    std::vector<BlueDot*> _dots;

    void colors();
    void stripes();
    void blue();
    void paintPixel(byte *stage, int index, bool add);
    void setTransitionColor(CRGB* led, float numerator, float denomenator, CHSV fromColor, CHSV toColor);
    void setTransitionColor(CRGB* led, float numerator, float denomenator, CRGB fromColor, CRGB toColor);
};

#endif /* CLOSEENCOUNTERS_H_ */
