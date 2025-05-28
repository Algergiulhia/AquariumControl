#include "Database.h"
#include <iostream>

int main(){
    structIdealConditions sIdealCond;
    Database db;

    db.add("ourico");
    db.remove("atum de lata");

    std::vector<std::string> speciesList = db.getSpeciesList("aquariumFish");

    for(auto &i: speciesList){

        std::cout << "AQ:" << i << std::endl;
    }

    std::vector<std::string> fishList = db.getSpeciesList("fishList");

    for(auto &i: fishList){

        std::cout << "FL:" << i << std::endl;
    }

    std::vector<std::string> quantity = db.getAqQuantity();

    for(auto &i: quantity){

        std::cout << "Qtty:" << i << std::endl;
    }

    sIdealCond = db.retrieveFishIdealCond();

    std::cout << "IC food:" << sIdealCond.food << std::endl;
    std::cout << "IC ph:" << sIdealCond.ph << std::endl;
    std::cout << "IC temperature:" << sIdealCond.temperature << std::endl;

    FishParameters sFishParams;

    sFishParams = db.getFishParameters("cavalo marinho");

    std::cout << "Params food:" << sFishParams.food << std::endl;
    std::cout << "Params ph:" << sFishParams.ph << std::endl;
    std::cout << "Params temperature:" << sFishParams.temperature << std::endl;

    db.insertSensorData(7.0, 25.0, 300);
    db.removeOldData();

    std::vector<std::string> phReadings = db.getSensorReadings("temp");
    std::cout << "pH Readings:" << std::endl;
    for (const auto& reading : phReadings) {
        std::cout << reading << std::endl;
    }

    return 0;
}

