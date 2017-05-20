#pragma once

#include <SFML/Network.hpp>

class Connection {
 public:
  sf::IpAddress address;
  unsigned short port;
  void send(sf::UdpSocket &socket, sf::Packet &packet);

  // these are needed because Connections have to be inserted to a map
  bool operator==(const Connection &other) const;
  bool operator<(const Connection &other) const;
  bool operator>(const Connection &other) const;
};
