#ifndef IDEALCONDITIONS_H
#define IDEALCONDITIONS_H

#include <QObject>
#include <vector>
#include <string.h>
#include <cmath>

#include "msgqueue.h"

class IdealConditions : public QObject {
    Q_OBJECT
public:
    static IdealConditions& getInstance();  // singleton design pattern method
    ~IdealConditions();

    void updtNewFishParam(std::vector<std::string>);

    void setTDS(float);
    void setTemp(float);
    void setPh(float);
    float getLightDuration();

public:
    Q_INVOKABLE void calculateIdeal();
    Q_INVOKABLE bool verifyAddition();
    Q_INVOKABLE float getFood();
    Q_INVOKABLE float getTDS();
    Q_INVOKABLE float getTemp();
    Q_INVOKABLE float getPh();
    Q_INVOKABLE float getLightTime();
    Q_INVOKABLE float getFeedTime(bool);
    Q_INVOKABLE void setLightTime(float, float);
    Q_INVOKABLE void setFeedTime (float, float, bool);
    Q_INVOKABLE void setLightTimeDuration(float, float);
private:
    IdealConditions();
    IdealConditions(const IdealConditions&);                // Private copy constructor to prevent copying
    IdealConditions& operator=(const IdealConditions&);     // Private copy assignment operator to prevent assignment

private:
    float tds;
    float temp;
    float ph;
    float lightTime;
    float feedTime[2];
    float foodQuantity;
    float lightTimeDuration;
    float fishParams[3];

    std::string mqNameR = "/mqueueDaemonToIdeal";
    std::string mqNameW = "/mqueueToDaemon";

};

#endif // IDEALCONDITIONS_H
