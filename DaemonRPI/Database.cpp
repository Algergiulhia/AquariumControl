#include "Database.h"

Database::Database()
{
}

Database::~Database()
{
}

sqlite3* Database::openDatabase() {

    int rc = sqlite3_open(dbPath.c_str(), &db);

    if (rc != SQLITE_OK) {
        syslog(LOG_ERR, "Cannot open database: %s", sqlite3_errmsg(db));
        std::cout << "error opening database" << std::endl;
        sqlite3_close(db);
        return nullptr;
    }

    return db;
}

void Database::closeDatabase() {
    sqlite3_close(db);
}

int Database::executeQuery(const std::string& sql) {
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        syslog(LOG_ERR, "SQL error: %s", sqlite3_errmsg(db));
        std::cout << "error execute query" << std::endl;
        std::cout << sql << std::endl;
        return 1;
    }

    return 0;
}

void Database::insertSensorData(float ph, float temp, float tds) {
    sqlite3* db = openDatabase();
    if (!db) {
        return;
    }

    const char* query = "INSERT INTO sensorsData (ph, temp, tds, data_hora) VALUES (?, ?, ?, datetime('now'))";

    if (executeQuery(query) != 0) {
        closeDatabase();
        return;
    }

    sqlite3_bind_double(stmt, 1, ph);
    sqlite3_bind_double(stmt, 2, temp);
    sqlite3_bind_double(stmt, 3, tds);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    closeDatabase();

    if (rc != SQLITE_DONE) {
        syslog(LOG_ERR, "Execution error: %s", sqlite3_errmsg(db));
        std::cerr << "error execution error" << std::endl;
        return;
    }
}

std::vector<std::string> Database::getSensorReadings(const std::string& sensorType) {
    std::vector<std::string> readings;

    db = openDatabase();

    if (!db) {
        return readings;
    }

    std::string sql = "SELECT ROUND(" + sensorType + ", 2) FROM sensorsData;";

    if (executeQuery(sql) != 0) {
        closeDatabase();
        return readings;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *reading = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
        readings.push_back(reading);
    }

    sqlite3_finalize(stmt);
    closeDatabase();

    if (readings.empty()) {
        readings.push_back("EMPTY");
    }

    return readings;
}

void Database::removeOldData() {
    sqlite3* db = openDatabase();
    if (!db) {
        return;
    }

    const char* query = "DELETE FROM sensorsData WHERE data_hora < datetime('now', '-72 hours')";

    //sqlite3_stmt* stmt;
    if (executeQuery(query) != 0) {
        closeDatabase();
        return;
    }

    // Execução da consulta
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    closeDatabase();

    if (rc != SQLITE_DONE) {
        syslog(LOG_ERR, "Execution error: %s", sqlite3_errmsg(db));
        return;
    }
}

