#include "Sensors.h"

Sensors::Sensors() {
    this->realValue = 20;
}

Sensors::~Sensors() {
    realValue = 0;
}

bool Sensors::readSensor() {
    char* driverPtr = const_cast<char*>(driver);
    if(!ads1115.openI2CBus(driverPtr))
        return false;
    if(!ads1115.setI2CSlave(i2cAddress))
        return false;
    if(!ads1115.readADS1115(i2cChannel))
        return false;
    voltagevalue = ads1115.getSensorValue();

    return true;
}

void Sensors::setVoltageValue(float voltagevalue) {
    this->voltagevalue = voltagevalue;
}

void Sensors::setRealValue(float realValue) {
    this->realValue = realValue;
}

float Sensors::getVoltageValue() {
    return voltagevalue;
}

float Sensors::getRealValue() {
    return realValue;
}
