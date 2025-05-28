#ifndef SENSORS_H
#define SENSORS_H

#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QObject>

#include <iostream>
#include "ADS1115.h"

#define BuffSize 5
#define driver "/dev/i2c-1"
#define i2cAddress 0x48

class Sensors : public QObject  {
    Q_OBJECT
protected:
    float voltagevalue;  /**< The voltage value read from the sensor */
    float realValue;     /**< The real value converted from the raw sensor reading */
    int i2cChannel;      /**< The I2C channel used for communication */
    ADS1115 ads1115;     /**< The ADS1115 object for analog-to-digital conversion */

public :
    /**
     * @brief Gets the value of the sensor.
     * @return The real value (value already converted) of the sensor.
     */
    Q_INVOKABLE float getRealValue();

public:
    Sensors();
    ~Sensors();

    /**
     * @brief Reads the sensor value
     * @return True if the sensor reading was successful, false otherwise
     */
    bool readSensor();

    /**
     * @brief Sets the voltage value of the sensor
     * @param voltagevalue The voltage value to set
     */
    void setVoltageValue(float voltagevalue);

    /**
     * @brief Sets the real value of the sensor
     * @param realValue The real value to set
     */
    void setRealValue(float realValue);

    /**
     * @brief Gets the voltage value of the sensor
     * @return The voltage value of the sensor
     */
    float getVoltageValue();

    /**
     * @brief Converts the sensor reading to a real value
     * @param rawValue The sensor reading value
     * @return The converted value (real value)
     */
    virtual float convertValue(float rawValue) = 0;
};

#endif
