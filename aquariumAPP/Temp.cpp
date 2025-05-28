#include "Temp.h"

Temp::Temp(std::string address)   
{
    this->address = oneWireRootFolder + address + oneWireFile;
    return;
}

Temp::~Temp(){

}

bool Temp::readSensor()
{
    // Open one-Wire reader as file stream and read
    std::ifstream reader{address};

    std::string firstLine, secondLine;
    getline(reader, firstLine);
    getline(reader, secondLine);

    // Get status, present in first line
    std::string status = firstLine.substr(firstLine.find_last_of(" ") + 1);

    // Check if temperature was read properly
    if (status != "YES")
    {
        std::cerr << "Temperature was not measured properly!" << std::endl;

        return false;
    }

    // Get temperature second line
    float tempRaw = std::stof(secondLine.substr(secondLine.find("t=") + 2));


    voltagevalue = tempRaw;
    
    return true;
}

float Temp::convertValue(float voltageValue)
{
    realValue = (voltageValue/1000);
    return realValue;
}
