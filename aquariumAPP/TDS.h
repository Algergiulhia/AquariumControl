#ifndef TDS_H
#define TDS_H

#include "Sensors.h"

#define thresholdTDS 50

class TDS : public Sensors
{
public:
    TDS();
    ~TDS();

    float convertValue(float rawValue);
};




#endif
