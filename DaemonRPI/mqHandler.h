#ifndef MQHANDLER_H
#define MQHANDLER_H

#include <cstdlib>
#include <string.h>
#include <sstream>
#include <syslog.h>
#include <mqueue.h> 
#include <vector>

#include "Database.h"
#include "msgqueue.h"

/**
 * @brief handle message queues function
 * @param content first word of message queue
 * 
 * reads the first word of the message queue as a command 
 * and executes an action based on the command
 */
void HandleMQueue (char*);

/**
 * @brief send to a message queue the ideal conditions of a fish species
 * @param strFishName fish species to get information about
 * 
 * reads the fish species name, retrieves its ideal conditions from 
 * the SQLite Database and sends them to a message queue
 */
void Info(std::string);

/**
 * @brief retrieves and sends the aquarium ideal conditions
 * 
 * retrieves the aquarium ideal conditions from the SQLite Database
 * and send them to a message queue
 */
void retrieveIdealCond();

/**
 * @brief sends the list of all the fish species to a message queue
 * @param speciesList vector of strings containing all the fish species of the SQLite Database
 * 
 * converts the vector of strings in a string and sends it to a message queue
 */
void sendStringVector(std::vector<std::string>);

/**
 * @brief converts a specified struct in a string
 * @tparam structFTP type of the struct to be converted
 * @param tempStruct instance of the struct to be converted
 * @return string representation of the converted struct
 * 
 * converts a struct of type structFTP and converts its fields to
 * a string where each field is separated by a '+'
 */
template<typename structFTP>
std::string StructToString(structFTP);

#endif