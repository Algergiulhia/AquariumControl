#include "msgqueue.h"
#include <ostream>
#include <unistd.h>

mqd_t createMsgQueue(const char* MQueueName)
{
    // message queue describer
    mqd_t mq;
    
    // create message queue if it doesnt exist already
    mq = mq_open(MQueueName, O_CREAT | O_WRONLY, 0666, NULL);

    // check error creating message queue
    if (mq == (mqd_t)-1){
        syslog(LOG_ERR, "Error creating message queue"); 
        exit(EXIT_FAILURE);
    }

    // returns the message queue describer
    return mq;
}

char* readMsgQueue(const char* MQueueName)
{
    // open the message queue to read
    mqd_t mq = mq_open(MQueueName, O_RDONLY);

    // check error opening message queue
    if (mq == (mqd_t)-1){
        syslog(LOG_ERR, "Error creating message queue to read in daemon"); 
        exit(EXIT_FAILURE);
    }

    // Get the attributes of the message queue to determine the maximum message size
    struct mq_attr attr;
    if (mq_getattr(mq, &attr) == -1) {
        syslog(LOG_ERR, "Error getting message queue attributes"); 
        mq_close(mq);
        exit(EXIT_FAILURE);
    }
    
    char *messageContent = (char *)malloc(attr.mq_msgsize);

    // receive message and checks error reading it
    ssize_t readBytes = mq_receive(mq, messageContent, attr.mq_msgsize, NULL);
    if (readBytes == -1){
        syslog(LOG_ERR, "Error reading message"); 
        free(messageContent);        
        messageContent = NULL;          
    }
    else
        messageContent[readBytes] = '\0';   

    
    //char* content = messageContent;       TESTAR ISTO

    mq_close(mq);
    return messageContent;
}

void mqInitALL() {
    mqd_t mq1 = createMsgQueue("/mqueueToDaemon");
    mq_close(mq1);

    mqd_t mq2 = createMsgQueue("/mqueueDaemonToDisplay");
    mq_close(mq2);

    mqd_t mq3 = createMsgQueue("/mqueueDaemonToIdeal");
    mq_close(mq3);

    return;
}
