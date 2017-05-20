#pragma once
#include <SFML/System.hpp>
#include <vector>
#include "Command.h"
#include "Connection.h"

// forward declaration
class Command;

class Session {
 public:
  sf::Uint16 playerId;
  Connection connection;

  std::vector<Command> incomingCommands;
  std::vector<Command> outgoingCommands;

  std::vector<Command> incomingRealtimeCommands;
  std::vector<Command> outgoingRealtimeCommands;

 private:
  // (explain the logic behind these)
  sf::Uint16 highestReceivedCommandId = 0;
  sf::Uint16 lowestNonReceivedCommandId = 0;
};
