#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include "Communicator.h"
#include "DatabaseConnection.h"
#include "Game.h"
#include "Logger.h"
#include "PacketHandler.h"

int main(int argc, char **argv) {
  unsigned short port = 4499;

  // initialize logger
  logger = new Logger();

  std::stringstream message;
  message << "Starting server on 127.0.0.1:" << port;
  logger->log(message);

  // create components
  Communicator com(port);
  Game game;

  // start threads
  std::thread communicatorThread(&Communicator::run, std::ref(com));
  std::thread gameloopThread(&Game::run, std::ref(game));

  // Ctrl+D stops the server
  // this could be a simple shell in the future
  // (gnu readline?)
  std::cin.get();

  logger->log("Terminating");
  com.terminated = true;

  gameloopThread.join();
  communicatorThread.join();

  delete logger;

  return 0;
}
