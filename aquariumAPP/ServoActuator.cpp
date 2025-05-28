#include "ServoActuator.h"

ServoActuator::ServoActuator()
{
}

ServoActuator::~ServoActuator()
{
}

bool ServoActuator::feed(int foodQuantity)
{
    unsigned int freq = 50;
    volatile unsigned int duty = openS;
    int i = 0;
    char buffer[9];
    
    int fd0 = open("/dev/pwmDD", O_WRONLY);
    
    for (i = 0; i < 2*foodQuantity; i++){         // rotate de motor the times os foodQuantity
        snprintf(buffer, sizeof(buffer), "%05u%03u", freq, duty);
        write(fd0, &buffer, sizeof(buffer));    // send to the driver the duty cycle and frequency
        usleep(500000);
        if(duty == openS)   // change the state
            duty = closeS;
        else
            duty = openS;
    }
    close(fd0);
    if(dayTime == morning) // changes the dayTime variable in order to
        dayTime = night;      // ensure it has been fed
    else
        dayTime = morning;

    return false;
}

void ServoActuator::setDayTime(dayTimeS dayTime)
{
    this->dayTime = dayTime;
}

dayTimeS ServoActuator::getDayTime()
{
    return dayTime;
}
