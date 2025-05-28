#ifndef MSGQUEUE_H
#define MSGQUEUE_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <mqueue.h> 

/**
 * @brief create or open a message queue
 * creates a message queue if it doesn't exist already
 * opens a message queue is it does already exist
 * returns the describer so it can be used to write in the same message queue
 * @param MQueueName name of the message queue
 * @return mq describer of the created or opened message queue
 */
mqd_t createMsgQueue(const char* );

/**
 * @brief reads a message queue
 * @param MQueueName name of the message queue
 * @return messageContent character array containing the message content          //DYNAMICALLY ALLOCATED ?????
 * 
 * opens and reads the message queue content and returns the read content
 */
char* readMsgQueue(const char* );

/**
 * @brief create all message queues of the program
 */
void mqInitALL();

#endif