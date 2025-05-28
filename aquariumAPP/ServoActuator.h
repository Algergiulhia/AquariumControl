#ifndef SERVOACTUATOR_H
#define SERVOACTUATOR_H

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdint>

#define openS 7
#define closeS 9
#define thresholdServo 0.5

enum dayTimeS {morning, night};

class ServoActuator {

private:
    dayTimeS dayTime; /**< Current time of the day */

public:
    ServoActuator();
    ~ServoActuator();
    /**
     * @brief Feed the actuator with a specified quantity of food
     * @param foodQuantity The quantity of food to feed
     * @return True if was successful, false otherwise
     */
    bool feed(int foodQuantity);

    /**
     * @brief Set the time of the day that fed occurred (morning / night)
     * @param dayTime The time of the day to set
     */
    void setDayTime(dayTimeS dayTime);

    /**
     * @brief Get the time of day when feeding has already occurred
     * @return The time of the day that feeding has occurred
     */
    dayTimeS getDayTime();
};
#endif
