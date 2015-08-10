#ifndef MELTMODE_H_
#define MELTMODE_H_

#include "BaseMode.h"
#include "Lib/perlins.h"

class MeltMode: public BaseMode {
public:
    MeltMode();
    virtual ~MeltMode();

public:
    virtual void frame();
    virtual const char* name() { return "melt"; }

private:
    double hueTracker;
    double valueTracker;
    double threeHueTracker;
    double threeValueTracker;
    Perlins *perlins;
};

#endif /* MELTMODE_H_ */