int Database::updateFishQuantity(const std::string& species) {
    std::string sql = "UPDATE aquariumFish SET quantity = quantity + 1 WHERE species = ?;";
    
    if (executeQuery(sql) != 0) {
        return 1;
    }

    sqlite3_bind_text(stmt, 1, species.c_str(), -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        syslog(LOG_ERR, "Update error: %s", sqlite3_errmsg(db));
        std::cerr << "Update error: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    return 0;
}

int Database::insertFishIfNotExists(const std::string& species) {
    std::string sql = "INSERT INTO aquariumFish (species, quantity) SELECT ?, 1 WHERE (SELECT COUNT(*) FROM aquariumFish WHERE species = ?) = 0;";
    
    if (executeQuery(sql) != 0) {
        return 1;
    }

    sqlite3_bind_text(stmt, 1, species.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, species.c_str(), -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        syslog(LOG_ERR, "Insert error: %s", sqlite3_errmsg(db));
        std::cerr << "Insert error: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    return 0;
}

int Database::add(const std::string& species) {
    sqlite3* db = openDatabase();
    if (!db) {
        return 1;
    }

    if (updateFishQuantity(species) != 0) {
        closeDatabase();
        return 1;
    }

    // If the updateFishQuantity func didnt change anything in the database, this condition is valid
    if (sqlite3_changes(db) == 0) {
        if (insertFishIfNotExists(species) != 0) {
            closeDatabase();
            return 1;
        }
    }

    closeDatabase();
    return 0;
}


int Database::remove(const std::string& species) {
    
    sqlite3* db = openDatabase();
    if (!db) {
        return 1;
    }

    std::string sql = "UPDATE aquariumFish SET quantity = quantity - 1 WHERE species = ? AND quantity > 0;";

    if (executeQuery(sql) != 0) {
        closeDatabase();
        return 1;
    }

    sqlite3_bind_text(stmt, 1, species.c_str(), -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    sql = "DELETE FROM aquariumFish WHERE species = ? AND quantity = 0;";

    if (executeQuery(sql) != 0) {
        closeDatabase();
        return 1;
    }

    sqlite3_bind_text(stmt, 1, species.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    closeDatabase();

    if (rc != SQLITE_DONE) {
        syslog(LOG_ERR, "Execution error: %s", sqlite3_errmsg(db));
        return 1;
    }

    return 0;
}

std::vector<std::string> Database::getSpeciesList(const std::string& tableName) {
    std::vector<std::string> speciesList;

    db = openDatabase();


    if (!db) {
        return speciesList;
    }

    std::string sql = "SELECT species FROM " + tableName + " ORDER BY species;";

    if (executeQuery(sql) != 0) {
        sqlite3_close(db);
        return speciesList;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *species = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
        speciesList.push_back(species);
    }

    sqlite3_finalize(stmt);
    closeDatabase();

    if (speciesList.empty()) {
        speciesList.push_back("EMPTY");
    }

    return speciesList;
}


std::vector<std::string> Database::getAqQuantity() {
    std::vector<std::string> speciesQtty;

    db = openDatabase();


    if (!db) {
        return speciesQtty;
    }

    /* Create SQL statement */
    std::string sql = "SELECT quantity FROM aquariumFish ORDER BY species;";
    if (executeQuery(sql) != 0) {
        closeDatabase();
        return speciesQtty;
    }

    /* Execute the query and add species to the list */
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *quantity = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
        speciesQtty.push_back(quantity);
    }

    sqlite3_finalize(stmt);
    closeDatabase();

    if (speciesQtty.empty()) {
        speciesQtty.push_back("0");
    }

    return speciesQtty;
}

FishParameters Database::getFishParameters(const std::string& species) {
    FishParameters params;

    db = openDatabase();
    if (!db) {
        return params;
    }

    std::string sql = "SELECT temperature, ph, food FROM fishList WHERE species = ?;";
    if (executeQuery(sql) != 0) {
        closeDatabase();
        return params;
    }

    int rc = sqlite3_bind_text(stmt, 1, species.c_str(), -1, SQLITE_STATIC);

    if (rc != SQLITE_OK) {
        syslog(LOG_ERR, "SQL bind error: %s", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return params;  // Return default parameters on bind error
    }

    /* Execute the query and retrieve parameters */
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        params.temperature = static_cast<float>(sqlite3_column_double(stmt, 0));
        params.ph = static_cast<float>(sqlite3_column_double(stmt, 1));
        params.food = static_cast<float>(sqlite3_column_double(stmt, 2));
    }

    /* Finalize the statement and close the database */
    sqlite3_finalize(stmt);
    closeDatabase();

    return params;
}

structIdealConditions Database::retrieveFishIdealCond() {
    char *errMsg = 0;

    structIdealConditions sIdealCond = {0, 0, 0};

    db = openDatabase();
    if (!db) {
        return sIdealCond;
    }

    const char *sql = "SELECT SUM(aquariumFish.quantity * fishList.food), AVG(fishList.temperature), AVG(fishList.ph) "
                      "FROM aquariumFish "
                      "JOIN fishList ON aquariumFish.species = fishList.species;";

    int rc = sqlite3_exec(db, sql, &Database::callback, &sIdealCond, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_free(errMsg);
    }

    closeDatabase();

    return sIdealCond;
}

int Database::callback(void *data, int argc, char **argv, char **colNames) {
    structIdealConditions *sIdealCond = static_cast<structIdealConditions *>(data);

    sIdealCond->food = atoi(argv[0]); //SUM(food)
    sIdealCond->temperature = atof(argv[1]); //AVG(temperature)
    sIdealCond->ph   = atof(argv[2]); //AVG(ph)

    return 0;
}

