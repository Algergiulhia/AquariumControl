#ifndef ADS1115_H
#define ADS1115_H

#include <iostream>
#include <stdio.h>
#include <sys/types.h> // open
#include <sys/stat.h>  // open
#include <fcntl.h>     // open
#include <unistd.h>    // read/write usleep
#include <stdlib.h>    // exit
#include <inttypes.h>  // uint8_t, etc
#include <linux/i2c-dev.h> // I2C bus definitions
#include <sys/ioctl.h>

/// @brief Class responsible for reading voltage (adc) sensors values through i2c bus

class ADS1115{

    private:
        float sensorValue;  /**< Current sensor value read from the sensor */
        int fd;             /**< File descriptor for the I2C bus communication */
    public:
        ADS1115();
        ~ADS1115();
        /**
         * @brief Opens the I2C bus
         * @param bus The I2C bus to open
         * @return True if the I2C bus was opened successfully, false otherwise
         */
        bool openI2CBus(char *bus);

        /**
         * @brief Sets the I2C slave device address
         * @param deviceAddr The I2C slave device address
         * @return True if the I2C slave device address was set successfully, false otherwise
         */
        bool setI2CSlave(unsigned char deviceAddr);

        /**
         * @brief Reads the specified channel from the ADS1115
         * @param channel The channel to read from
         * @return True if the reading was successful, false otherwise.
         */
        bool readADS1115(int channel);

        /**
         * @brief Gets the current sensor value
         * @return The current sensor value
         */
        float getSensorValue();

};

#endif