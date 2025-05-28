#ifndef PH_H
#define PH_H

#include "Sensors.h"

#define thresholdPH 1.5

class PH : public Sensors
{
public:
    PH();
    ~PH();

    float convertValue(float rawValue);
};
#endif
