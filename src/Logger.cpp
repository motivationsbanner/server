#include "Logger.h"
#include <ctime>
#include <iostream>

Logger::Logger(const char *path) {
  if (path == NULL) {
    file.open("logs/log.txt", std::ios::out | std::ios::app);
  } else {
    file.open(path, std::ios::out | std::ios::app);
  }
}

Logger::~Logger() { file.close(); }

void Logger::log(const char *message, Level level) {
  std::stringstream logEntry;

  // http://en.cppreference.com/w/cpp/chrono/c/strftime
  std::time_t time = std::time(NULL);
  char timeBuffer[100];
  std::strftime(timeBuffer, sizeof(timeBuffer), "%c", std::localtime(&time));

  logEntry << timeBuffer << " [";

  switch (level) {
    case Level::Info:
      logEntry << "INFO";
      break;
    case Level::Warning:
      logEntry << "WARNING";
      break;
    case Level::Error:
      logEntry << "Error";
      break;
  }

  logEntry << "] " << message << std::endl;

  file << logEntry.str();
  std::cout << logEntry.str();
}

void Logger::log(std::stringstream &message, Level level) {
  std::string messageString = message.str();
  logger->log(messageString.c_str(), level);
}

Logger *logger;
