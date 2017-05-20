#pragma once

#include <fstream>
#include <sstream>

class Logger {
 public:
  enum Level { Info, Warning, Error };

  Logger(const char *path = NULL);
  ~Logger();
  void log(const char *message, Level level = Level::Info);
  void log(std::stringstream &message, Level level = Level::Info);

 private:
  std::ofstream file;
};

extern Logger *logger;
