#pragma once

#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include "Communicator.h"
#include "Connection.h"
#include "DatabaseConnection.h"

/*
  Login packet (Client -> Server)
  ================================
  To initialize a session, the client sends a login package.
  The login package gets resent every 500 ms in case it gets
  lost.

  +--------------+
  | sf::Uint8    | PacketHandler::PacketType::Login
  | sf::string   | username
  | sf::string   | password
  +--------------+


  Login failed packet (Server -> Client)
  =========================================
  When the client sends the wrong credentials or a session with
  the same name is already active, this packet gets sent.
  If this packet gets lost, the client will simply send a new login
  packet.

  +------------------+
  | sf::Uint8        | PacketHandler::PacketType::LoginFailed
  +------------------+


  Regular packet(Client <-> Server)
  ==================================
  +--------------+
  | sf::Uint8    | PacketHandler::PacketType::Regular
  | [            | Array with all the player positions
  |  sf::Uint32  | x
  |  sf::Uint32  | y
  | ]            |
  +--------------+

*/

// forward declaration
class Communicator;

class PacketHandler {
 public:
  PacketHandler(Communicator *communicator);
  enum PacketType : sf::Uint8 { Login, LoginFailed, Regular, P_END };
  void handlePacket(Connection &client, sf::Packet &packet);

 private:
  Communicator *communicator;
  DatabaseConnection database;
  void handleLoginPacket(Connection &client, sf::Packet &packet);
  void acceptClient(Connection &connection,
                    DatabaseConnection::PlayerData data);
  void rejectClient(Connection &connection);
};
