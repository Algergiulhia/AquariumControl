#ifndef MQUEUEHANDLER_H
#define MQUEUEHANDLER_H

#include <QDebug>
#include <QObject>
#include <QStringList>
#include <mqueue.h>
#include <string>
#include <vector>
#include <sstream>

#include "idealconditions.h"

class MQueueHandler : public QObject {
    Q_OBJECT
public:
    explicit MQueueHandler(const char*, QObject *parent = nullptr);
    void saveData(std::string);

public /*slots*/:
    Q_INVOKABLE QStringList getQStrList();
    Q_INVOKABLE void requestMQ(QString);
    Q_INVOKABLE QStringList getFishInfo();

private:
    mqd_t createMsgQueue();
    char* readMsgQueue();
    std::vector<std::string> splitString(char*, char);
    QStringList convertVectorToQStringList(const std::vector<std::string>&);

private:
    const char* MQueueName;
    IdealConditions& idealCondInst = IdealConditions::getInstance();
};

#endif
