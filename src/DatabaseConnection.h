#pragma once

#include <cppconn/driver.h>
#include <mysql_connection.h>
#include <SFML/System.hpp>
#include <string>

class DatabaseConnection {
 public:
  struct PlayerData {
    sf::Uint32 id;
    std::string name;
    sf::Uint32 x;
    sf::Uint32 y;
  };

  DatabaseConnection();
  ~DatabaseConnection();
  DatabaseConnection::PlayerData getPlayerData(std::string &name,
                                               std::string &password);
  void setPlayerPosition(sf::Uint32 id, sf::Uint32 x, sf::Uint32 y);

 private:
  sql::Driver *driver;
  sql::Connection *con;
};
