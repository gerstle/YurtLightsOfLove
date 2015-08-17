#ifndef RAINBOW_H_
#define RAINBOW_H_

#include "BaseMode.h";

class Rainbow: public BaseMode {
public:
    Rainbow();
    virtual ~Rainbow();

public:
    virtual void frame();
    virtual const char* name() { return _name; }

private:
    unsigned long _lastChangeTime;
    const char *_name;
    byte _lastStartHue;
    float _increment;
    uint8_t _delay;

    unsigned long _lastTopChangeTime;
    byte _lastTopStartHue;
    float _topIncrement;
    uint8_t _topDelay;
};

#endif /* RAINBOW_H_ */
