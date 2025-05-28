#ifndef GPIOACTUATOR_H
#define GPIOACTUATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdint>

#define thresholdLight 0.5

enum OnOff {on, off};

class GPIOActuator {

private:
    OnOff state;        /**< Current state of the actuator */
    OnOff operation;    /**< Operation to be performed on the actuator*/
    uint8_t gpioPin;    /**< GPIO pin associated with the actuator */

public:
    GPIOActuator(uint8_t gpioPin);
    ~GPIOActuator();
    /**
     * @brief Controls the actuator based on the operation.
     * @param operation The operation to be performed (on / off)
     * @return True if the operation was successful, false otherwise
     */
    bool control(OnOff operation);

    /**
     * @brief Gets the current state of the actuator
     * @return The current state
     */
    OnOff getState();

    /**
     * @brief Sets the state of the actuator.
     * @param state The new state
     */
    void setState(OnOff state);
};

#endif
