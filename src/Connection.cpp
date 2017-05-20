#include "Connection.h"
#include "Logger.h"

void Connection::send(sf::UdpSocket &socket, sf::Packet &packet) {
  if (socket.send(packet, address, port) != sf::Socket::Done) {
    logger->log("failed to send a package", Logger::Level::Error);
  }
}

bool Connection::operator==(const Connection &other) const {
  return this->address == other.address && this->port == other.port;
}

bool Connection::operator>(const Connection &other) const {
  return this->address > other.address ||
         (this->address == other.address && this->port > other.port);
}

bool Connection::operator<(const Connection &other) const {
  return this->address < other.address ||
         (this->address == other.address && this->port < other.port);
}
