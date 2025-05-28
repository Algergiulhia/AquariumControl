#ifndef MAINCLASS_H
#define MAINCLASS_H

#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QApplication>
#include <QQuickView>
#include <QQmlContext>
#include <QMetaObject>
#include <QMetaMethod>
#include <QObject>
#include <QDebug>

#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <csignal>
#include <ctime>


#include "MQueueHandler.h"
#include "idealconditions.h"
#include "dataList.h"
#include "Sensors.h"
#include "Temp.h"
#include "PH.h"
#include "TDS.h"
#include "ServoActuator.h"
#include "GPIOActuator.h"

class MainClass : public QObject  {
    Q_OBJECT
public:
    MainClass();
    ~MainClass();
    void RunAC();
    void createMutexes();
    void destroyMutexes();
    void createConds();
    void destroyConds();
    void attrThreads();
    void createThreads();
    void joinThreads();

public /*slots*/:
    Q_INVOKABLE void requestReading();
    Q_INVOKABLE QStringList requestValues();
    Q_INVOKABLE int verifyError();
    Q_INVOKABLE void shutdown();

private:
    /*
     * @brief Threads Functions
    */
    static void *tUVLightFunc(void *);
    static void *tSigFunc(void *);
    static void *tReadSensorsFunc(void *);
    static void *tWaterHeaterFunc(void *);
    static void *tServoMotorFunc(void *);
    static void *tIdleFunc(void *);
    static void* tInterfaceFunc(void*);

    static void handleSig(int signal);
    /**
     * @brief Get the system current time
     * @return The system current time in minutes
     */
    static unsigned int getSystemTime();

    /**
     * @brief Receives an array of values, order them in ascending order and returns the middle value
     * @param sensorArray An array with the sensors reading values
     * @param size Size of the array
     * @return The best value
     */
    static float getBestValue(float sensorArray[], int size);

private:
    IdealConditions& idealCond = IdealConditions::getInstance();

    GPIOActuator heater{23};
    GPIOActuator light{24};
    ServoActuator servo;
    TDS tds;
    PH ph;
    Temp temp{"28-3ce1e3806c3f"};

    pthread_t tUVLight, tSig, tReadSensors, tWaterHeater, tServoMotor, tIdle, tInterface;
    pthread_attr_t attr0, attr1, attr2, attr3;
    sched_param sched0, sched1, sched2, sched3;
    pthread_mutex_t mutexRS, mutexMotor, mutexWH, mutexLight, mutexTime;
    pthread_cond_t condRS, condMotor, condWH, condLight, condTime;

    unsigned int sensorsReadTime;       /**< Time between sensors reading */
    static unsigned int FlagSIGALRM;    /**< Flag to signal that the timer ended */

    float realValueTds;
    float realValuePh;
    float realValueTemp;

    bool tds_error; /**< True if the tds value is unusual, false otherwise */
    bool ph_error;  /**< True if the ph value is unusual, false otherwise */
};

#endif
