#ifndef BASEMODE_H_
#define BASEMODE_H_

#include "leds.h"

class BaseMode {
public:
    BaseMode();
    virtual ~BaseMode();

public:
	virtual void frame() = 0;
    virtual const char* name() = 0;
};

#endif /* BASEMODE_H_ */
