#ifndef COLORMODE_H_
#define COLORMODE_H_

#include "BaseMode.h"

class ColorMode: public BaseMode {
public:
    ColorMode(const char *name, CHSV color, long delay);
    virtual ~ColorMode();

public:
    virtual void frame();
    virtual const char* name() { return _name; }

private:
    const char *_name;
    CHSV _color;
    unsigned long _lastChange;
    long _delay;
};

#endif /* COLORMODE_H_ */
