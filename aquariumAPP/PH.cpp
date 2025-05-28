#include "PH.h"

PH::PH() {
    this->i2cChannel = 0;
}

PH::~PH() { }

float PH::convertValue(float voltageValue) {
    realValue = -5.70 * voltageValue + (21.34 - 0.7);
    return realValue;
}
