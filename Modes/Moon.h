#ifndef MOON_H_
#define MOON_H_

#include "BaseMode.h"
#include "Perlins.h"

class Moon: public BaseMode {
public:
    Moon();
    virtual ~Moon();

public:
    virtual void frame();
    virtual const char* name() { return _name; }

private:
    const char *_name;
    CHSV _skyColor;
    CHSV _moonColor;
    unsigned long _lastChange;
    long _delay;

    Perlins *perlins;
    double _valueTracker;

    void moon();
    void sky();
};

#endif /* MOON_H_ */
