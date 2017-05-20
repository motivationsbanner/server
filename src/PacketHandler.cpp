#include "PacketHandler.h"
#include "Logger.h"

PacketHandler::PacketHandler(Communicator *communicator) {
  this->communicator = communicator;
}

void PacketHandler::handlePacket(Connection &client, sf::Packet &packet) {
  // here we "handle" a packet. in case something goes wrong we log something
  // and abort the operation

  // try to read the type of the packet
  sf::Uint8 packetType;
  if (!(packet >> packetType)) {
    logger->log("failed to extract packetType", Logger::Level::Warning);
    return;
  }

  // make sure the packet type is in the allowed range so we don't have to
  // handle special cases later
  if (packetType >= PacketType::P_END) {
    logger->log("got an invalid packetType", Logger::Level::Warning);
    return;
  }

  // test if the client has already an active session
  if (communicator->sessionsByConnection.find(client) !=
      communicator->sessionsByConnection.end()) {
    // the client already has an active session so this most is most likely a
    // regular packet. In case the packet is a Login packet we ignore it. We
    // send regular packets anyways and when the client gets one it will know
    // that the session is active
    if (packetType == PacketType::Login) {
      return;
    }

    // here we handle regular packets (TODO)

  } else {
    if (packetType != PacketType::Login) {
      logger->log("ignoring a login packet of a already logged in user");
      return;
    }
    handleLoginPacket(client, packet);
  }

  if (!packet.endOfPacket()) {
    logger->log("there was more data in a packet than expected",
                Logger::Level::Warning);
  }
}

void PacketHandler::handleLoginPacket(Connection &client, sf::Packet &packet) {
  std::string username;
  std::string password;

  if (!(packet >> username) || !(packet >> password)) {
    logger->log("failed to extract username and password",
                Logger::Level::Warning);
    packet.clear();  // don't log a second warning
    return;
  }

  DatabaseConnection::PlayerData data =
      database.getPlayerData(username, password);

  if (data.id == 0) {
    logger->log("login failed due to wrong credentials");
    rejectClient(client);
    return;
  }

  logger->log("successful login");
  acceptClient(client, data);
}

void PacketHandler::rejectClient(Connection &connection) {
  sf::Packet packet;
  packet << (sf::Uint8)PacketType::LoginFailed;
  connection.send(communicator->socket, packet);
}

void PacketHandler::acceptClient(Connection &connection,
                                 DatabaseConnection::PlayerData data) {
  // the sessions are saved in two maps. they can be accessed by the connection
  // or by the player id

  Session *session = new Session();
  session->playerId = data.id;
  session->connection = connection;

  communicator->sessionsByPlayerId[data.id] = session;
  communicator->sessionsByConnection[connection] = session;

  communicator->newSessions.push_back(data.id);
}
