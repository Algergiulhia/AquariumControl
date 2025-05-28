#ifndef TEMP_H
#define TEMP_H

#include <string>
#include <regex>
#include <limits>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <cmath>
#include "Sensors.h"

#define oneWireRootFolder "/sys/bus/w1/devices/"
#define oneWireFile "/w1_slave"

#define thresholdTemp 2

class Temp : public Sensors
{
private:
    std::string address;
    
public:

    Temp(std::string address);
    ~Temp();
    bool readSensor();
    float convertValue(float rawValue);
};

#endif
