#include "mainclass.h"

Q_DECLARE_METATYPE(MainClass*)

MainClass::MainClass() {
    createMutexes();
    createConds();
    attrThreads();
}

MainClass::~MainClass() { }

void MainClass::RunAC() {
    createThreads();
    joinThreads();
}

void MainClass::createMutexes() {
    pthread_mutex_init(&mutexRS, nullptr);
    pthread_mutex_init(&mutexMotor, nullptr);
    pthread_mutex_init(&mutexWH, nullptr);
    pthread_mutex_init(&mutexLight, nullptr);
    pthread_mutex_init(&mutexTime, nullptr);
}

void MainClass::destroyMutexes() {
    pthread_mutex_destroy(&mutexRS);
    pthread_mutex_destroy(&mutexMotor);
    pthread_mutex_destroy(&mutexWH);
    pthread_mutex_destroy(&mutexLight);
    pthread_mutex_destroy(&mutexTime);
}

void MainClass::createConds() {
    pthread_cond_init(&condRS, nullptr);
    pthread_cond_init(&condMotor, nullptr);
    pthread_cond_init(&condWH, nullptr);
    pthread_cond_init(&condLight, nullptr);
    pthread_cond_init(&condTime, nullptr);
}

void MainClass::destroyConds() {
    pthread_cond_destroy(&condRS);
    pthread_cond_destroy(&condMotor);
    pthread_cond_destroy(&condWH);
    pthread_cond_destroy(&condLight);
    pthread_cond_destroy(&condTime);
}

void MainClass::attrThreads() {

    sched0.sched_priority = 1;
    sched1.sched_priority = 2;
    sched2.sched_priority = 3;
    sched3.sched_priority = 4;

    pthread_attr_setdetachstate(&attr0 ,PTHREAD_CREATE_JOINABLE );
    pthread_attr_setschedparam(&attr0 ,&sched0);
    pthread_attr_init(&attr0);
    pthread_attr_setdetachstate(&attr1 , PTHREAD_CREATE_JOINABLE);
    pthread_attr_setschedparam(&attr1 ,&sched1);
    pthread_attr_init(&attr1);
    pthread_attr_setdetachstate(&attr2 , PTHREAD_CREATE_JOINABLE);
    pthread_attr_setschedparam(&attr2 ,&sched2);
    pthread_attr_init(&attr2);
    pthread_attr_setdetachstate(&attr3 , PTHREAD_CREATE_JOINABLE);
    pthread_attr_setschedparam(&attr3 ,&sched3);
    pthread_attr_init(&attr3);
}

void MainClass::createThreads() {

    pthread_create(&tUVLight , &attr3 , tUVLightFunc , this);
    pthread_create(&tSig , &attr2 , tSigFunc , this);
    pthread_create(&tReadSensors , &attr2 , tReadSensorsFunc, this);
    pthread_create(&tWaterHeater , &attr3 , tWaterHeaterFunc , this);
    pthread_create(&tServoMotor , &attr3 , tServoMotorFunc , this);
    pthread_create(&tIdle , &attr1 , tIdleFunc , this);
    pthread_create(&tInterface, &attr0 , tInterfaceFunc, this);

}

void MainClass::joinThreads() {
    pthread_join(tUVLight, NULL);
    pthread_join(tSig, NULL);
    pthread_join(tReadSensors, NULL);
    pthread_join(tWaterHeater, NULL);
    pthread_join(tServoMotor, NULL);
    pthread_join(tIdle, NULL);
    pthread_join(tInterface, NULL);
}

void MainClass::shutdown() {
    system("rmmod pwm") ;
    system("rmmod relay");
    system("umount /dev/mqueue");
    system("rm -r /dev/mqueue");
    system("poweroff");
}

void *MainClass::tSigFunc(void *aquarium) {
    MainClass * aq = static_cast<MainClass*>(aquarium);
    unsigned int timeInMinutes = getSystemTime();
    float feedTime [2];
    feedTime[morning] = aq->idealCond.getFeedTime(morning);     // [0]:morning
    feedTime[night] = aq->idealCond.getFeedTime(night);         // [1]:night

    if (timeInMinutes > feedTime[morning]) {
        aq->servo.setDayTime(morning);
    } else {
        aq->servo.setDayTime(night);
    }

    while(1) {
        pthread_mutex_lock(&aq->mutexTime);
        pthread_cond_wait(&aq->condTime, &aq->mutexTime);
        pthread_mutex_unlock(&aq->mutexTime);

        timeInMinutes = getSystemTime();

        feedTime[morning] = aq->idealCond.getFeedTime(morning); // [0]:morning
        feedTime[night] = aq->idealCond.getFeedTime(night);     // [1]:night

        std::cout << "time cpu: " << timeInMinutes <<  std::endl;
        std::cout << "feed time morning: " << feedTime[morning] <<  std::endl;
        std::cout << "feed time night: " << feedTime[night] <<  std::endl;
        std::cout << "light time : " << aq->idealCond.getLightTime() <<  std::endl;

        if((feedTime[morning] <= timeInMinutes && timeInMinutes <= feedTime[morning] + thresholdServo   // check if it is time to feed
             && aq->servo.getDayTime() == night) || (feedTime[night] <= timeInMinutes
                && timeInMinutes <= feedTime[night] + thresholdServo && aq->servo.getDayTime() == morning)) {
            pthread_cond_signal(&aq->condMotor);    //condition to feed the tank
        }
        if (aq->idealCond.getLightTime() <= timeInMinutes && timeInMinutes <= (aq->idealCond.getLightTime() + thresholdLight) // // check if it is time to activate the light
            && aq->light.getState() == off) {
            pthread_cond_signal(&aq->condLight);    //condition to activate UVlight the tank
        }
        pthread_cond_signal(&aq->condRS);   //condition to read sensors values
    }
    pthread_exit(NULL);
}

