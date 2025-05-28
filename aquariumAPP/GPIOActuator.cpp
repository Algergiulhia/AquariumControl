#include "GPIOActuator.h"

GPIOActuator::GPIOActuator(uint8_t gpioPin)
{
    this->gpioPin = gpioPin;
    this->state = off;
}

GPIOActuator::~GPIOActuator()
{
}

bool GPIOActuator::control(OnOff operation)
{
    char buffer[5];
    int fd0 = open("/dev/relayDD", O_WRONLY);

    sprintf(buffer, "%d%d", gpioPin, operation);
    write(fd0, &buffer, sizeof(buffer));
    state = operation;
    close(fd0);
    
    return false;
}

OnOff GPIOActuator::getState()
{
    return state;
}

void GPIOActuator::setState(OnOff state)
{
    this->state = state;
}
