#ifndef MSGQUEUE_H
#define MSGQUEUE_H

#include "QDebug"

#include <iostream>
#include <cstdlib>
#include "mqueue.h"
#include <vector>
#include <sstream>


mqd_t createMsgQueue(const char*);
char* readMsgQueue(const char*);

std::vector<std::string> splitString(char*, char);

#endif
