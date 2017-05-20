#pragma once

#include "Game.h"

// forward declaration
class Game;

class Command {
  virtual void execute(Game &game) = 0;  // has to implemented by child classes
};
