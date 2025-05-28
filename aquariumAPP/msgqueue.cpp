#include "msgqueue.h"


mqd_t createMsgQueue(const char* MQueueName) {
    mqd_t mq = mq_open(MQueueName, O_CREAT | O_WRONLY, 0666, NULL);

    if (mq == (mqd_t)-1) {
        perror("Error creating message queue in create QT\n");
        exit(EXIT_FAILURE);
    }

    return mq;
}

char* readMsgQueue(const char* MQueueName) {
    mqd_t mqR = mq_open(MQueueName, O_RDONLY);

    if (mqR == (mqd_t)-1) {
        perror("Error READING message queue\n");
        exit(EXIT_FAILURE);
    }

    struct mq_attr attr;
    if (mq_getattr(mqR, &attr) == -1) {
        perror("Error getting message queue attributes\n");
        mq_close(mqR);
        exit(EXIT_FAILURE);
    }

    char *messageContent = (char *)malloc(attr.mq_msgsize);

    ssize_t readBytes = mq_receive(mqR, messageContent, attr.mq_msgsize, NULL);
    if (readBytes == -1) {
        perror("Error reading message\n");
        free(messageContent);
        messageContent = NULL;
    }
    else
        messageContent[readBytes] = '\0';

    mq_close(mqR);

    return messageContent;
}

std::vector<std::string> splitString(char* input, char delimiter) {
    std::vector<std::string> result;
    std::stringstream ss(input);

    std::string item;
    while (std::getline(ss, item, delimiter)) {
        result.push_back(item);
    }
    return result;
}
