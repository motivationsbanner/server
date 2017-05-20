#pragma once

#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <atomic>
#include <deque>
#include <map>
#include <mutex>
#include "Connection.h"
#include "DatabaseConnection.h"
#include "PacketHandler.h"
#include "Session.h"

// forward declaration
class PacketHandler;
class Session;

class Communicator {
 public:
  Communicator(unsigned short port);
  ~Communicator();
  void run();

  std::atomic<bool> terminated;

 private:
  std::mutex sessionMutex;
  std::map<sf::Uint16, Session *> sessionsByPlayerId;
  std::map<Connection, Session *> sessionsByConnection;

  std::deque<sf::Uint16> newSessions;         // player ids
  std::deque<sf::Uint16> terminatedSessions;  // player ids

  friend PacketHandler;
  unsigned short port;
  sf::UdpSocket socket;
  PacketHandler *packetHandler;
};
