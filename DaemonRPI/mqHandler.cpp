#include "mqHandler.h"

void HandleMQueue (char* content) {
    Database db;
    char* c_command = strtok(content, " ");
    std::string command = c_command;
    syslog(LOG_INFO, "COMMAND READ: %s", c_command); 

    if (command == "ALLFISH") {
        std::vector<std::string> allSpeciesList = db.getSpeciesList("fishList");
        sendStringVector(allSpeciesList); 
        return;
    }
    else if (command == "AQUARIUM") {
        std::vector<std::string> aqSpeciesList = db.getSpeciesList("aquariumFish");
        sendStringVector(aqSpeciesList); 
        std::vector<std::string> aqSpeciesQtty = db.getAqQuantity();
        sendStringVector(aqSpeciesQtty);
        return;
    }
    else if (command == "INFO") {
        char* c_fishName = strtok (nullptr, "\0");
        std::string strFishName = c_fishName;
        syslog(LOG_INFO, "INFO FISH NAME READ: %s", c_fishName); 
        Info(strFishName);
        return;
    }
    else if (command == "IDEAL") {
        retrieveIdealCond();
        return;
    }
    else if (command == "ADD") {
        char* c_fishName = strtok (nullptr, "\0");
        std::string strFishName = c_fishName;
        syslog(LOG_INFO, "ADD FISH NAME READ: %s", c_fishName); 
        db.add(strFishName);
        return;
    }
    else if (command == "REMOVE") {
        char* c_fishName = strtok (nullptr, "\0");
        std::string strFishName = c_fishName;
        syslog(LOG_INFO, "REMOVE FISH NAME READ: %s", c_fishName); 
        db.remove(strFishName);
        return;
    }
    else if (command == "DATA") {
        char* c_sensor = strtok (nullptr, "\0");
        std::string strSensor = c_sensor;
        syslog(LOG_INFO, "SENSOR TO READ DATA: %s", c_sensor); 
        std::vector<std::string> sensorData = db.getSensorReadings(strSensor);
        sendStringVector(sensorData);
        return;
    }
    else if (command == "SAVEDATA") {
        char* c_ph = strtok (nullptr, " ");
        char* c_temp = strtok (nullptr, " ");
        char* c_tds = strtok (nullptr, "\0");
        syslog(LOG_INFO, "SENSOR DATA TO SAVE: %s + %s + %s", c_ph, c_temp, c_tds); 
        float f_ph = std::strtof(c_ph, nullptr);
        float f_temp = std::strtof(c_temp, nullptr);
        float f_tds = std::strtof(c_tds, nullptr);
        db.insertSensorData(f_ph, f_temp, f_tds);
        return;
    }
    else {
        syslog(LOG_INFO, "read unknow mq: %s", command.c_str());
        return;
    }
}

void Info(std::string strFishName) {
    FishParameters parameters;
    mqd_t mq;
    Database db;
    parameters = db.getFishParameters(strFishName);
    std::string stringData = StructToString(parameters);

    mq = createMsgQueue("/mqueueDaemonToDisplay");
    mq_send(mq, stringData.c_str(), stringData.size(), 0);
    mq_close(mq);
    return;
}


void retrieveIdealCond() {
    structIdealConditions newIC;
    mqd_t mq;
    Database db;

    newIC = db.retrieveFishIdealCond();

    std::string stringData = StructToString(newIC);
    syslog(LOG_INFO, "food %f \n temp %f \nph %f", newIC.food, newIC.temperature, newIC.ph);
    mq = createMsgQueue("/mqueueDaemonToIdeal");
    mq_send(mq, stringData.c_str(), stringData.size(), 0);
    mq_close(mq);
    return;
}

void sendStringVector(std::vector<std::string> stringVector) {
    std::string stringData;

    mqd_t mq = createMsgQueue("/mqueueDaemonToDisplay");

    for (const auto& str : stringVector)
        stringData += str + '+'; 

    mq_send(mq, stringData.c_str(), stringData.size(), 0);
    mq_close(mq);
    return;
}


template<typename structFTP>
std::string StructToString(structFTP tempStruct) {
    char buffer [100];
    std::sprintf(buffer, "%.1f+%.1f+%.1f+", tempStruct.food, tempStruct.temperature, tempStruct.ph);
    return std::string(buffer);
}