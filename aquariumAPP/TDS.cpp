#include "TDS.h"

TDS::TDS()
{
    this->i2cChannel = 3;
}

TDS::~TDS()
{
}

float TDS::convertValue(float voltageValue) {
    float tdsValue=(133.42*voltageValue*voltageValue*voltageValue - 255.86*voltageValue*voltageValue + 857.39*voltageValue)*0.5;
    realValue = tdsValue;
    return tdsValue;
}