void *MainClass::tReadSensorsFunc(void *aquarium) {
    MainClass * aq = static_cast<MainClass*>(aquarium);

    const int readingBuffer = 5;
    float tdsReading[readingBuffer];
    float phReading[readingBuffer];
    float tempReading[readingBuffer];
    float idealTds, idealPh, idealTemp;
    OnOff heaterState;
    MQueueHandler SaveSensorData("/mqueueToDaemon");


    while(1) {
        pthread_mutex_lock(&aq->mutexRS);
        pthread_cond_wait(&aq->condRS, &aq->mutexRS);
        pthread_mutex_unlock(&aq->mutexRS);

        idealTds = aq->idealCond.getTDS();
        idealPh = aq->idealCond.getPh();
        idealTemp =  aq->idealCond.getTemp();
        heaterState = aq->heater.getState();


        int i = 0;
        for (i = 0; i < readingBuffer; ++i) { // read all the sensors the readingBuffer times

            if(!aq->tds.readSensor())
                std::cout << "Error Reading TDS sensor " << std::endl;
            if(!aq->ph.readSensor())
                std::cout << "Error Reading PH sensor " << std::endl;
            if(!aq->temp.readSensor())
                std::cout << "Error Reading Temp sensor " << std::endl;

            tdsReading[i]   = aq->tds.getVoltageValue();    // store the value of sensor readings
            phReading[i]    = aq->ph.getVoltageValue();     // in an array 
            tempReading[i]  = aq->temp.getVoltageValue();
        }

        aq->realValuePh   = aq->ph.convertValue(getBestValue(phReading, i));    // get the best value of the array
        aq->realValueTemp = aq->temp.convertValue(getBestValue(tempReading, i));
        aq->realValueTds  = aq->tds.convertValue(getBestValue(tdsReading, i));

        std::string sensorData = "SAVEDATA " + std::to_string(aq->realValuePh)
                                 + " " + std::to_string(aq->realValueTemp)
                                 + " " + std::to_string(aq->realValueTds);
        SaveSensorData.saveData(sensorData); // produce a msqueue with the sensors values to store in the database

        if (aq->realValueTds > idealTds + thresholdTDS) { // tds exceeded acceptable limits
            aq->tds_error = true; // variable to produce a tds warning in the display
        }
        else {
            aq->tds_error = false;
        }

        if (aq->realValuePh > idealPh + thresholdPH || aq->realValuePh < idealPh - thresholdPH){ // pH exceeded acceptable limits
            aq->ph_error = true;    // variable to produce a ph warning in the display
        }
        else {
            aq->ph_error = false;
        }
        if (aq->realValueTemp > idealTemp + thresholdTemp && heaterState == on) {   // the temperature is higher than it should so the heater
                                                                                    // must be turned off
            aq->heater.setState(off);
            pthread_mutex_lock(&aq->mutexWH);
            pthread_cond_signal(&aq->condWH);
            pthread_mutex_unlock(&aq->mutexWH);
        }

        if (aq->realValueTemp < idealTemp - thresholdTemp) {    // water is to cold according to the ideal Conditions

            alarm(0);
            alarm(15);  // change the timer to perform fast readings because water will be warming up

            if (heaterState != on) {
                aq->heater.setState(on);
                pthread_mutex_lock(&aq->mutexWH);
                pthread_cond_signal(&aq->condWH);
                pthread_mutex_unlock(&aq->mutexWH);
            }
        }
    }
    pthread_exit(NULL);
}

void *MainClass::tWaterHeaterFunc(void *aquarium) {
    MainClass * aq = static_cast<MainClass*>(aquarium);

    aq->heater.control(off);
    while(1) {
        pthread_mutex_lock(&aq->mutexWH);
        pthread_cond_wait(&aq->condWH, &aq->mutexWH);   
        pthread_mutex_unlock(&aq->mutexWH);

        aq->heater.control(aq->heater.getState());  //according to the state, turn on or off the heater

    }
    pthread_exit(NULL);
}

