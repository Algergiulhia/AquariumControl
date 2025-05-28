#include "MQueueHandler.h"

MQueueHandler::MQueueHandler(const char* MQueueName, QObject* parent) : QObject(parent){
    this -> MQueueName = MQueueName;
    createMsgQueue();
}

void MQueueHandler::saveData(std::string command) {
    mqd_t mq = createMsgQueue();
    mq_send(mq,command.c_str(), command.size(), 0);
    mq_close(mq);
}

void MQueueHandler::requestMQ(QString command) {
    std::string strCommand = command.toStdString();
    mqd_t mq = createMsgQueue();
    mq_send(mq,strCommand.c_str(), strCommand.size(), 0);
    mq_close(mq);
}


QStringList MQueueHandler::getQStrList() {
    char* charFishList = readMsgQueue();
    std::vector<std::string> vectorFishList = splitString(charFishList, '+');
    QStringList QSLfishList = convertVectorToQStringList(vectorFishList);
    return QSLfishList;
}

QStringList MQueueHandler::getFishInfo() {
    char* charFishParams = readMsgQueue();
    std::vector<std::string> vectorFishParams = splitString(charFishParams, '+');
    QStringList QSLFishParams = convertVectorToQStringList(vectorFishParams);
    this->idealCondInst.updtNewFishParam(vectorFishParams);

    return QSLFishParams;
}

mqd_t MQueueHandler::createMsgQueue() {
    mqd_t mq = mq_open(MQueueName, O_CREAT | O_WRONLY, 0666, NULL);

    if (mq == (mqd_t)-1) {
        perror("Error creating message queue in create QT\n");
        exit(EXIT_FAILURE);
    }

    return mq;
}

char* MQueueHandler::readMsgQueue() {
    mqd_t mq = mq_open(MQueueName, O_RDONLY);

    if (mq == (mqd_t)-1) {
        perror("Error READING creating message queue\n");
        exit(EXIT_FAILURE);
    }

    struct mq_attr attr;
    if (mq_getattr(mq, &attr) == -1) {
        perror("Error getting message queue attributes\n");
        mq_close(mq);
        exit(EXIT_FAILURE);
    }

    char *messageContent = (char *)malloc(attr.mq_msgsize);

    ssize_t readBytes = mq_receive(mq, messageContent, attr.mq_msgsize, NULL);
    if (readBytes == -1) {
        perror("Error reading message\n");
        free(messageContent);
        messageContent = NULL;
    }
    else
        messageContent[readBytes] = '\0';

    mq_close(mq);
    qDebug() << "readBytes: " << readBytes << "\n";

    return messageContent;
}

std::vector<std::string> MQueueHandler::splitString(char* input, char delimiter) {
    std::vector<std::string> result;
    std::stringstream ss(input);

    std::string item;
    while (std::getline(ss, item, delimiter)) {
        result.push_back(item);
    }
    return result;
}

QStringList MQueueHandler::convertVectorToQStringList(const std::vector<std::string> &vector) {
    QStringList result;
    for (const auto &item : vector) {
        result.append(QString::fromStdString(item));
    }
    return result;
}
