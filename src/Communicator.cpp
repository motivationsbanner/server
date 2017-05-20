#include "Communicator.h"
#include <sstream>
#include <string>
#include "Connection.h"
#include "Logger.h"

Communicator::Communicator(unsigned short port) {
  packetHandler = new PacketHandler(this);

  terminated = false;
  this->port = port;
}

Communicator::~Communicator() { delete packetHandler; }

void Communicator::run() {
  sf::SocketSelector selector;  // the selector allows the use of timeouts

  // try to listen on the specified port
  if (socket.bind(this->port) != sf::Socket::Done) {
    throw std::runtime_error("Failed to bind the socket.");
  }

  // the socket has to be added AFTER the socket.bind call
  selector.add(socket);

  // recieve packets until the server shuts down
  sf::Socket::Status status;
  while (terminated != true) {
    Connection client;  // ip and port of the sender
    sf::Packet packet;  // incoming data

    if (selector.wait(sf::seconds(0.1))) {
      status = socket.receive(packet, client.address, client.port);

      if (status == sf::Socket::Done) {
        sessionMutex.lock();
        packetHandler->handlePacket(client, packet);
        sessionMutex.unlock();
      } else {
        std::stringstream message;
        message << "Failed to receive packet (Error Code " << status << ")";
        logger->log(message, Logger::Level::Warning);
      }
    }
  }
}