void *MainClass::tUVLightFunc(void *aquarium) {
    MainClass * aq = static_cast<MainClass*>(aquarium);

    aq->light.control(off);
    unsigned int lightDuration = aq->idealCond.getLightDuration();
    bool flagUv = 0;
    volatile int count = 0;
    while(1) {

        if(!flagUv){
            pthread_mutex_lock(&aq->mutexLight);
            pthread_cond_wait(&aq->condLight, &aq->mutexLight); //condition to activate the light
            pthread_mutex_unlock(&aq->mutexLight);
            flagUv = 1;
            aq->light.control(on);  
            lightDuration = aq->idealCond.getLightDuration();
        }
        sleep(1);
        count++;
        if(count >= lightDuration*60){           // When the time for having the light on ends,
            pthread_mutex_lock(&aq->mutexLight); // this condition is valid
            aq->light.control(off);
            pthread_mutex_unlock(&aq->mutexLight);
            count = 0;
            flagUv = 0;
        }
    }
    pthread_exit(NULL);
}


void *MainClass::tServoMotorFunc(void *aquarium) {
    MainClass * aq = static_cast<MainClass*>(aquarium);

    while(1) {
        pthread_mutex_lock(&aq->mutexMotor);
        pthread_cond_wait(&aq->condMotor, &aq->mutexMotor);
        pthread_mutex_unlock(&aq->mutexMotor);

        float foodQuantity = aq->idealCond.getFood();

        aq->servo.feed(foodQuantity);   // rotates the motor the times of food needed
    }
    pthread_exit(NULL);
}

unsigned int MainClass::FlagSIGALRM = 0;

void MainClass::handleSig(int signal) {
    std::cout << "signalAlarm" << std::endl;
    if (signal == SIGALRM) {
        FlagSIGALRM = 1;
    }
}

float MainClass::getBestValue(float sensorArray[], int size) {
    for (int j = size; j > 0 && sensorArray[j] < sensorArray[j - 1]; --j) {
        std::swap(sensorArray[j], sensorArray[j - 1]);
    }
    return sensorArray[size/2];
}

unsigned int MainClass::getSystemTime() {
    std::time_t TimeInfo = std::time(nullptr);
    std::tm* sTime = std::localtime(&TimeInfo); // Converts the time to a tm struct

    return ((sTime->tm_hour * 60) + sTime->tm_min);
}

void MainClass::requestReading() {
    pthread_cond_signal(&condRS);
}

QStringList MainClass::requestValues() {
    QStringList readValues;
    readValues << QString::number(realValueTemp) << QString::number(realValuePh) <<  QString::number(realValueTds);
    return readValues;
}

// return 0 if there is no error, 1 if tds is wrong, 2 if ph is wrong, 3 if both are wrong
int MainClass::verifyError() {
    int error = 0;
    if (tds_error == true)
        error += 1;
    if (ph_error == true)
        error += 2;
    return error;
}

void *MainClass::tIdleFunc(void *aquarium) {
    MainClass *aq = static_cast<MainClass*>(aquarium);

    aq->FlagSIGALRM = 0;
    aq->sensorsReadTime = 30;

    signal(SIGALRM, handleSig);
    alarm(1);
    while (1) {
        if (aq->FlagSIGALRM) {
            pthread_mutex_lock(&aq->mutexTime);
            pthread_cond_signal(&aq->condTime);
            pthread_mutex_unlock(&aq->mutexTime);
            alarm(aq->sensorsReadTime);
            aq->FlagSIGALRM = 0;
        }
        sleep(1);
    }
    pthread_exit(NULL);
}

void* MainClass::tInterfaceFunc(void* aquariumQt) {
    MainClass *aqQt = static_cast<MainClass*>(aquariumQt);

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    int a = 1;
    QApplication app(a, nullptr);
    QQmlApplicationEngine engine;

    const QUrl url_1(QStringLiteral("qrc:/main.qml"));

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [url_1](QObject *obj, const QUrl &objUrl) {
            if (!obj && url_1 == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);

    MQueueHandler SendToDaemon("/mqueueToDaemon");
    MQueueHandler ReadFromDaemon("/mqueueDaemonToDisplay");
    dataList all;
    dataList aquarium;
    IdealConditions& Ideal = IdealConditions::getInstance();

    engine.rootContext()->setContextProperty("all", &all);
    engine.rootContext()->setContextProperty("aquarium", &aquarium);
    engine.rootContext()->setContextProperty("SendToDaemon", &SendToDaemon);
    engine.rootContext()->setContextProperty("ReadFromDaemon", &ReadFromDaemon);
    engine.rootContext()->setContextProperty("Ideal", &Ideal);
    engine.rootContext()->setContextProperty("aq", QVariant::fromValue (aqQt));

    engine.load(url_1);

    app.exec();

    pthread_exit(NULL);
    return nullptr;
}
