#include "DatabaseConnection.h"
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cstdlib>
#include <iostream>
#include "Logger.h"

// this is based on
// https://dev.mysql.com/doc/connector-cpp/en/connector-cpp-examples-complete-example-1.html
// https://dev.mysql.com/doc/connector-cpp/en/connector-cpp-examples-complete-example-2.html

DatabaseConnection::DatabaseConnection() {
  const char *username = "overkeggly_user";
  const char *password = std::getenv("OVERKEGGLY_USER_PASSWORD");

  if (password == nullptr) {
    throw std::runtime_error(
        "the OVERKEGGLY_USER_PASSWORD environment variable has to be set");
  }

  try {
    driver = get_driver_instance();

    con = driver->connect("tcp://127.0.0.1:3306", username, password);
    con->setSchema("overkeggly");
  } catch (sql::SQLException &e) {
    throw std::runtime_error("failed to connect to the database");
  }
}

DatabaseConnection::~DatabaseConnection() { delete con; }

DatabaseConnection::PlayerData DatabaseConnection::getPlayerData(
    std::string &name, std::string &password) {
  try {
    sql::PreparedStatement *pstmt;
    sql::ResultSet *res;

    pstmt = con->prepareStatement(
        "SELECT id, name, x, y FROM players WHERE"
        " name = ? AND SHA2(?, 256) = password");

    pstmt->setString(1, name);
    pstmt->setString(2, password);

    res = pstmt->executeQuery();

    if (!res->next()) {
      return {0};
    }

    PlayerData data{res->getUInt("id"), res->getString("name"),
                    res->getUInt("x"), res->getUInt("y")};

    delete res;
    delete pstmt;
    return data;

  } catch (sql::SQLException &e) {
    logger->log("unable to test user credentials", Logger::Level::Error);
    return {0};
  }
}

void DatabaseConnection::setPlayerPosition(sf::Uint32 id, sf::Uint32 x,
                                           sf::Uint32 y) {
  try {
    sql::PreparedStatement *pstmt;
    pstmt =
        con->prepareStatement("UPDATE players SET x = ?, y = ? WHERE id = ?");
    pstmt->setUInt(1, x);
    pstmt->setUInt(2, y);
    pstmt->setUInt(3, id);

    pstmt->executeUpdate();
    delete pstmt;
  } catch (sql::SQLException &e) {
    // TODO: also log position
    logger->log("unable to save playerposition", Logger::Level::Error);
  }
}
