#ifndef DATABASE_H
#define DATABASE_H

#include "syslog.h"
#include <string>
#include <iostream>
#include <sqlite3.h>
#include <vector>
#include <ctime>

/**
 * @brief Represents parameters associated with a fish
 */
struct FishParameters {
    float food;         ///< Quantity of food suitable for the fish
    float temperature;  ///< Water temperature suitable for the fish
    float ph;           ///< pH level suitable for the fish
};

/**
 * @brief Represents the ideal value of each measure for the aquarium
*/
struct structIdealConditions {
    float food;         ///< Ideal Quantity of food for all the fish in the Aquarium
    float temperature;  ///< Ideal Temperature for the fish in the Aquarium
    float ph;           ///< Ideal ph for the fish in the Aquarium
};

class Database
{
private:
    sqlite3 *db;
    sqlite3_stmt* stmt;
    const std::string dbPath = "/etc/Aquarium/aqdb.db";
public:
    Database();
    ~Database();

    /**
     * @brief Opens a connection to the database
     * @return A pointer to the SQLite connection if successful, nullptr on error
     */
    sqlite3* openDatabase();

    /**
     * @brief Closes the connection to the database
     * @param db A pointer to the SQLite connection to be closed
     */
    void closeDatabase();

    /**
     * @brief Updates the quantity of a fish species in the aquarium database
     *
     * This function increases the quantity of the specified fish species in the aquariumFish table by 1
     *
     * @param species The name of the fish species
     * @param db The SQLite database connection
     * @return 0 on success, 1 on failure
     */

    int updateFishQuantity(const std::string& species);
    
    /**
     * @brief Inserts a new fish species into the aquarium database if it doesn't exist
     * @param species The name of the fish species
     * @param db The SQLite database connection
     * @return 0 on success, 1 on failure
     */
    int insertFishIfNotExists(const std::string& species);

    /**
     * @brief Executes an SQLite query
     * @param db A pointer to the SQLite connection
     * @param sql The SQL query to be executed
     * @param stmt A pointer to the prepared SQLite statement
     * @return 0 if the execution is successful, 1 on error
     */
    int executeQuery(const std::string& sql);

    /**
     * @brief Adds a new fish species to the database or increments its quantity if already exists
     * @param species Name of the fish species to add
     * @return 0 if successful, 1 if an error occurs
     */
    int add(const std::string& species);

    /**
     * @brief Decrements the quantity of a fish species in the database
     * If the quantity becomes zero, the species is removed from the database
     * @param species Name of the fish species to remove or decrement
     * @return 0 if successful, 1 if an error occurs
     */
    int remove(const std::string& species);

    /**
     * @brief Retrieves a list of species from the specified table in the database
     * @param tableName Name of the table containing species information
     * @return A vector of strings containing the species names If an error occurs, the vector returns with the word "EMPTY"
     */
    std::vector<std::string> getSpeciesList(const std::string& tableName);

    /**
     * @brief Retrieves the quantity of each fish species in the aquarium
     * @return A vector of strings containing the quantity of each species If an error occurs, the vector returns with "0"
     */
    std::vector<std::string> getAqQuantity();

    /**
     * @brief Retrieves fish parameters for a specific species from the database
     * This function gets a species name as input and queries the database
     * to retrieve the associated temperature, pH, and food parameters
     * @param species The name of the fish species
     * @return FishParameters structure containing temperature, pH, and food parameters
     */
    FishParameters getFishParameters(const std::string& species);

    /**
     * @brief Retrieves the ideal conditions for all fish in the aquarium
     * @return structIdealConditions structure containing the ideal quantity of food, temperature, and pH
     */
    structIdealConditions retrieveFishIdealCond();

    /**
     * @brief Callback function used in SQL queries
     * @param data User data passed to the callback
     * @param argc Number of columns in the result set
     * @param argv Array of strings representing column values
     * @param colNames Array of strings representing column names
     * @return 0 to continue processing rows
     */
    static int callback(void *data, int argc, char **argv, char **colNames);

    /**
     * @brief Inserts sensor data (PH, Temp, TDS) into the database
     * @param ph PH value
     * @param temp Temperature value
     * @param tds TDS (Total Dissolved Solids) value
     */
    void insertSensorData(float ph, float temp, float tds);

    /**
     * @brief Retrieves sensor readings from the database based on the sensor type
     * @param sensorType Type of sensor for which readings are retrieved ("PH", "Temp", "TDS")
     * @return A vector of strings containing the sensor requested readings
     */
    std::vector<std::string> getSensorReadings(const std::string& sensorType);

    /**
     * @brief Removes old sensor data from the database
     * Old data is defined as data recorded more than 72 hours ago
     */
    void removeOldData();
};

#endif